#ifndef SENSORES_H
#define SENSORES_H
 
#include <Arduino.h>
 
// ~SENSORES~
class sensores {
  private:
    const int pinesIR[5] = { 27, 33, 13, 25, 26 };
    const int sensores[5] = {27, 33, 35, 34, 26};

  public:
    void confSensores() {
      for (int i = 0; i < 5; i++) {
        pinMode(pinesIR[i], INPUT);
      }
    }
    long lecturaSensores() {
      long errorSuma = 0;
      int sensoresActivos = 0;

      for (int i = 0; i < 5; i++) {
        int lectura = digitalRead(pinesIR[i]);

        if (lectura == LOW) {
          errorSuma += pesos[i];
          sensoresActivos++;
        }
      }
 
      if (sensoresActivos == 0) return 999; // Línea perdida
 
      return errorSuma / sensoresActivos;
    }
};
 
#endif