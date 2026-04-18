// --- DEFINICIÓN DE PINES ---
#define PIN_MOTOR_A_IN1 27
#define PIN_MOTOR_A_IN2 26
#define PIN_MOTOR_B_IN1 25
#define PIN_MOTOR_B_IN2 33

// --- CONFIG PWM ---
const int frecuencia = 5000;
const int resolucion = 8;

const int canal_A1 = 0;
const int canal_A2 = 1;
const int canal_B1 = 2;
const int canal_B2 = 3;

// --- AJUSTE DE MOTORES calibración ---
int ajusteIzq = 255;
int ajusteDer = 245;

// --- SETUP ---
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

// --- LOOP DE PRUEBA ---
void loop() {

  adelante(150);
  delay(2000);

  detenerMotores();
  delay(1000);

  atras(200);
  delay(2000);

  izquierda(150);
  delay(1500);

  derecha(150);
  delay(1500);

  frenoActivo();
  delay(2000);
}

// --- FUNCIÓN BASE ---
void moverMotores(int velIzq, int velDer) {

  velIzq = constrain(velIzq, -255, 255);
  velDer = constrain(velDer, -255, 255);

  velIzq = velIzq * ajusteIzq / 255;
  velDer = velDer * ajusteDer / 255;

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

// --- FUNCIONES DE MOVIMIENTO ---
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

// --- FRENO ACTIVO ---
void frenoActivo() {
  ledcWrite(canal_A1, 255);
  ledcWrite(canal_A2, 255);
  ledcWrite(canal_B1, 255);
  ledcWrite(canal_B2, 255);
}
