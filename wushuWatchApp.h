#ifndef wushuWatchApp_H
#define wushuWatchApp_H
#include "bluetoothHandler.h"


class wushuWatchApp{

  private:
      bluetoothHandler* blH = NULL;
      String codeGetTime = "00000001:";
      String codeAddSch = "00000010:";
      String codeGetSch = "00000011:";
      String evento, dia, mes, hora, minuto;
      String tiposEventos[6] = {"Reun", "Trei", "Trab", "Aula", "Alm", "Play"};

  public:
      void getTime(TTGOClass* ttgo);
      void addSchedule(TTGOClass* ttgo);
      void getSchedule(TTGOClass* ttgo);
      bool sendSchedule(String nome, String dia, String mes, String hora, String minuto);
      bool reqSchedule(String dia, String mes);
      wushuWatchApp(String deviceName);
  
};


#endif
