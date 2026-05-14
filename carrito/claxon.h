#ifndef CLAXON_H
#define CLAXON_H
 
#include <Arduino.h>
 
// ============================================================
//  CLAXON — Buzzer pasivo
// ============================================================
 
#define BUZZER_PIN 23
 
class claxon {
  public:
    void confClaxon() {
      pinMode(BUZZER_PIN, OUTPUT);
      digitalWrite(BUZZER_PIN, LOW);
      Serial.println("✓ Claxon configurado");
    }
 
    void activar() {
      tone(BUZZER_PIN, 400);
    }
 
    void desactivar() {
      noTone(BUZZER_PIN);
    }
};
 
#endif
