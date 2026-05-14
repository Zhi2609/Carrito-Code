// LIBRERIAS
#include <WiFi.h>
#include <PubSubClient.h>
#include "carrito.h"
#include "sensores.h"
#include "motores.h"
#include "antichoques.h"
#include "claxon.h"
#include "luces.h"

// ----------- OBJETOS -----------
carrito carro;

// ----------- CONFIGURACIÓN WiFi -----------
// cambiar segun tu red wifi
const char* ssid     = "iss";
const char* password = "1234567f";

// ----------- CONFIGURACIÓN MQTT -----------
const char* mqtt_server = "broker.hivemq.com";

// ----------- CLIENTES -----------
WiFiClient   espClient;
PubSubClient client(espClient);

// ----------- VARIABLES -----------
String modo  = "";
String extra = "";
int x = 0, y = 0, v = 0;
float lat, lon;

// ----------- TIMERS -----------
unsigned long ultimoIntento = 0;
unsigned long ultimoLoop    = 0;


// ============================================================
//  SETUP
// ============================================================
void setup() {
  Serial.begin(115200);
  delay(500);

  Serial.println("==============================================");
  Serial.println("  CARRITO INTELIGENTE");
  Serial.println("==============================================");

  // Inicializar hardware
  carro.confCarrito();

  // Conexión WiFi
  WiFi.begin(ssid, password);
  Serial.print("Conectando a WiFi: ");
  Serial.println(ssid);

  int intentos = 0;
  while (WiFi.status() != WL_CONNECTED && intentos < 20) {
    delay(500);
    Serial.print(".");
    intentos++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n WiFi conectado");
    Serial.print("  IP: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\nNo se pudo conectar al WiFi");
  }

  // Configurar MQTT
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  client.setKeepAlive(15);

  Serial.println("Esperando comandos MQTT...\n");
}


// ============================================================
//  LOOP
// ============================================================
void loop() {
  // Mantener conexión MQTT
  reconnect();
  client.loop();

  // Actualizar timers de claxon y luces
  carro.actualizarClaxon();
  carro.actualizarLuces();

  // --- Procesar extras (luces y claxon) ---
  if (extra == "claxon") {
    carro.activarClaxon();
    extra = "";
  } else if (extra == "lucesIzq") {
    carro.direccionales("izquierda");
    extra = "";
  } else if (extra == "lucesDer") {
    carro.direccionales("derecha");
    extra = "";
  } else if (extra == "lucesPrev") {
    carro.preventivas();
    extra = "";
  } else if (extra == "") {
    carro.apagarLuces();
  }

  if (millis() - ultimoLoop >= 50) {
    ultimoLoop = millis();

    if (modo == "automatico") {
      carro.automatico();
    } else if (modo == "antichoques") {
      carro.antichoques();
    } else if (modo == "manual") {
      carro.manual(x, y, v);
    } else if (modo == "gps") {
      carro.gps();
    }
  }
}


// ============================================================
//  RECONEXIÓN MQTT — Sin bloquear el loop
// ============================================================
void reconnect() {
  if (client.connected()) return;
  if (millis() - ultimoIntento < 3000) return;

  ultimoIntento = millis();
  Serial.print("Conectando a MQTT... ");

  if (client.connect("CARRITO5")) {
    Serial.println("✓ Conectado");
    client.subscribe("c5/carrito/#");
  } else {
    Serial.print("✗ Error rc=");
    Serial.print(client.state());
    Serial.println(" — reintentando en 3s");
  }
}


// ============================================================
//  CALLBACK MQTT
// ============================================================
void callback(char* topic, byte* payload, unsigned int length) {
  String mensaje = "";
  for (unsigned int i = 0; i < length; i++) {
    mensaje += (char)payload[i];
  }

  String t = String(topic);

  Serial.print("✓ [");
  Serial.print(t);
  Serial.print("] ");
  Serial.println(mensaje);

  // --- Modos ---
  if (t == "c5/carrito/automatico") {
    modo = "automatico";
    Serial.println("→ Modo: AUTOMÁTICO");
  }
  else if (t == "c5/carrito/antichoques") {
    modo = "antichoques";
    Serial.println("→ Modo: ANTICHOQUES");
  }
  else if (t == "c5/carrito/manual") {
    modo = "manual";
    // Parsear correctamente "Modo: manual | Direcciones: x,y,v"
    int idx = mensaje.indexOf("Direcciones: ");
    if (idx != -1) {
      String coords = mensaje.substring(idx + 13);
      sscanf(coords.c_str(), "%d,%d,%d", &x, &y, &v);
    }
    Serial.print("→ Modo: MANUAL  x=");
    Serial.print(x);
    Serial.print(" y=");
    Serial.print(y);
    Serial.print(" v=");
    Serial.println(v);
  }
  else if (t == "c5/carrito/gps") {
    modo = "gps";

    sscanf(mensaje.c_str(), "%f,%f", &lat, &lon);

    Serial.println("→ Modo: GPS | ");
    Serial.print(" altitude=");
    Serial.print(lat);
    Serial.print(" longitude=");
    Serial.println(lon);
  }

  // --- Extras ---
  else if (t == "c5/carrito/claxon") {
    extra = "claxon";
    Serial.println("→ Extra: CLAXON");
  }
  else if (t == "c5/carrito/lucesIzq") {
    extra = "lucesIzq";
    Serial.println("→ Extra: LUCES IZQ");
  }
  else if (t == "c5/carrito/lucesDer") {
    extra = "lucesDer";
    Serial.println("→ Extra: LUCES DER");
  }
  else if (t == "c5/carrito/lucesPrev") {
    extra = "lucesPrev";
    Serial.println("→ Extra: LUCES PREV");
  }
  else if (t == "c5/carrito/lucesApagar") {
    extra = "";
    carro.apagarLuces();
    Serial.println("→ Extra: LUCES APAGADAS");
  }
}
