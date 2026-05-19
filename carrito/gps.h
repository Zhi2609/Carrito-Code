// #ifndef GPS_H
// #define GPS_H

// #include <Arduino.h>
// #include <TinyGPS++.h>

// // ~GPS~
// class GPS {
//     private:
//         TinyGPSPlus gps;
//         HardwareSerial gpsSerial = HardwareSerial(1);

//     public:
//         void confGPS() {
//             gpsSerial.begin(115200, SERIAL_8N1, 16, 17); // colocar pines correctos
//             Serial.println("GPS iniciado");
//         }
//         void actualizarGPS() {
//             while (gpsSerial.available()) {
//                 gpsData.encode(gpsSerial.read());
//             }
//         }
//         float obtenerLatitud() {
//             if (gpsData.location.isValid()) {
//                 return gpsData.location.lng();
//             }
//             return 0;
//         }
//         float obtenerLongitud() {
//             if (gpsData.location.isValid()) {
//                 return gpsData.location.lng();
//             }
//             return 0;
//         }
//         float obtenerGrados() {
//             return gpsData.course.deg();
//         }
//         bool disponible() {
//             return gpsData.location.isValid();
//         }
// }

// #endif