#ifndef MOTORES_H
#define MOTORES_H

#include <Arduino.h>

// ============================================================
//  MOTORES
// ============================================================

class motores {
  private:
    // Pines
    static const int PIN_A_IN1 = 27;  // Motor izquierdo — avance
    static const int PIN_A_IN2 = 26;  // Motor izquierdo — retroceso
    static const int PIN_B_IN1 = 25;  // Motor derecho   — avance
    static const int PIN_B_IN2 = 33;  // Motor derecho   — retroceso

    // PWM
    static const int FRECUENCIA   = 5000;
    static const int RESOLUCION   = 8;

    // Velocidad
    static const int VEL_MAX = 180;
    static const int VEL_MIN = 80;

    // Estado interno para la rampa
    int velActualIzq = 0;
    int velActualDer = 0;
    static const int PASO_MS = 5;

    // Aplica PWM real a los pines
    void aplicar(int velIzq, int velDer) {
      // Motor izquierdo
      if (velIzq > 0) {
        int pwm = map(velIzq, 1, VEL_MAX, VEL_MIN, VEL_MAX);
        ledcWrite(PIN_A_IN1, pwm);
        ledcWrite(PIN_A_IN2, 0);
      } else if (velIzq < 0) {
        int pwm = map(abs(velIzq), 1, VEL_MAX, VEL_MIN, VEL_MAX);
        ledcWrite(PIN_A_IN1, 0);
        ledcWrite(PIN_A_IN2, pwm);
      } else {
        ledcWrite(PIN_A_IN1, 0);
        ledcWrite(PIN_A_IN2, 0);
      }

      // Motor derecho
      if (velDer > 0) {
        int pwm = map(velDer, 1, VEL_MAX, VEL_MIN, VEL_MAX);
        ledcWrite(PIN_B_IN1, pwm);
        ledcWrite(PIN_B_IN2, 0);
      } else if (velDer < 0) {
        int pwm = map(abs(velDer), 1, VEL_MAX, VEL_MIN, VEL_MAX);
        ledcWrite(PIN_B_IN1, 0);
        ledcWrite(PIN_B_IN2, pwm);
      } else {
        ledcWrite(PIN_B_IN1, 0);
        ledcWrite(PIN_B_IN2, 0);
      }
    }

  public:
    // Inicializar PWM — llamar desde setup() del .ino
    void confMotores() {
      ledcAttach(PIN_A_IN1, FRECUENCIA, RESOLUCION);
      ledcAttach(PIN_A_IN2, FRECUENCIA, RESOLUCION);
      ledcAttach(PIN_B_IN1, FRECUENCIA, RESOLUCION);
      ledcAttach(PIN_B_IN2, FRECUENCIA, RESOLUCION);
      detenerMotores();
      Serial.println("✓ Motores configurados");
    }

    // Rampa de aceleración — bloqueante
    // NOTA: refactorizar a no-bloqueante antes del Modo 3 completo
    void mover(int objetivoIzq, int objetivoDer) {
      objetivoIzq = constrain(objetivoIzq, -VEL_MAX, VEL_MAX);
      objetivoDer = constrain(objetivoDer, -VEL_MAX, VEL_MAX);

      while (velActualIzq != objetivoIzq || velActualDer != objetivoDer) {
        if (velActualIzq < objetivoIzq) velActualIzq++;
        else if (velActualIzq > objetivoIzq) velActualIzq--;

        if (velActualDer < objetivoDer) velActualDer++;
        else if (velActualDer > objetivoDer) velActualDer--;

        aplicar(velActualIzq, velActualDer);
        delay(PASO_MS);
      }
    }

    // Aplicar velocidad directa sin rampa — para control en tiempo real
    void moverDirecto(int velIzq, int velDer) {
      velActualIzq = constrain(velIzq, -VEL_MAX, VEL_MAX);
      velActualDer = constrain(velDer, -VEL_MAX, VEL_MAX);
      aplicar(velActualIzq, velActualDer);
    }

    // Movimientos básicos con rampa
    void adelante(int v)      { mover(v, v);   }
    void atras(int v)         { mover(-v, -v); }
    void izquierda(int v)     { mover(-v, v);  }
    void derecha(int v)       { mover(v, -v);  }
    void detenerMotores()     { mover(0, 0);   }

    // Freno activo por cortocircuito (DRV8833: IN1=IN2=HIGH)
    void frenoActivo() {
      ledcWrite(PIN_A_IN1, 255);
      ledcWrite(PIN_A_IN2, 255);
      ledcWrite(PIN_B_IN1, 255);
      ledcWrite(PIN_B_IN2, 255);
      velActualIzq = 0;
      velActualDer = 0;
    }
};

#endif
