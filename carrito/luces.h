#ifndef LUCES_H
#define LUCES_H
 
#include <Arduino.h>
 
#define LED_R 23
#define LED_G 21
#define LED_B 22
 
// ~LUCES~
class luces {
  public:
    void confLuces() {
      pinMode(LED_R, OUTPUT);
      pinMode(LED_G, OUTPUT);
      pinMode(LED_B, OUTPUT);
      apagar();
    }
    // Direccional izquierda — amarillo (R+G)
    void encenderIzq() {
      digitalWrite(LED_R, HIGH);
      digitalWrite(LED_G, HIGH);
      digitalWrite(LED_B, LOW);
    }
    // Direccional derecha — verde 
    void encenderDer() {
      digitalWrite(LED_R, LOW);
      digitalWrite(LED_G, HIGH);
      digitalWrite(LED_B, LOW);
    }
    // Luces preventivas — rojo sólido
    void encender() {
      digitalWrite(LED_R, HIGH);
      digitalWrite(LED_G, LOW);
      digitalWrite(LED_B, LOW);
    }
    // Apagar todo
    void apagar() {
      digitalWrite(LED_R, LOW);
      digitalWrite(LED_G, LOW);
      digitalWrite(LED_B, LOW);
    }
};
 
#endif