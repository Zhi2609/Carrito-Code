// LIBRERIAS
#include <WiFi.h>
#include <PubSubClient.h>
#include "carrito.h"
#include "sensores.h"
#include "motores.h"
#include "antichoques.h"

carrito carro;
sensores s;
motores m;
antichoques ac;

// CONFIGURACION WIFI
const char* ssid = "CARRITO5_WIFI";
const char* password = "12345678";

// CONFIGURACION MQTT
const char* mqtt_server = "broker.hivemq.com";

// ASIGNACION DE CLIENTES WiFi y MQTT
WiFiClient espClient;
PubSubClient client(espClient);

// VARIABLES
String modo;
int x, y, v;

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

  // Configuracion de componentes
  s.confSensores();
  m.confMotores();
  ac.confAntichoques();
}

// LOOP
void loop() {
  // cliente
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // modo del carrito
  if (modo == "automatico") {
    carro.automatico();
  }
  if (modo == "automaticoSeguro") {
    carro.automaticoSeguro();
  }
  if (modo == "manual") {
    carro.manual(x, y, v);
  }
  if (modo == "gps") {
    carro.gps();
  }
  delay(50);
}

// FUNCIONES BROKER
void reconnect() {
  if (!client.connected()) {
    Serial.println("Conectando a MQTT...");
    
    if (client.connect("CARRITO5")) {
      Serial.println("¡Conectado a MQTT!");
      client.subscribe("c5/carrito/#");
    } else {
      Serial.println("Error al conectar el cliente.");
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
    carro.claxon();
  }
  if (String(topic) == "c5/carrito/automatico") {
    Serial.println("\nModo: automatico");
    modo = "automatico";
  }
  if (String(topic) == "c5/carrito/automaticoSeguro") {
    Serial.println("\nModo: automatico seguro");
    modo = "automaticoSeguro";
  }
  if (String(topic) == "c5/carrito/manual") {
    Serial.println("\nModo: manual");
    sscanf(mensaje.c_str(), "%d,%d,%d", &x, &y, &v);
    modo = "manual";
  }
  if (String(topic) == "c5/carrito/gps") {
    Serial.println("\nModo: gps");
    modo = "gps";
  }
}