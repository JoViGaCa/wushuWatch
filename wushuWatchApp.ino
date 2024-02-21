#include "wushuWatchApp.h"


wushuWatchApp::wushuWatchApp(String deviceName){
    blH = new bluetoothHandler(deviceName);
    Serial.begin(115200);
  
}


void wushuWatchApp::getTime(TTGOClass *ttgo){
  ttgo->tft->fillRect(0,0,240,240, TFT_BLACK);

  // Verify if the phone is connected
  if(!blH->isConnected()){
    ttgo->tft->setCursor(10,10);
    ttgo->tft->print("Phone not connected");
    ttgo->tft->setCursor(10,30);
    ttgo->tft->print("Ending app");
    delay(2000);
    return;
    
  }

  // Send the request to the phone
  ttgo->tft->setCursor(10,10);
  ttgo->tft->print("Sending request...");
  blH->blWrite(codeGetTime);

  // Wait for the answer
  ttgo->tft->setCursor(10,30);
  ttgo->tft->print("Waiting answer...");
  int count = 0;
  String newTime = "";
  // Uses the variable count to determine the timeout
  // to increase one to the count it takes about one second, experimentaly
  while(count < 6){
    newTime = blH->blRead();
    if(!newTime.equals("Erro na leitura")){
      break;
    }
    count++;
  }
  // Verify if the answer is valid
  if(count == 6 && newTime.equals("Erro na leitura")){
    ttgo->tft->setCursor(10,50);
    ttgo->tft->print("No response received");
    ttgo->tft->setCursor(10,70);
    ttgo->tft->print("Ending app");
    delay(2000);  
    return;
  }

  // Manipulate the string to obtain the new time
  String aux1, aux2;
  RTC_Date tnow = ttgo->rtc->getDateTime();
  int dday, hh, mm, ss, yyear, mmonth = tnow.month;
  sscanf(newTime.c_str(), "%s %s %d %d:%d:%d GMT-03:00 %d",&aux1, &aux2, &dday, &hh, &mm, &ss, &yyear); 
  yyear = tnow.year;
  // update time
  ttgo->rtc->setDateTime(yyear, mmonth, dday, hh, mm, ss);
  ttgo->tft->setCursor(10,50);
  ttgo->tft->print(newTime);
  ttgo->tft->setCursor(10,90);
  ttgo->tft->print("Ending app");  
  delay(2000);
  
}


