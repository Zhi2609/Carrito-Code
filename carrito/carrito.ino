// LIBRERIAS
#include <WiFi.h>
#include <PubSubClient.h>
#include "carrito.h"
#include "sensores.h"
#include "motores.h"
#include "antichoques.h"
#include "claxon.h"
#include "luces.h"

carrito carro;
sensores s;
motores m;
antichoques ac;
claxon c;
luces l;

// CONFIGURACION WIFI
const char* ssid = "CARRITO5_WIFI";
const char* password = "12345678";

// CONFIGURACION MQTT
const char* mqtt_server = "broker.hivemq.com";

// ASIGNACION DE CLIENTES WiFi y MQTT
WiFiClient espClient;
PubSubClient client(espClient);

// VARIABLES
String modo = "automatico";
String extra = "";
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
  m.detenerMotores();
  ac.confAntichoques();
  c.confClaxon();
  l.confLuces();
}

// LOOP
void loop() {
  // cliente
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Funcionalidades extra
  // if (extra == "") {
  //   carro.apagarLuces();
  // } else if(extra == "claxon") {
  //   carro.claxon();
  //   carro.actualizarClaxon();
  //   extra = "";
  // } else if (extra == "lucesIzq") {
  //   carro.direccionales(izquierda);
  //   carro.actualizarLuces();
  //   extra = "";
  // } else if (extra == "lucesDer") {
  //   carro.direccionales(izquierda);
  //   carro.actualizarLuces();
  //   extra = "";
  // } else if (extra == "lucesPrev") {
  //   carro.preventivas();
  //   carro.actualizarLuces();
  //   extra = "";
  // }

  // Modo del carrito
  // if (modo == "automatico") {
  //   carro.automatico();
  // }
  // if (modo == "antichoques") {
  //   carro.antichoques();
  // }
  if (modo == "manual") {
    carro.manual(x, y, v);
  }
  // if (modo == "gps") {
  //   carro.gps();
  // }
  delay(150);
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
  if (String(topic) == "c5/carrito/lucesIzq") {
    Serial.println("\nDireccional izquierda activada");
    extra = "lucesIzq";
  }
  if (String(topic) == "c5/carrito/lucesPrev") {
    Serial.println("\nLuces preventivas activadas");
    extra = "lucesPrev";
  }
  if (String(topic) == "c5/carrito/lucesDer") {
    Serial.println("\nDireccional derecha activada");
    extra = "lucesDer";
  }
    if (String(topic) == "c5/carrito/lucesApagar") {
    Serial.println("\nSe apagaron las luces");
    extra = "";
  }
  if (String(topic) == "c5/carrito/claxon") {
    Serial.println("\n¡Tocaron el claxon!");
    extra = "claxon";
  }
  if (String(topic) == "c5/carrito/automatico") {
    Serial.println("\nModo: automatico");
    modo = "automatico";
  }
  if (String(topic) == "c5/carrito/antichoques") {
    Serial.println("\nModo: antichoques");
    modo = "antichoques";
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