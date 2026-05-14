#ifndef SENSORES_H
#define SENSORES_H
 
#include <Arduino.h>
 
// ============================================================
//  SENSORES — Sensor IR TCRT5000 de 5 vías
// ============================================================
 
class sensores {
  private:

    const int pinesIR[5] = { 13, 12, 34, 35, 32 };
    const int pesos[5] = { -20, -10, 0, 10, 20 };
    const int UMBRAL = 2000;
 
  public:
    void confSensores() {
      for (int i = 0; i < 5; i++) {
        pinMode(pinesIR[i], INPUT);
      }
      Serial.println("Sensores IR configurados");
    }
 
    void lecturaSensores() {
      Serial.print("Sensores RAW: ");
      for (int i = 0; i < 5; i++) {
        Serial.print(analogRead(pinesIR[i]));
        Serial.print(" ");
      }
      Serial.println();
    }
 
    long errorSensores() {
      long errorSuma     = 0;
      int  sensoresActivos = 0;
 
      for (int i = 0; i < 5; i++) {
        int lectura = analogRead(pinesIR[i]);

        if (lectura < UMBRAL) {
          errorSuma += pesos[i];
          sensoresActivos++;
        }
      }
 
      if (sensoresActivos == 0) return 999;  // Línea perdida
 
      return errorSuma / sensoresActivos;
    }
};
 
#endif
