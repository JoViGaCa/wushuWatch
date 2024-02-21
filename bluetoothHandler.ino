#include "bluetoothHandler.h"


bluetoothHandler::bluetoothHandler(String newName){
    SerialBT.begin(newName);
}

bool bluetoothHandler::blWrite(String msg){
    if((SerialBT.println(msg)) != 0 ){
      return true;
    } else {
      return false;
    }
}
String bluetoothHandler::blRead(){
    String msg = SerialBT.readString();
    if(msg.length() != 0){
      return msg;
    } else {
      return "Erro na leitura";
    }
  
}

bool bluetoothHandler::isConnected(){
  return SerialBT.connected();
}