void wushuWatchApp::addSchedule(TTGOClass* ttgo){
  bool out = false; 
  int16_t x,y;
  int screen = 0, dia = 1, mes = 1, hora = 1, minuto = 1, event = 0;

  ttgo->tft->fillRect(0,0,240,240,TFT_BLACK);
  ttgo->tft->setCursor(10,10);
  ttgo->tft->print("Create Event");

  while(!out){
    // get touch position
    if(!(ttgo->getTouch(x,y))){
      x = 0;
      y = 0;
    }
    
    //Select Event type
    ttgo->tft->setCursor(10,40);
    ttgo->tft->print("Evento");
    ttgo->tft->setCursor(10,60);
    ttgo->tft->print(tiposEventos[event]);

    ttgo->tft->drawRect(10,90,40,40, TFT_GREEN);
    ttgo->tft->drawTriangle(30,125,15,95,45,95,TFT_GREEN);
    
    if(x > 10 && x < 50 && y > 90 && y < 130){
      ttgo->tft->fillRect(10,60,100,20,TFT_BLACK);
      event++;
      if(event >= 6){
        event = 0;
      }
      delay(100);
    }
    
    
    // switch screens to date and time
    switch(screen%2){
      case 0:
        // head
        ttgo->tft->setCursor(130,40);
        ttgo->tft->print("Dia");
        ttgo->tft->setCursor(180,40);
        ttgo->tft->print("Mes");

        // up
        ttgo->tft->drawRect(130,60,40,40,TFT_GREEN);
        ttgo->tft->drawTriangle(150,65,135,95,165,95,TFT_GREEN);
        ttgo->tft->drawRect(180,60,40,40,TFT_GREEN);
        ttgo->tft->drawTriangle(200,65,185,95,215,95,TFT_GREEN);
        
        if(x > 130 && x < 170 && y > 60 && y < 100){
          ttgo->tft->fillRect(130,110,25,20,TFT_BLACK);
          dia++;
          if(dia >=32)
           dia = 1;
          delay(100);
        }
        if(x > 180 && x < 220 && y > 60 && y < 100){
          ttgo->tft->fillRect(180,110,25,20,TFT_BLACK);
          mes++;
          if(mes >=13)
           mes = 1;
          delay(100);
        }

        // value
        ttgo->tft->setCursor(130,110);
        ttgo->tft->print((String) dia);
        ttgo->tft->setCursor(180,110);
        ttgo->tft->print((String) mes);       

        // down
        ttgo->tft->drawRect(130,130,40,40,TFT_GREEN);
        ttgo->tft->drawTriangle(150,165,135,135,165,135,TFT_GREEN);
        ttgo->tft->drawRect(180,130,40,40,TFT_GREEN);
        ttgo->tft->drawTriangle(200,165,185,135,215,135,TFT_GREEN);
        
        if(x > 130 && x < 170 && y > 130 && y < 170){
          ttgo->tft->fillRect(130,110,25,20,TFT_BLACK);
          dia--;
          if(dia <= 0)
           dia = 31;
          delay(100);
        }
        if(x > 180 && x < 220 && y > 130 && y < 170){
          ttgo->tft->fillRect(180,110,25,20,TFT_BLACK);
          mes--;
          if(mes <= 0)
           mes = 12;
          delay(100);
        }        
        break;
      case 1:
        // head
        ttgo->tft->setCursor(130,40);
        ttgo->tft->print("Hora");
        ttgo->tft->setCursor(180,40);
        ttgo->tft->print("Min");

        // up
        ttgo->tft->drawRect(130,60,40,40,TFT_GREEN);
        ttgo->tft->drawTriangle(150,65,135,95,165,95,TFT_GREEN);
        ttgo->tft->drawRect(180,60,40,40,TFT_GREEN);
        ttgo->tft->drawTriangle(200,65,185,95,215,95,TFT_GREEN);
        
        if(x > 130 && x < 170 && y > 60 && y < 100){
          ttgo->tft->fillRect(130,110,25,20,TFT_BLACK);
          hora++;
          if(hora >=25)
           hora = 1;
          delay(100);
        }
        if(x > 180 && x < 220 && y > 60 && y < 100){
          ttgo->tft->fillRect(180,110,25,20,TFT_BLACK);
          minuto++;
          if(minuto >= 61)
           minuto = 1;
          delay(100);
        }

        // value
        ttgo->tft->setCursor(130,110);
        ttgo->tft->print((String) hora);
        ttgo->tft->setCursor(180,110);
        ttgo->tft->print((String) minuto);       

        // down
        ttgo->tft->drawRect(130,130,40,40,TFT_GREEN);
        ttgo->tft->drawTriangle(150,165,135,135,165,135,TFT_GREEN);
        ttgo->tft->drawRect(180,130,40,40,TFT_GREEN);
        ttgo->tft->drawTriangle(200,165,185,135,215,135,TFT_GREEN);
        
        if(x > 130 && x < 170 && y > 130 && y < 170){
          ttgo->tft->fillRect(130,110,25,20,TFT_BLACK);
          hora--;
          if(hora <= 0)
           hora = 24;
          delay(100);
        }
        if(x > 180 && x < 220 && y > 130 && y < 170){
          ttgo->tft->fillRect(180,110,25,20,TFT_BLACK);
          minuto--;
          if(minuto <= 0)
           minuto = 60;
          delay(100);
        }     
        break;
    }

    // change screen
    ttgo->tft->drawRect(180,180,40,40,TFT_GREEN);
    ttgo->tft->setCursor(185,190);
    ttgo->tft->print("P" + String(screen%2 + 1));

    if(x > 180 && x < 220 && y > 180 && y < 220){
      ttgo->tft->fillRect(130,40,90,180,TFT_BLACK);
      screen++;
      delay(100);
    }

    // save and send
    ttgo->tft->drawRect(95,180,50,40,TFT_GREEN);
    ttgo->tft->setCursor(100,190);
    ttgo->tft->print("Send");

    if(x > 95 && x < 145 & y > 180 && y < 220){
    
      if(this->sendSchedule(tiposEventos[event], (String) dia, (String) mes, (String) hora, (String) minuto)){
        ttgo->tft->fillRect(10,10,140,20,TFT_BLACK);
        ttgo->tft->setCursor(10,10);
        ttgo->tft->print("Schedule Send");
      } else {
        ttgo->tft->fillRect(10,10,140,20,TFT_BLACK);
        ttgo->tft->setCursor(10,10);
        ttgo->tft->print("Phone not connected");
      }
      delay(1000);
      out = true;
    }


    // get out
    ttgo->tft->setCursor(15,190);
    ttgo->tft->print("OUT");
    ttgo->tft->drawRect(10,180,40,40,TFT_GREEN);
    if(x > 10 && x < 50 && y > 180 && y < 220){
      out = true;
    }

    
  }
  
}

bool wushuWatchApp::sendSchedule(String nome, String dia, String mes, String hora, String minuto){
    String schedule = codeAddSch + nome +":" + dia + ":" + mes + ":" + hora + ":" + minuto + ":";

    if(!blH->isConnected()){
      return false;
    } else {
      //send
      return blH->blWrite(schedule);
    } 
   
}

