// *EN PRUEBA*
#ifndef SENSORES_h
#define SENSORES_h

class sensores {
    private:
        const int pinesIR[8] = {13, 12, 14, 27, 26, 25, 33, 32};
        int pesos[8] = {-40, -30, -20, 10, 10, 20, 30, 40};
    public:
        void confSensores(void);
        void lecturaSensores(void);
        int errorSensores(void);
};

void sensores::confSensores(void) {
    for (int i = 0; i < 8; i++) {
        pinMode(pinesIR[i], INPUT);
    }
}
void sensores::lecturaSensores(void) {
    Serial.print("\nLectura de sensores: ");

    for (int i = 0; i < 8; i++) {
        Serial.print(", "); 
        Serial.print(digitalRead(pinesIR[i])); 
    }
}
long sensores::errorSensores(void) {
  
  long errorSuma = 0;
  int sensoresActivos = 0;

  for (int i = 0; i < 8; i++) {
    if (digitalRead(pinesIR[i]) == 1) { // CAMBIAR VALOR?
      errorSuma += pesos[i];
      sensoresActivos++;
    }
  }

  if (sensoresActivos == 0) {
    return 999;
  }
  long errorFinal = errorSuma / sensoresActivos;
  
  return errorFinal;
}

#endif