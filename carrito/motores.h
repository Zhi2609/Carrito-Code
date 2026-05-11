#ifndef MOTORES_H
#define MOTORES_H

// ----------- DEFINICIÓN DE PINES -----------
#define PIN_MOTOR_A_IN1 27   // Motor izquierdo — avance
#define PIN_MOTOR_A_IN2 26   // Motor izquierdo — retroceso
#define PIN_MOTOR_B_IN1 25   // Motor derecho  — avance
#define PIN_MOTOR_B_IN2 33   // Motor derecho  — retroceso


// ----------- CONFIGURACIÓN PWM -----------
const int FRECUENCIA = 5000;  // Hz — adecuado para DRV8833
const int RESOLUCION = 8;     // bits → rango PWM 0–255


// ----------- CONFIGURACIÓN DE VELOCIDAD -----------
const int VELOCIDAD_MAX = 180; // Duty máximo (sobre 255)
const int VELOCIDAD_MIN = 80;  // Duty mínimo para vencer inercia


// ----------- VARIABLES DE CONTROL -----------
int velActualIzq = 0;
int velActualDer = 0;


// ============================================================
//  SETUP
// ============================================================
void setup() {
  Serial.begin(115200);
  Serial.println("== Carrito — Prueba 1A: Motores ==");

  ledcAttach(PIN_MOTOR_A_IN1, FRECUENCIA, RESOLUCION);
  ledcAttach(PIN_MOTOR_A_IN2, FRECUENCIA, RESOLUCION);
  ledcAttach(PIN_MOTOR_B_IN1, FRECUENCIA, RESOLUCION);
  ledcAttach(PIN_MOTOR_B_IN2, FRECUENCIA, RESOLUCION);

  detenerMotores();
  Serial.println("Motores iniciados. Comenzando secuencia de prueba...");
  delay(1000);
}


// ============================================================
//  APLICAR PWM A LOS PINES FÍSICOS
// ============================================================
void aplicarMotores(int velIzq, int velDer) {

  // --- MOTOR IZQUIERDO ---
  if (velIzq > 0) {
    int pwm = map(velIzq, 1, VELOCIDAD_MAX, VELOCIDAD_MIN, VELOCIDAD_MAX);
    ledcWrite(PIN_MOTOR_A_IN1, pwm);
    ledcWrite(PIN_MOTOR_A_IN2, 0);
  } else if (velIzq < 0) {
    int pwm = map(abs(velIzq), 1, VELOCIDAD_MAX, VELOCIDAD_MIN, VELOCIDAD_MAX);
    ledcWrite(PIN_MOTOR_A_IN1, 0);
    ledcWrite(PIN_MOTOR_A_IN2, pwm);
  } else {
    ledcWrite(PIN_MOTOR_A_IN1, 0);
    ledcWrite(PIN_MOTOR_A_IN2, 0);
  }

  // --- MOTOR DERECHO ---
  if (velDer > 0) {
    int pwm = map(velDer, 1, VELOCIDAD_MAX, VELOCIDAD_MIN, VELOCIDAD_MAX);
    ledcWrite(PIN_MOTOR_B_IN1, pwm);
    ledcWrite(PIN_MOTOR_B_IN2, 0);
  } else if (velDer < 0) {
    int pwm = map(abs(velDer), 1, VELOCIDAD_MAX, VELOCIDAD_MIN, VELOCIDAD_MAX);
    ledcWrite(PIN_MOTOR_B_IN1, 0);
    ledcWrite(PIN_MOTOR_B_IN2, pwm);
  } else {
    ledcWrite(PIN_MOTOR_B_IN1, 0);
    ledcWrite(PIN_MOTOR_B_IN2, 0);
  }
}


// ============================================================
//  RAMPA DE ACELERACIÓN / DESACELERACIÓN
// ============================================================
const int PASO_MS = 5;

void moverMotores(int objetivoIzq, int objetivoDer) {
  objetivoIzq = constrain(objetivoIzq, -VELOCIDAD_MAX, VELOCIDAD_MAX);
  objetivoDer = constrain(objetivoDer, -VELOCIDAD_MAX, VELOCIDAD_MAX);

  while (velActualIzq != objetivoIzq || velActualDer != objetivoDer) {
    if (velActualIzq < objetivoIzq) velActualIzq++;
    else if (velActualIzq > objetivoIzq) velActualIzq--;

    if (velActualDer < objetivoDer) velActualDer++;
    else if (velActualDer > objetivoDer) velActualDer--;

    aplicarMotores(velActualIzq, velActualDer);
    delay(PASO_MS);
  }
}


// ============================================================
//  FUNCIONES DE MOVIMIENTO
// ============================================================
void adelante(int v) {
  Serial.print(">> Adelante  v="); Serial.println(v);
  moverMotores(v, v);
}

void atras(int v) {
  Serial.print(">> Atrás     v="); Serial.println(v);
  moverMotores(-v, -v);
}

void izquierda(int v) {
  Serial.print(">> Izquierda v="); Serial.println(v);
  moverMotores(-v, v);
}

void derecha(int v) {
  Serial.print(">> Derecha   v="); Serial.println(v);
  moverMotores(v, -v);
}

void detenerMotores() {
  Serial.println(">> Stop (coast)");
  moverMotores(0, 0);
}

void frenoActivo() {
  Serial.println(">> Freno activo");
  ledcWrite(PIN_MOTOR_A_IN1, 255);
  ledcWrite(PIN_MOTOR_A_IN2, 255);
  ledcWrite(PIN_MOTOR_B_IN1, 255);
  ledcWrite(PIN_MOTOR_B_IN2, 255);
  velActualIzq = 0;
  velActualDer = 0;
}


void loop() {

}
