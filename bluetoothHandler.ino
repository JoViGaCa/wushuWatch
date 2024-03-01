#include "bluetoothHandler.h"


bluetoothHandler::bluetoothHandler(String newName){
    SerialBT.begin(newName,true);
}

bool bluetoothHandler::connectPhone(){
    return SerialBT.connect("WushuPhone");
}

bool bluetoothHandler::connectedPhone(){
  return SerialBT.connected();
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
