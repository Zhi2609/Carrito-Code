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
        void manual(void);
        void gps(void);
};

void carrito::claxon(void) {
    Serial.println("\n¡Tocaron el claxon!");
}
void carrito::automatico(void) {
    Serial.println("\nModo: automatico");
}
void carrito::automaticoSeguro(void) {
    Serial.println("\nModo: automatico seguro");
}
void carrito::manual(void) {
    Serial.println("\nModo: manual");
}
void carrito::gps(void) {
    Serial.println("\nModo: gps");
}

#endif