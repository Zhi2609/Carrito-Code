#ifndef CARRITO_H
#define CARRITO_H

#include <Arduino.h>
#include "sensores.h"
#include "motores.h"
#include "antichoques.h"
#include "claxon.h"
#include "luces.h"

// ============================================================
//  CARRITO — Clase principal de control
// ============================================================

class carrito {
  private:
    sensores    s;
    motores     m;
    antichoques ac;
    claxon      c;
    luces       l;

    // Timers internos
    unsigned long tClaxon      = 0;
    unsigned long tLuces       = 0;
    unsigned long tAntichoques = 0;

    // Estados
    bool claxonActivo = false;
    bool lucesActivas = false;

    // Estado antichoques
    enum EstadoAntichoques {
      ANTICHOQUES_AVANZAR = 0,
      ANTICHOQUES_RETROCEDER,
      ANTICHOQUES_GIRAR
    };

    EstadoAntichoques estadoAntichoques = ANTICHOQUES_AVANZAR;
    bool giroDerecha = true;
    unsigned long tSensor = 0;
    unsigned long tUltimaDistancia = 0;
    float distanciaCm = -1.0f;

  public:
    // --- Configuración inicial ---
    void confCarrito() {
      s.confSensores();
      m.confMotores();
      ac.confAntichoques();
      c.confClaxon();
      l.confLuces();
    }

    // --- Claxon ---
    void activarClaxon() {
      c.activar();
      tClaxon      = millis();
      claxonActivo = true;
    }

    void actualizarClaxon() {
      if (claxonActivo && millis() - tClaxon >= 1000) {
        c.desactivar();
        claxonActivo = false;
      }
    }

    // --- Luces ---
    void direccionales(String lado) {
      if (lado == "izquierda") {
        l.encenderIzq();
      } else if (lado == "derecha") {
        l.encenderDer();
      }
      tLuces       = millis();
      lucesActivas = true; 
    }

    void preventivas() {
      l.encender();
      tLuces       = millis();
      lucesActivas = true;
    }

    void actualizarLuces() {
      if (lucesActivas && millis() - tLuces >= 500) {
        l.apagar();
        lucesActivas = false;
      }
    }

    void apagarLuces() {
      l.apagar();
      lucesActivas = false;
    }

    // --- Modo automático (seguidor de línea) ---
    // Pesos del sensor de 5 vías: {-20, -10, 0, 10, 20}
    void automatico() {
      long error = s.errorSensores();

      if (error == 999) {
        // Línea perdida — frenar
        m.detenerMotores();
        return;
      }

      // Control proporcional directo
      // KP = 3: ajustar si zigzaguea (bajar) o no corrige (subir)
      const float KP = 3.0;
      int correccion = (int)(KP * error);
      int velBase    = 120;

      int velIzq = constrain(velBase - correccion, 0, 180);
      int velDer = constrain(velBase + correccion, 0, 180);

      m.moverDirecto(velIzq, velDer);
    }

    // --- Modo antichoques ---
    void antichoques() {
      const int VEL_AVANCE   = 120;
      const int VEL_LENTO    = 80;
      const int VEL_RETRO    = 110;
      const int VEL_GIRO     = 110;

      const int UMBRAL_CERCA = 18;
      const int UMBRAL_LENTO = 30;

      const unsigned long T_RETRO  = 350;
      const unsigned long T_GIRO   = 450;
      const unsigned long T_SENSOR = 80;

      if (millis() - tSensor >= T_SENSOR) {
        float lectura = ac.lecturaDistancia();

        if (lectura > 0) {
          tUltimaDistancia = millis();
          if (distanciaCm < 0) distanciaCm = lectura;
          else distanciaCm = (distanciaCm * 0.7f) + (lectura * 0.3f);
        } else if (millis() - tUltimaDistancia > 300) {
          distanciaCm = -1;
        }

        tSensor = millis();
      }

      bool obstaculo = (distanciaCm > 0 && distanciaCm <= UMBRAL_CERCA);

      switch (estadoAntichoques) {
        case ANTICHOQUES_AVANZAR: {
          if (obstaculo) {
            estadoAntichoques = ANTICHOQUES_RETROCEDER;
            tAntichoques = millis();
            break;
          }

          int vel = VEL_AVANCE;
          if (distanciaCm > 0 && distanciaCm < UMBRAL_LENTO) {
            vel = map((int)distanciaCm, UMBRAL_CERCA, UMBRAL_LENTO, VEL_LENTO, VEL_AVANCE);
            vel = constrain(vel, VEL_LENTO, VEL_AVANCE);
          }

          m.moverDirecto(vel, vel);
          break;
        }

        case ANTICHOQUES_RETROCEDER:
          m.moverDirecto(-VEL_RETRO, -VEL_RETRO);
          if (millis() - tAntichoques >= T_RETRO) {
            estadoAntichoques = ANTICHOQUES_GIRAR;
            tAntichoques = millis();
            giroDerecha = !giroDerecha;
          }
          break;

        case ANTICHOQUES_GIRAR:
          if (giroDerecha) m.moverDirecto(VEL_GIRO, -VEL_GIRO);
          else m.moverDirecto(-VEL_GIRO, VEL_GIRO);

          if (millis() - tAntichoques >= T_GIRO) {
            estadoAntichoques = ANTICHOQUES_AVANZAR;
          }
          break;
      }
    }

    // --- Modo manual (joystick desde app) ---
    // x: -100 a 100 (izq/der), y: -100 a 100 (atrás/adelante), v: 0 a 100 (velocidad %)
    void manual(int x, int y, int v) {
      // Mapear velocidad del joystick (0-100%) al rango real del motor
      int vel = map(v, 0, 100, 0, 180);

      // Movimiento diagonal: combinar X e Y
      // Si Y domina → adelante/atrás, si X domina → giro
      if (abs(y) >= abs(x)) {
        // Movimiento principal en Y
        if (y < -10) {
          // Joystick arriba → adelante
          // Corregir dirección con X para curvas suaves
          int velIzq = constrain(vel + x, 0, 180);
          int velDer = constrain(vel - x, 0, 180);
          m.moverDirecto(velIzq, velDer);
        } else if (y > 10) {
          // Joystick abajo → atrás
          int velIzq = constrain(-vel - x, -180, 0);
          int velDer = constrain(-vel + x, -180, 0);
          m.moverDirecto(velIzq, velDer);
        } else {
          m.detenerMotores();
        }
      } else {
        // Movimiento principal en X → giro en sitio
        if (x > 10) {
          m.moverDirecto(vel, -vel);   // derecha
        } else if (x < -10) {
          m.moverDirecto(-vel, vel);   // izquierda
        } else {
          m.detenerMotores();
        }
      }
    }

    // --- Modo GPS ---
    void gps() {
      // xd
    }
};

#endif
