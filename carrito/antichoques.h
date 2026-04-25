// *EN PRUEBA*
#ifndef ANTICHOQUES_H
#define ANTICHOQUES_H

#define TRIG_PIN 5;
#define ECHO_PIN 18;

class antichoques {
    public:
        void confAntichoques(void);
        float lecturaDistancia(void);
};

void antichoques::confAntichoques(void) {
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
}
float antichoques::lecturaDistancia(void) {
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);
    
    long duracion = pulseIn(ECHO_PIN, HIGH);
    
    float distancia = duracion * 0.034 / 2;

    return distancia;
}

#endif