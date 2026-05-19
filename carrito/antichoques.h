#ifndef ANTICHOQUES_H
#define ANTICHOQUES_H
 
#include <Arduino.h>
 
#define TRIG_PIN 32
#define ECHO_PIN 34
 
// ~ANTICHOQUES~
class antichoques {
  public:
    void confAntichoques() {
      pinMode(TRIG_PIN, OUTPUT);
      pinMode(ECHO_PIN, INPUT);
      digitalWrite(TRIG_PIN, LOW);
    }
    float lecturaDistancia() {
      digitalWrite(TRIG_PIN, LOW);
      delayMicroseconds(2);
      digitalWrite(TRIG_PIN, HIGH);
      delayMicroseconds(10);
      digitalWrite(TRIG_PIN, LOW);
 
      long duracion = pulseIn(ECHO_PIN, HIGH, 30000);
 
      if (duracion == 0) return -1;
 
      return (duracion * 0.034) / 2.0;
    }
};
 
#endif