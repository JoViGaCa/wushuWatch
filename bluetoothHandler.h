#ifndef bluetoothHandler_H
#define bluetoothHandler_H

#include "BluetoothSerial.h"
#define USE_NAME
#if !defined(CONFIG_BT_SPP_ENABLED)
#error Serial Bluetooth not available or not enabled. It is only available for the ESP32 chip.
#endif

class bluetoothHandler{
  // client bluetooth to connect to app Android
  private:
      BluetoothSerial SerialBT;
  
  public:
    bluetoothHandler(String newName);
    bool blWrite(String msg);
    String blRead();
    bool isConnected();
  
};



#endif
