#ifndef CLAXON_H
#define CLAXON_H

#define buzzer 8;

class claxon {
    public:
        void confClaxon(void);
        void activar(void);
        void desactivar(void);
};

void claxon::confClaxon(void) {
    pinMode(buzzer, OUTPUT);
}
void claxon::activar(void) {
    tone(buzzer, 400);
}
void claxon::desactivar(void) {
    noTone(buzzer);
}

#endif