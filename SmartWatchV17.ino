#include "image.h"
#include <time.h>
#include "config.h"
#include "appBattery.h"
#include "appSetTime.h"
#include "wushuWatchApp.h"
#include "bluetoothHandler.h"
  


// Sprite Settings
// Set delay after plotting the sprite
#define DELAY 30
// Width and height of sprite
#define SWIDTH  240
#define SHEIGHT 240

//Wake on sleep
bool irq = false;

// Watch Settings
TTGOClass   *ttgo  = NULL;
TFT_eSprite *spr    = NULL; // Sprite for meter reading
TFT_eSPI    *tft = NULL;

// Display Time Settings
uint32_t targetTime = 0;       // for next 1 second display update
int hh, mm, ss, mmonth, dday; // H, M, S variables
int yyear; // Year is 16 bit
byte xcolon = 0;


// Apps listados no menu
const int maxApp = 6; // number of apps
String appName[maxApp] = {"Clock",  "Battery", "SetTime", "GetPhoneTime", "addSchedule", "getSchedule"}; // app names

// Classes dos aplicativos
appBattery* aB = NULL;
appSetTime* aST = NULL;
//bluetoothHandler* blH;
wushuWatchApp* wWA = NULL;


void setup(){
    Serial.begin(115200);
    // Inicia relógio
    ttgo = TTGOClass::getWatch();
    ttgo->begin();
    ttgo->openBL();
    
    // Sprite
    tft = ttgo->tft;
    spr   = new TFT_eSprite(tft); // Sprite object for eSpDate
    spr->setColorDepth(8);
    spr->createSprite(SWIDTH, SHEIGHT);
    tft->fillScreen(TFT_BLACK);
    ttgo->tft->setRotation(0);
    spr->pushImage(0, 0, 240, 240, (uint16_t *)background);
    spr->pushSprite(0, 0);

    ttgo->rtc->check();
    ttgo->rtc->syncToSystem();
    displayTime(true); // Our GUI to show the time

    
    pinMode(AXP202_INT, INPUT_PULLUP);
    attachInterrupt(AXP202_INT, [] {
        irq = true;
    }, FALLING);
    //!Clear IRQ unprocessed  first
    ttgo->power->enableIRQ(AXP202_PEK_SHORTPRESS_IRQ, true);
    ttgo->power->clearIRQ();

    pinMode(TOUCH_INT, INPUT);

    // Init apps
    aB = new appBattery();
    aST = new appSetTime();
    wWA = new wushuWatchApp("Relogio_ESP32");
    
  
}

void displayTime(boolean fullUpdate){

    byte xpos = 20; // Stating position for the display
    byte ypos = 20;

    // Get the current data
    RTC_Date tnow = ttgo->rtc->getDateTime();

    hh = tnow.hour;
    mm = tnow.minute;
    ss = tnow.second;
    dday = tnow.day;
    mmonth = tnow.month;
    yyear = tnow.year;

    ttgo->tft->setTextSize(1);

    if (fullUpdate) {
        // Font 7 is a 7-seg display but only contains
        // characters [space] 0 1 2 3 4 5 6 7 8 9 0 : .
        int size_font = 0;

        spr->pushImage(0, 0, 240, 240, (uint16_t *)background);
        spr->pushSprite(0, 0);

        ttgo->tft->setTextColor(0xF800);

        if (hh < 10){ 
          size_font = ttgo->tft->drawChar('0', xpos, ypos, 7);
          xpos += size_font ;
          ypos += 2*(ttgo->tft->drawNumber(hh, xpos, ypos, 7) - 5);
        } else {
          ypos += (ttgo->tft->drawNumber(hh, xpos, ypos, 7) - 5);
        }
        xcolon = xpos + 3;
        if(hh < 10) xpos -= size_font;
        if (mm < 10) xpos += ttgo->tft->drawChar('0', xpos, ypos, 7);
        ttgo->tft->drawNumber(mm, xpos, ypos, 7);
    }

    ttgo->tft->setTextSize(3);
    ttgo->tft->setCursor( 10, 210);

    ttgo->tft->print(dday);
    ttgo->tft->print("/");
    ttgo->tft->print(mmonth);
    ttgo->tft->print("/");
    ttgo->tft->print(yyear);
}

