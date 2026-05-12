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
    bool retrocediendo = false;

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
      float distancia = ac.lecturaDistancia();

      if (distancia > 0 && distancia < 15) {
        // Obstáculo detectado — retroceder
        if (!retrocediendo) {
          tAntichoques  = millis();
          retrocediendo = true;
        }
        m.atras(100);

        if (millis() - tAntichoques >= 2000) {
          m.derecha(100);
        }
      } else {
       
        retrocediendo = false;
        m.adelante(100);
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
      console.log("xd")
    }
};

#endif
