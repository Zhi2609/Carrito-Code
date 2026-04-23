#include <TinyGPSPlus.h>

const int TRIG_PIN = 5;
const int ECHO_PIN = 18;

const int pinesIR[8] = {13, 12, 14, 27, 26, 25, 33, 32}; // Cambiar segun los pines del ESP32
int estadoLinea[8];

TinyGPSPlus gps;
HardwareSerial SerialGPS(2); 
const int RX_GPS = 16;
const int TX_GPS = 17;

float distanciaObstaculo = 0.0;
double latitudActual = 0.0;
double longitudActual = 0.0;

void setup() {
  Serial.begin(115200);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  for (int i = 0; i < 8; i++) {
    pinMode(pinesIR[i], INPUT);
  }

  SerialGPS.begin(9600, SERIAL_8N1, RX_GPS, TX_GPS);
}

float leerDistancia() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  long duracion = pulseIn(ECHO_PIN, HIGH);
  
  float distancia = duracion * 0.034 / 2;
  
  distanciaObstaculo = distancia;
  return distancia;
}

void leerSensoresLinea() {
  Serial.print("Lectura de línea: ");
  for (int i = 0; i < 8; i++) {
    estadoLinea[i] = digitalRead(pinesIR[i]);

    Serial.print(estadoLinea[i]); 
    Serial.print(" ");
  }
  Serial.println();
}

void actualizarGPS() {
  while (SerialGPS.available() > 0) {
    if (gps.encode(SerialGPS.read())) {
      if (gps.location.isValid()) {
        latitudActual = gps.location.lat();
        longitudActual = gps.location.lng();
      }
    }
  }
}

void imprimirUbicacionGPS() {
  if (gps.location.isValid()) {
    Serial.print("Latitud: ");
    Serial.print(latitudActual, 6);
    Serial.print(" | Longitud: ");
    Serial.println(longitudActual, 6);
  } else {
    Serial.println("GPS: Esperando señal de satélites...");
  }
}

int calcularErrorLinea() {
  leerSensoresLinea();

  int pesos[8] = {-40, -30, -20, -10, 10, 20, 30, 40};
  
  long errorSuma = 0;
  int sensoresActivos = 0;

  for (int i = 0; i < 8; i++) {
    if (estadoLinea[i] == 1) {
      errorSuma += pesos[i];
      sensoresActivos++;
    }
  }

  if (sensoresActivos == 0) {
    return 999;
  }
  int errorFinal = errorSuma / sensoresActivos;
  
  return errorFinal;
}