uint8_t modeMenu(){
    int mSelect = 0; // The currently highlighted app
    int16_t x, y;

    boolean exitMenu = false; // used to stay in the menu until user selects app

    setMenuDisplay(0); // display the list of Apps

    while (!exitMenu) {
        if (ttgo->getTouch(x, y)) { // If you have touched something...

            while (ttgo->getTouch(x, y)) {} // wait until you stop touching

            if (y >= 160) { // you want the menu list shifted up
                mSelect += 1;
                if (mSelect == maxApp) mSelect = 0;
                setMenuDisplay(mSelect);
            }

            if (y <= 80) { // you want the menu list shifted down
                mSelect -= 1;
                if (mSelect < 0) mSelect = maxApp - 1;
                setMenuDisplay(mSelect);
            }
            if (y > 80 && y < 160) { // You selected the middle
                exitMenu = true;
            }
        }

    }
    //Return with mSelect containing the desired mode
    ttgo->tft->fillScreen(TFT_BLACK);
    return mSelect;

}

void setMenuDisplay(int mSel){

    int curSel = 0;
    // Display mode header
    ttgo->tft->fillScreen(TFT_BLACK);
    ttgo->tft->fillRect(0, 80, 239, 80, TFT_BLACK);

    // Display apps
    if (mSel == 0) curSel = maxApp - 1;
    else curSel = mSel - 1;

    ttgo->tft->setTextSize(2);
    ttgo->tft->setTextColor(TFT_GREEN);
    ttgo->tft->setCursor(50, 30);
    ttgo->tft->println(appName[curSel]);

    ttgo->tft->setTextSize(3);
    ttgo->tft->setTextColor(TFT_RED);
    ttgo->tft->setCursor(40, 110);
    ttgo->tft->println(appName[mSel]);

    if (mSel == maxApp - 1) curSel = 0;
    else curSel = mSel + 1;

    ttgo->tft->setTextSize(2);
    ttgo->tft->setTextColor(TFT_GREEN);
    ttgo->tft->setCursor(50, 190);
    ttgo->tft->print(appName[curSel]);
}



void loop(void){
    if (targetTime < millis()) {
        targetTime = millis() + 1000;
        displayTime(ss == 0); // Call every second but only update time every minute
    }

    int16_t x, y;
    // se tocar na tela, ativa o menu
    if (ttgo->getTouch(x, y)) {
        while (ttgo->getTouch(x, y)) {} 
        
        switch (modeMenu()) { 
          case 0: 
              break;
          case 1:
              aB->showBattery(ttgo);
              break;
          case 2:
              aST->setTime(ttgo);
              break;
          case 3:
              wWA->getTime(ttgo);
              break;
          case 4:
              wWA->addSchedule(ttgo);
              break;
          case 5:
              wWA->getSchedule(ttgo);
              break;
        }
        spr->pushImage(0, 0, 240, 240, (uint16_t *)background);
        spr->pushSprite(0, 0);
        displayTime(true);
    }

    if (irq) {
        irq = false;
        ttgo->power->readIRQ();
        if (ttgo->power->isPEKShortPressIRQ()) {
            // Clean power chip irq status
            ttgo->power->clearIRQ();

            // Set  touchscreen sleep
            ttgo->displaySleep();

            ttgo->powerOff();

            //Set all channel power off
            ttgo->power->setPowerOutPut(AXP202_LDO3, false);
            ttgo->power->setPowerOutPut(AXP202_LDO4, false);
            ttgo->power->setPowerOutPut(AXP202_LDO2, false);
            ttgo->power->setPowerOutPut(AXP202_EXTEN, false);
            ttgo->power->setPowerOutPut(AXP202_DCDC2, false);

            // TOUCH SCREEN  Wakeup source
            esp_sleep_enable_ext1_wakeup(GPIO_SEL_38, ESP_EXT1_WAKEUP_ALL_LOW);
            // PEK KEY  Wakeup source
            // esp_sleep_enable_ext1_wakeup(GPIO_SEL_35, ESP_EXT1_WAKEUP_ALL_LOW);
            esp_deep_sleep_start();
        }
        ttgo->power->clearIRQ();
    }

}
