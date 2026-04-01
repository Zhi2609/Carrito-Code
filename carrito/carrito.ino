// Librerias
#include <WiFi.h>

// Constantes WIFI
const char* ssid = "CARRITO_PLUS5";
const char* password = "12345678";

// Configuraciones
WiFiServer server(80);

// Variables
int x = 0;
int y = 0;
int v = 0;

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

    String request = client.readStringUntil('\r');

    if (request.indexOf("/manual") != -1) {

      int ix = request.indexOf("x=");
      int iy = request.indexOf("y=");
      int iv = request.indexOf("v=");

      if (ix != -1 && iy != -1 && iv != -1) {
        x = request.substring(ix + 2, request.indexOf("&", ix)).toInt();
        y = request.substring(iy + 2, request.indexOf("&", iy)).toInt();
        v = request.substring(iv + 2).toInt();

        manual(x, y, v);
      }
    }

    if (request.indexOf("/automatico") != -1) {
      automatico();
    }

    // Contenido de la pagina web
    client.println(R"rawliteral(<!DOCTYPE html>
<html lang="es">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0, user-scalable=no">
    <title>Carrito</title>
    <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.3.8/dist/css/bootstrap.min.css" rel="stylesheet" integrity="sha384-sRIl4kxILFvY47J16cr9ZwB07vP4J8+LH7qKQnuqkuIAvNWLzeN8tE5YBujZqJLB" crossorigin="anonymous">
</head>
<style>
    #main-article {
        margin: 2%;
    }
    #btn-div {
        margin: 3%;
    }
    #ref-div {
        margin: 5%;
    }
    .title {
        border-bottom: 1px solid;
        border-image: linear-gradient(to right, #6610f2, #c099ff) 1;
    }
    .btn {
        background-color: #6610f2;
        color: #06010f;
        font-family: 'Arial Rounded MT', Helvetica, sans-serif;
    }
    .btn:hover {
        background-color: #06010f;
    }
    body {
        background-color: #06010f;
    }
    h1 {
        font-family: 'Arial Rounded MT', Helvetica, sans-serif;
        color: #6610f2;
    }
    p {
        color: #280561;
        font-family: 'Arial Rounded MT', Helvetica, sans-serif;
    }
</style>
<body id="body" scroll="no" style="overflow: hidden;">
    <header>
        <div class="title">
            <h1 class="text-center">CARRITO</h1>
        </div>
    </header>
    <article id="main-article">
        <p class="text-center">
            Selecciona el modo del carrito dando clic a uno de los botones.
            El modo elegido aparecera abajo como referencia.
        </p>
    </article>
    <main>
        <section>
            <div id="btn-div" class="d-grid gap-2 d-md-flex justify-content-md-center">
                <button type="button" class="btn" id="btn-manual">Manual</button>
                <button type="button" class="btn" id="btn-automatico">Automatico</button>
                <button type="button" class="btn" id="btn-gps">GPS</button>
            </div>
            <div id="div-modo"></div>
        </section>
    </main>
</body>
<script>
const divModo = document.getElementById("div-modo");
let eventosAgregados = false;

// BOTON AUTOMATICO
const btnAutomatico = document.getElementById("btn-automatico");

btnAutomatico.addEventListener("click", () => {

    divModo.innerHTML = "";
    divModo.innerHTML = `<h2 class="text-center" style="color: #280561">MODO AUTOMÁTICO</h2>`;

    fetch("/automatico");
})

// BOTON MANUAL Y JOYSTICK
let canvas, ctx;
let cx, cy, vel, ang;
const btnManual = document.getElementById("btn-manual");

let ultimoEnvio = 0;

btnManual.addEventListener("click", () => {

    divModo.innerHTML = "";
    divModo.innerHTML = `<h2 class="text-center" style="color: #280561">MODO MANUAL</h2>
                        <p class="text-center" style="color: #280561">
                        X: <span id="coordenada_x"></span>
                        Y: <span id="coordenada_y"></span>
                        Velocidad: <span id="velocidad"></span> %
                        Ángulo: <span id="angulo"></span>
                        </p>
                        <canvas id="canvas"></canvas>`;

    canvas = document.getElementById("canvas");
    ctx = canvas.getContext("2d");
    resize(); 

    if (!eventosAgregados) {
        document.addEventListener("mousedown", comenzarDibujo);
        document.addEventListener("mouseup", pararDibujo);
        document.addEventListener("mousemove", dibujar);

        document.addEventListener('touchstart', comenzarDibujo);
        document.addEventListener('touchend', pararDibujo);
        document.addEventListener('touchcancel', pararDibujo);
        document.addEventListener('touchmove', dibujar);

        eventosAgregados = true;
    }
    
    window.addEventListener("resize", resize);

    cx = document.getElementById("coordenada_x");
    cy = document.getElementById("coordenada_y");
    vel = document.getElementById("velocidad");
    ang = document.getElementById("angulo");

    if (cx) cx.innerText = 0;
    if (cy) cy.innerText = 0;
    if (vel) vel.innerText = 0;
    if (ang) ang.innerText = 0;
})

let ancho, alto, radio, origen_x, origen_y;

function resize() {
    ancho = window.innerWidth;
    radio = 50;
    alto = radio * 6.5;
    ctx.canvas.width = ancho;
    ctx.canvas.height = alto;
    fondo();
    joystick(ancho / 2, alto / 3);
}

function fondo() {
    origen_x = ancho / 2;
    origen_y = alto / 3;

    ctx.beginPath();
    ctx.arc(origen_x, origen_y, radio + 20, 0, Math.PI * 2, true);
    ctx.fillStyle = "#06010f";
    ctx.fill();
}

function joystick(ancho, alto) {
    ctx.beginPath();
    ctx.arc(ancho, alto, radio, 0, Math.PI * 2, true);
    ctx.fillStyle = "#6610f2";
    ctx.fill();
    ctx.strokeStyle = "#c099ff";
    ctx.lineWidth = 3;
    ctx.stroke();
}

let coordenadas = { x: 0, y: 0 };
let pintura = false;

function obtenerPosicion(event) {
    let mouse_x = event.clientX || event.touches[0].clientX;
    let mouse_y = event.clientY || event.touches[0].clientY;
    coordenadas.x = mouse_x - canvas.offsetLeft;
    coordenadas.y = mouse_y - canvas.offsetTop;
}

function detectorCirculo() {
    let radio_actual = Math.sqrt(Math.pow(coordenadas.x - origen_x, 2) + Math.pow(coordenadas.y - origen_y, 2));
    if (radio >= radio_actual) return true
    else return false
}


function comenzarDibujo(event) {
    pintura = true;
    obtenerPosicion(event);
    if (detectorCirculo()) {
        ctx.clearRect(0, 0, canvas.width, canvas.height);
        fondo();
        joystick(coordenadas.x, coordenadas.y);
        dibujar(event);
    }
}


function pararDibujo() {
    pintura = false;
    ctx.clearRect(0, 0, canvas.width, canvas.height);
    fondo();
    joystick(ancho / 2, alto / 3);
    if (cx) cx.innerText = 0;
    if (cy) cy.innerText = 0;
    if (vel) vel.innerText = 0;
    if (ang) ang.innerText = 0;
}

function dibujar(event) {

    if (pintura) {
        ctx.clearRect(0, 0, canvas.width, canvas.height);
        fondo();
        let angulo_grados, x, y, velocidad;
        let angulo = Math.atan2((coordenadas.y - origen_y), (coordenadas.x - origen_x));

        if (Math.sign(angulo) == -1) {
            angulo_grados = Math.round(-angulo * 180 / Math.PI);
        }
        else {
            angulo_grados = Math.round(360 - angulo * 180 / Math.PI);
        }


        if (detectorCirculo()) {
            joystick(coordenadas.x, coordenadas.y);
            x = coordenadas.x;
            y = coordenadas.y;
        }
        else {
            x = radio * Math.cos(angulo) + origen_x;
            y = radio * Math.sin(angulo) + origen_y;
            joystick(x, y);
        }
    
        obtenerPosicion(event);

        velocidad =  Math.round(100 * Math.sqrt(Math.pow(x - origen_x, 2) + Math.pow(y - origen_y, 2)) / radio);

        let relativo_x = Math.round(x - origen_x);
        let relativo_y = Math.round(y - origen_y);
        
        if (cx) cx.innerText = relativo_x;
        if (cy) cy.innerText = relativo_y;
        if (vel) vel.innerText = velocidad;
        if (ang) ang.innerText = angulo_grados;

        let ahora = Date.now();
        
        console.log(ahora);

        if (ahora - ultimoEnvio > 100) {
            fetch(`/manual?x=${relativo_x}&y=${relativo_y}&v=${velocidad}`);
            ultimoEnvio = ahora;
        }
    }
}
</script>
</html>
)rawliteral");

    client.stop();
  }
}

// Funciones
void manual(int x, int y, int v) {
    // *EN PROCESO*
}
void automatico() {
    // *EN PROCESO*
}
