#ifndef appSetTime_H
#define appSetTime_H

class appSetTime{
    private:
        int getTnum(TTGOClass *ttgo);
        void prtTime(byte digit, TTGOClass* ttgo);

    public:
        void setTime(TTGOClass* ttgo);
};




#endif
