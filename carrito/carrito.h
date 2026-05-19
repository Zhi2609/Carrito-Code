#ifndef CARRITO_H
#define CARRITO_H

#include <Arduino.h>

#include "sensores.h"
#include "motores.h"
#include "antichoques.h"
#include "claxon.h"
#include "luces.h"
// #include "gps.h"

// ~CARRITO~
class carrito {
  private:
    // Variables de clases
    sensores s;
    motores m;
    antichoques ac;
    claxon c;
    luces l;
    // GPS gps;

    // Auxiliares de luces
    String tipoLuz;

    // Auxiliares de gps
    float gpsLat;
    float gpsLon;

    // Timers
    unsigned long tClaxon = 0;
    unsigned long tLuces = 0;
    unsigned long tAntichoques = 0;

    // Estados
    bool claxonActivo = false;
    bool lucesActivas = false; 
    bool antichoquesActivo = false;
    bool estadoLuz;

  public:
    // Configuración inicial
    void confCarrito() {
      s.confSensores();
      m.confMotores();
      ac.confAntichoques();
      c.confClaxon();
      l.confLuces();
    }

    // Claxon
    void activarClaxon() {
      c.activar();
      tClaxon = millis();
      claxonActivo = true;
    }
    // Actualizacion de claxon
    void actualizarClaxon() {
      if (claxonActivo && millis() - tClaxon >= 1000) {
        c.desactivar();
        claxonActivo = false;
      }
    }
    // Luces
    void luces(String tipo) {
      tipoLuz = tipo;
      lucesActivas = true; 
    }
    void apagarLuces() {
      l.apagar();
      lucesActivas = false;
      estadoLuz = false;
    }
    // Actualizacion de luces
    void actualizarLuces() {

      if (lucesActivas && millis() - tLuces >= 1000) {

        tLuces = millis();

        estadoLuz = !estadoLuz;

        if (estadoLuz) {
          if (tipoLuz == "izquierda") {
            l.encenderIzq();
          } else if (tipoLuz == "derecha") {
            l.encenderDer();
          } else if (tipoLuz == "preventivas") {
            l.encender();
          }
        } else {
          l.apagar();
        }
      }
    }

    // Modo Automatico
    void automatico() {
      long error = s.lecturaSensores();

      if (error == 999) {
        l.encender();
        m.mover(0, 0);
        return;
      } else {
        l.apagar();
      }

      if (error > 10) {
        m.mover(35, 55);
      }
      else if (error > 0) {
        m.mover(45, 55);
      }
      else if (error < -10) {
        m.mover(55, 35);
      }
      else if (error < 0) {
        m.mover(55, 45);
      }
      else {
        m.mover(50, 50);
      }
    }

    // Modo Antichoques
    void antichoques() {
      float distancia = ac.lecturaDistancia();
      Serial.println(distancia);

      if (distancia > 0 && distancia < 50) {

        if (!antichoquesActivo) {
          antichoquesActivo = true;
          tAntichoques = millis();
        }

        unsigned long tiempo = millis() - tAntichoques;

        if (antichoquesActivo && tiempo < 600) {
          l.encender();
          m.mover(-70, -70);
        }else if (tiempo < 1400) {
          m.mover(0, 100); // !!
        } else if (tiempo >= 1400){
          l.apagar();
          antichoquesActivo = false;
          tAntichoques = 0;
        }
      } else {
        m.mover(50, 50);
      }
    }

    // Modo Manual
   void manual(int x, int y, int v) {

    if (abs(x) < 5) x = 0;
    if (abs(y) < 5) y = 0;

    int vel = map(v, 0, 100, 0, 180);
    int giro = map(x, -35, 35, -80, 80);

    int velIzq = 0;
    int velDer = 0;

    if (y < -10) {
      velIzq = constrain(vel + giro, 0, 180);
      velDer = constrain(vel - giro, 0, 180);
    }
    else if (y > 10) {
      velIzq = constrain(-vel + giro, -180, 0);
      velDer = constrain(-vel - giro, -180, 0);
    }
    else {
      velIzq = giro;
      velDer = -giro;
    }

    m.mover(velIzq, velDer);
  }

  // Modo GPS
  // void gps(float latDestino, float lonDestino) {
  //   if(gps.disponible()) {
  //     latActual = gps.obtenerLatitud();
  //     lonActual = gps.obtenerLongitud();
  //   }
  //   double rumboDestino =
  //     TinyGPSPlus::courseTo(
  //         latActual,
  //         lonActual,
  //         latDestino,
  //         lonDestino
  //     );

  //   double rumboActual = gps.obtenerGrados();
  // }
};

#endif