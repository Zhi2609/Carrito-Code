#ifndef LUCES_H
#define LUCES_H

#define ledA 1  // Cambiar valores
#define ledB 2

class luces {
    public:
        void confLuces(void);
        void encenderIzq(void);
        void encender(void);
        void encenderDer(void);
        void apagar(void);
};

void luces::confLuces(void) {
    pinMode(ledA, OUTPUT);
    pinMode(ledB, OUTPUT);
}
void luces::encenderIzq(void) {
    digitalWrite(ledB, HIGH);
}
void luces::encender(void) {
    digitalWrite(ledA, HIGH);
    digitalWrite(ledB, HIGH);
}
void luces::encenderDer(void) {
    digitalWrite(ledA, HIGH);
}
void luces::apagar(void) {
    digitalWrite(ledA, LOW);
    digitalWrite(ledB, LOW);
}