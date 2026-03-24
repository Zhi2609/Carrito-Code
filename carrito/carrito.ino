// Librerias
#include <WiFi.h>

// Constantes WIFI
const char* ssid = "CARRITO_PLUS5";
const char* password = "12345678";

// Configuraciones
WiFiServer server(80);

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
  server.begin();
}

void loop() {

  // Creacion del servidor web
  WiFiClient client = server.available();

  if (client) {
    // Encabezados Pagina Web
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println();

    // Contenido Pagina Web
    client.println("<h1>xd</h1>");
    client.stop();
  }
}