void wushuWatchApp::getSchedule(TTGOClass* ttgo){
  bool out = false; 
  int16_t x,y;
  int dia = 1, mes = 1;

  ttgo->tft->fillRect(0,0,240,240,TFT_BLACK);
  ttgo->tft->setCursor(10,10);
  ttgo->tft->print("Get schedule");

  while(!out){
    // get touch position
    if(!(ttgo->getTouch(x,y))){
      x = 0;
      y = 0;
    }

    // Select Day and Month
    // head
    ttgo->tft->setCursor(10,40);
    ttgo->tft->print("Dia");
    ttgo->tft->setCursor(60,40);
    ttgo->tft->print("Mes");

    // up
    ttgo->tft->drawRect(10,60,40,40,TFT_GREEN);
    ttgo->tft->drawTriangle(30,65,15,95,45,95,TFT_GREEN);
    ttgo->tft->drawRect(60,60,40,40,TFT_GREEN);
    ttgo->tft->drawTriangle(80,65,65,95,95,95,TFT_GREEN);
    
    if(x > 10 && x < 50 && y > 60 && y < 100){
      ttgo->tft->fillRect(10,110,25,20,TFT_BLACK);
      dia++;
      if(dia >=32)
       dia = 1;
      delay(100);
    }
    if(x > 60 && x < 100 && y > 60 && y < 100){
      ttgo->tft->fillRect(60,110,25,20,TFT_BLACK);
      mes++;
      if(mes >=13)
       mes = 1;
      delay(100);
    }

    // value
    ttgo->tft->setCursor(10,110);
    ttgo->tft->print((String) dia);
    ttgo->tft->setCursor(60,110);
    ttgo->tft->print((String) mes);       

    // down
    ttgo->tft->drawRect(10,130,40,40,TFT_GREEN);
    ttgo->tft->drawTriangle(30,165,15,135,45,135,TFT_GREEN);
    ttgo->tft->drawRect(60,130,40,40,TFT_GREEN);
    ttgo->tft->drawTriangle(80,165,65,135,95,135,TFT_GREEN);
    
    if(x > 10 && x < 50 && y > 130 && y < 170){
      ttgo->tft->fillRect(10,110,25,20,TFT_BLACK);
      dia--;
      if(dia <= 0)
       dia = 31;
      delay(100);
    }
    if(x > 60 && x < 100 && y > 130 && y < 170){
      ttgo->tft->fillRect(60,110,25,20,TFT_BLACK);
      mes--;
      if(mes <= 0)
       mes = 12;
      delay(100);
    }

    // request
    ttgo->tft->drawRect(95,180,50,40,TFT_GREEN);
    ttgo->tft->setCursor(100,190);
    ttgo->tft->print("Req");

    if(x > 95 && x < 145 & y > 180 && y < 220){
      // make request
      if(!this->reqSchedule((String) dia, (String) mes)){
        ttgo->tft->fillRect(10,10,140,20,TFT_BLACK);
        ttgo->tft->setCursor(10,10);
        ttgo->tft->print("Phone not connected");
      } else {
        // wait for the answer
        String answer = "";
        int count = 0;
        while(count <= 6){
          answer = blH->blRead();
          if(!answer.equals("Erro na leitura")){
            break;
          }
          count++;
        }
        if(answer.equals("Erro na leitura")){
          ttgo->tft->fillRect(10,10,140,20,TFT_BLACK);
          ttgo->tft->setCursor(10,10);
          ttgo->tft->print("Error in answer");  
        } else {
          ttgo->tft->fillRect(110,40,90,180,TFT_BLACK);
          // Print events 
          char* event;
          char auxiliar[1024];
          strcpy(auxiliar,answer.c_str());
          String nameEvent = "";
          int day, month, hour, minute;
          int pos = 40;
          event = strtok(auxiliar, "|");
          bool readEvent = true;
          while(readEvent){
            ttgo->tft->setCursor(110,pos);
            ttgo->tft->print(event);
            event = strtok(NULL,"|");
            pos+= 20;
            if(event == NULL){
              readEvent = false;
              break;
            }
          }
        }
      }
    }

    // get out
    ttgo->tft->setCursor(15,190);
    ttgo->tft->print("OUT");
    ttgo->tft->drawRect(10,180,40,40,TFT_GREEN);
    if(x > 10 && x < 50 && y > 180 && y < 220){
      out = true;
    }
    
    
  }
  
}

bool wushuWatchApp::reqSchedule(String dia, String mes){
    String schedule = codeGetSch + dia + ":" + mes + ":";

    if(!blH->isConnected()){
      return false;
    } else {
      //send
      return blH->blWrite(schedule);
    } 
   
}
