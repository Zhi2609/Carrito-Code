#ifndef CLAXON_H
#define CLAXON_H
 
#include <Arduino.h>
 
#define BUZZER_PIN 4
 
// ~CLAXON~
class claxon {
  public:
    void confClaxon() {
      pinMode(BUZZER_PIN, OUTPUT);
      digitalWrite(BUZZER_PIN, LOW);
    }
 
    void activar() {
      tone(BUZZER_PIN, 400);
    }
 
    void desactivar() {
      noTone(BUZZER_PIN);
    }
};
 
#endif