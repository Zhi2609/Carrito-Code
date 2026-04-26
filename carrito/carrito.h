#ifndef CARRITO_h
#define CARRITO_h
#include "sensores.h"
#include "motores.h"
#include "antichoques.h"

class carrito {
    private:
        sensores s;
        motores m;
        antichoques ac;
    public:
        void claxon(void);
        void automatico(void);
        void automaticoSeguro(void);
        void manual(int);
        void gps(void);
};

void carrito::claxon(void) {
    Serial.println("\n¡Tocaron el claxon!");
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
void carrito::automaticoSeguro(void) {
    long error = s.errorSensores();
    float distancia = ac.lecturaDistancia();

    if (distancia < 15) {
        m.atras(120);
        delay(500);
        m.derecha(120);
    } else {
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
}
void carrito::manual(int x, int y, int v) {
    Serial.println("undefined");
}
void carrito::gps(void) {
    Serial.println("undefined");
}

#endif