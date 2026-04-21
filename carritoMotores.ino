// ----------- DEFINICIÓN DE PINES -----------
#define PIN_MOTOR_A_IN1 27  // Motor izquierdo
#define PIN_MOTOR_A_IN2 26

#define PIN_MOTOR_B_IN1 25  // Motor derecho
#define PIN_MOTOR_B_IN2 33

// ----------- CONFIGURACIÓN PWM -----------
const int frecuencia = 5000;
const int resolucion = 8;

const int canal_A1 = 0;
const int canal_A2 = 1;
const int canal_B1 = 2;
const int canal_B2 = 3;

// ----------- CONFIGURACIÓN DE VELOCIDAD -----------

const int VELOCIDAD_MAX = 180;
const int VELOCIDAD_MIN = 80;

// ----------- VARIABLES DE CONTROL -----------

int velActualIzq = 0;
int velActualDer = 0;

// ----------- SETUP -----------
void setup() {
  Serial.begin(115200);

  ledcSetup(canal_A1, frecuencia, resolucion);
  ledcSetup(canal_A2, frecuencia, resolucion);
  ledcSetup(canal_B1, frecuencia, resolucion);
  ledcSetup(canal_B2, frecuencia, resolucion);

  ledcAttachPin(PIN_MOTOR_A_IN1, canal_A1);
  ledcAttachPin(PIN_MOTOR_A_IN2, canal_A2);
  ledcAttachPin(PIN_MOTOR_B_IN1, canal_B1);
  ledcAttachPin(PIN_MOTOR_B_IN2, canal_B2);

  detenerMotores();
}


void moverMotores(int objetivoIzq, int objetivoDer) {

  // Limitar velocidad máxima
  objetivoIzq = constrain(objetivoIzq, -VELOCIDAD_MAX, VELOCIDAD_MAX);
  objetivoDer = constrain(objetivoDer, -VELOCIDAD_MAX, VELOCIDAD_MAX);

  while (velActualIzq != objetivoIzq || velActualDer != objetivoDer) {

    if (velActualIzq < objetivoIzq) velActualIzq++;
    if (velActualIzq > objetivoIzq) velActualIzq--;

    if (velActualDer < objetivoDer) velActualDer++;
    if (velActualDer > objetivoDer) velActualDer--;

    aplicarMotores(velActualIzq, velActualDer);

    delay(5);
  }
}

void aplicarMotores(int velIzq, int velDer) {

  // MOTOR IZQUIERDO
  if (velIzq > 0) {
    ledcWrite(canal_A1, velIzq);
    ledcWrite(canal_A2, 0);
  } else if (velIzq < 0) {
    ledcWrite(canal_A1, 0);
    ledcWrite(canal_A2, abs(velIzq));
  } else {
    ledcWrite(canal_A1, 0);
    ledcWrite(canal_A2, 0);
  }

  // MOTOR DERECHO
  if (velDer > 0) {
    ledcWrite(canal_B1, velDer);
    ledcWrite(canal_B2, 0);
  } else if (velDer < 0) {
    ledcWrite(canal_B1, 0);
    ledcWrite(canal_B2, abs(velDer));
  } else {
    ledcWrite(canal_B1, 0);
    ledcWrite(canal_B2, 0);
  }
}


void adelante(int v) {
  moverMotores(v, v);
}

void atras(int v) {
  moverMotores(-v, -v);
}

void izquierda(int v) {
  moverMotores(-v, v);
}

void derecha(int v) {
  moverMotores(v, -v);
}

void detenerMotores() {
  moverMotores(0, 0);
}

void frenoActivo() {
  ledcWrite(canal_A1, 255);
  ledcWrite(canal_A2, 255);
  ledcWrite(canal_B1, 255);
  ledcWrite(canal_B2, 255);
}

void loop() {

  // prueba

  adelante(150);
  delay(2000);

  izquierda(120);
  delay(1500);

  derecha(120);
  delay(1500);

  atras(150);
  delay(2000);

  detenerMotores();
  delay(2000);
}
