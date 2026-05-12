#ifndef GPS_H
#define GPS_H

#include <Arduino.h>

class gps {
    private:
        TinyGPSPlus gps;
        HardwareSerial gpsSerial = HardwareSerial(1);

        float latDestino;
        float lonDestino;

        float latActual;
        float lonActual;

    public:
        void confGPS() {
            gpsSerial.begin(9600, SERIAL_8N1, 16, 17);
            Serial.println("GPS iniciado");
        }
}

#endif