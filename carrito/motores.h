#ifndef MOTORES_H
#define MOTORES_H

#include <Arduino.h>

// ~MOTORES~
class motores {
  private:

    static const int IN_A1 = 17; // Motor izquierdo — avance
    static const int IN_A2 = 16; // Motor izquierdo — retroceso
    static const int IN_B1 = 18; // Motor derecho   — avance
    static const int IN_B2 = 19; // Motor derecho   — retroceso

    static const int frecuencia = 2000;
    static const int resolucion = 8;
  
    static const int VEL_MAX = 180;
    static const int VEL_MIN = 80;

    int velActualIzq = 0;
    int velActualDer = 0;

    // Aplicar PWM
    void aplicar(int velIzq, int velDer) {
      // Motor izquierdo
      if (velIzq > 0) {
        int PWM = map(velIzq, 1, VEL_MAX, VEL_MIN, VEL_MAX);
        ledcWrite(IN_A1, PWM);
        ledcWrite(IN_A2, 0);
      } else if (velIzq < 0) {
        int PWM = map(abs(velIzq), 1, VEL_MAX, VEL_MIN, VEL_MAX);
        ledcWrite(IN_A1, 0);
        ledcWrite(IN_A2, PWM);
      } else {
        ledcWrite(IN_A1, 0);
        ledcWrite(IN_A2, 0);
      };
      // Motor derecho
      if (velDer > 0) {
        int PWM = map(velDer, 1, VEL_MAX, VEL_MIN, VEL_MAX);
        ledcWrite(IN_B1, PWM);
        ledcWrite(IN_B2, 0);
      } else if (velDer < 0) {
        int PWM = map(abs(velDer), 1, VEL_MAX, VEL_MIN, VEL_MAX);
        ledcWrite(IN_B1, 0);
        ledcWrite(IN_B2, PWM);
      } else {
        ledcWrite(IN_B1, 0);
        ledcWrite(IN_B2, 0);
      }
    }

  public:
    // Inicializar PWM
    void confMotores() {
      ledcAttach(IN_A1, frecuencia, resolucion);
      ledcAttach(IN_A2, frecuencia, resolucion);
      ledcAttach(IN_B1, frecuencia, resolucion);
      ledcAttach(IN_B2, frecuencia, resolucion);
      mover(0, 0);
    }
    // Movimiento con rampa NO bloqueante
    void mover(int objetivoIzq, int objetivoDer) {

      objetivoIzq = constrain(objetivoIzq, -VEL_MAX, VEL_MAX);
      objetivoDer = constrain(objetivoDer, -VEL_MAX, VEL_MAX);

      // Detectar cambio brusco de dirección
      bool cambioIzq =
        (velActualIzq > 0 && objetivoIzq < 0) ||
        (velActualIzq < 0 && objetivoIzq > 0);

      bool cambioDer =
        (velActualDer > 0 && objetivoDer < 0) ||
        (velActualDer < 0 && objetivoDer > 0);

      // Frenito antes de invertir giro
      if (cambioIzq || cambioDer) {
        aplicar(0, 0);

        velActualIzq = 0;
        velActualDer = 0;

        return;
      }

      // Rampa izquierda
      if (velActualIzq < objetivoIzq) {
        velActualIzq += 5;

        if (velActualIzq > objetivoIzq)
          velActualIzq = objetivoIzq;
      }
      else if (velActualIzq > objetivoIzq) {
        velActualIzq -= 5;

        if (velActualIzq < objetivoIzq)
          velActualIzq = objetivoIzq;
      }

      // Rampa derecha
      if (velActualDer < objetivoDer) {
        velActualDer += 5;

        if (velActualDer > objetivoDer)
          velActualDer = objetivoDer;
      }
      else if (velActualDer > objetivoDer) {
        velActualDer -= 5;

        if (velActualDer < objetivoDer)
          velActualDer = objetivoDer;
      }

      // Aplicar PWM
      aplicar(velActualIzq, velActualDer);
    }
};

#endif