#ifndef CARRITO_h
#define CARRITO_h
#include "sensores.h"
#include "motores.h"
#include "antichoques.h"
#include "claxon.h"
#include "luces.h"

class carrito {
    private:
        sensores s;
        motores m;
        antichoques ac;
        claxon c;
        luces l;
        unsigned long tClaxon = 0;
        bool claxonActivo = false;
        unsigned long tLuces = 0;
        bool lucesActivas = false;
        unsigned long tAntichoques = 0;
        unsigned long tManual = 0;
    public:
        void claxon(void);
        void direccionales(String);
        void preventivas(void);
        void apagarLuces(void);
        void automatico(void);
        void antichoques(void);
        void manual(int);
        void gps(void);
};

void carrito::claxon(void) {
    c.activar();
    tClaxon = millis();
    claxonActivo = true;
}
void carrito::actualizarClaxon(void) {
    if (claxonActivo && millis() - tClaxon >= 1000) {
        c.desactivar();
        claxonActivo = false;
    }
}
void carrito::direccionales(String luz) {
    if (luz == "izquierda") {
        l.encenderIzq();
        tLuces = millis();
        bool lucesActivas = true;
    } else if (luz == "derecha") {
        l.encenderDer();
        tLuces = millis();
        bool lucesActivas = true;
    }
}
void carrito::preventivas(void) {
    l.encender();
    tLuces = millis();
    bool lucesActivas = true;
}
void carrito::actualizarLuces(String luz) {
    if (lucesActivas && millis() - tLuces >= 500) {
        l.apagar();
        bool lucesActivas = false;
    }
}
void carrito::apagarLuces(void) {
    l.apagar();
    bool lucesActivas = false;
}
void carrito::automatico(void) {
    long error = s.errorSensores();

    switch (error) {
        case -40:
            m.izquierda(120);
            break;
        case -30:
            m.izquierda(100);
            break;
        case -20:
            m.izquierda(80);
            break;
        case 10:
            m.adelante(150);
            break;
        case 20:
            m.derecha(80);
            break;
        case 30:
            m.derecha(100);
            break;
        case 40:
            m.derecha(120);
            break;
        default:
            m.detenerMotores();
            break;
    }
}
void carrito::antichoques(void) {
    long error = s.errorSensores();
    float distancia = ac.lecturaDistancia();
    tAntichoques = millis();

    if (distancia < 15) {
        m.atras(100);
        if (millis() - tAntichoques >= 2000) {
            m.derecha(100);
        }
    } else {
        m.adelante(100)
    }
}
void carrito::manual(int x, int y, int v) {
    tManual = millis()

    if (y < 0 && millis() - tManual >= 1000) {
        m.adelante(v);
        tManual = 0;
    } else if (y > 0 && millis() - tManual >= 1000) {
        m.atras(v);
        tManual = 0;
    } else {
        m.detenerMotores();
    }
    if (x > 0 && millis() - tManual >= 1000) {
        m.derecha(v);
        tManual = 0;
    } else if (x < 0 && millis() - tManual >= 1000) {
        m.izquierda(v);
        tManual = 0;
    } else {
        m.detenerMotores();
    }
}
void carrito::gps(void) { // *FINAL BOSS*
    Serial.println("undefined");
}

#endif