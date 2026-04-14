// Librerias
#include <WiFi.h>
#include <PubSubClient.h>

// Configuracion WIFI
const char* ssid = "CARRITO_PLUS5";
const char* password = "12345678";

// Configuracion MQTT
const char* mqtt_server = "broker.hivemq.com";
WiFiClient espClient;
PubSubClient client(espClient);

// SETUP
void setup() {

  Serial.begin(115200);

  // Conexion a WIFI
  WiFi.begin(ssid, password);

  Serial.println("Conectando a la red");
  
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println(WiFi.status());
  }

  Serial.println("¡Conexión exitosa!");
  Serial.print("IP del ESP32: ");
  Serial.println(WiFi.localIP());

  // Creacion del servidor web
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

// Funciones servidor
void reconnect() {
  while (!client.connected()) {
    Serial.println("Conectando a MQTT...");
    
    if (client.connect("CONTROL_CARRITO")) {
      Serial.println("Conectado a MQTT");
      client.subscribe("carrito/#");
    } else {
      Serial.print("Error: ");
      Serial.println(client.state());
      delay(2000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {

  // Procesamiento del callback *NO MOVER*
  String mensaje = "";

  for (int i = 0; i < length; i++) {
    mensaje += (char)payload[i];
  }

  // Seleccion de modo
  Serial.println(mensaje);
  Serial.println(topic);

  if (String(topic) == "carrito/manual") {
    int x, y, v;
    sscanf(mensaje.c_str(), "%d,%d,%d", &x, &y, &v);

    manual(x, y, v);
  }

  if (String(topic) == "carrito/automatico") {
    automatico();
  }
}

// Funciones funcionamiento
void automatico() {
  Serial.println("Modo automático");
}

int manual(int x, int y, int v) {
  Serial.printf("Manual: x=%d y=%d v=%d\n", x, y, v);
}