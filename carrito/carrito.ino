// LIBRERIAS
#include <WiFi.h>
#include <PubSubClient.h>
#include "carrito.h"

// CONFIGURACION WIFI
const char* ssid = "CARRITO5_WIFI";
const char* password = "12345678";

// CONFIGURACION MQTT
const char* mqtt_server = "broker.hivemq.com";

// ASIGNACION DE CLIENTES WiFi y MQTT
WiFiClient espClient;
PubSubClient client(espClient);

// SETUP
void setup() {

  Serial.begin(115200);

  // Conexion a WIFI
  WiFi.begin(ssid, password);

  Serial.print("Conectando a la red: ");
  Serial.print(ssid);
  Serial.println("...");
  
  while(WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("\n¡Conexión exitosa!\n");

  // Conexion del broker y callback
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

// LOOP
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}

// FUNCIONES BROKER
void reconnect() {
  while (!client.connected()) {
    Serial.println("Conectando a MQTT...\n");
    
    if (client.connect("CARRITO5")) {
      Serial.println("¡Conectado a MQTT!\n");
      client.subscribe("c5/carrito/#");
    } else {
      Serial.println("Error al conectar el cliente.\n");
      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {

  // Procesamiento del callback ¡NO MOVER!
  String mensaje = "";

  for (int i = 0; i < length; i++) {
    mensaje += (char)payload[i];
  }
  
  // Seleccion de modo
  if (String(topic) == "c5/carrito/claxon") {
    carrito.claxon();
  }
  if (String(topic) == "c5/carrito/automatico") {
    carrito.automatico();
  }
  if (String(topic) == "c5/carrito/automaticoSeguro") {
    carrito.automaticoSeguro();
  }
  if (String(topic) == "c5/carrito/manual") {
    int x, y, v;
    sscanf(mensaje.c_str(), "%d,%d,%d", &x, &y, &v);

    carrito.manual(x, y, v);
  }
  if (String(topic) == "c5/carrito/gps") {
    carrito.gps();
  }
}