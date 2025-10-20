#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "CESJT";
const char* password = "itisjtsmg";

WebServer server(80);
String textoBoton="XXXX";

bool ledEncendido = true;

// pagina
const char pagina_template[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset='utf-8'>
  <title>Servidor Web ESP32</title>
</head>
<body>
  <style>
    body { 
      background-color: #ecf0f1; 
      text-align: center; 
    }
    .titulo { 
      font-family: 'Arial Black';
      color:  #2c3e50;
    }
    .botones{
      display:block;
    }
    .boton_apagado{
      width: 180px;
      height: 60px;
      border: 1px solid black;
      border-radius:15px;
      background-color: #c0392b;
    }
    .boton_lento{ 
      width: 180px;
      height: 60px;
      border: 1px solid black;
      border-radius:15px;
      background-color: #2980b9;
    }
    .boton_medio{ 
      width: 180px;
      height: 60px;
      border: 1px solid black;
      border-radius:15px;
      background-color: #27ae60;
    }
    .boton_rapido{ 
      width: 180px;
      height: 60px;
      border: 1px solid black;
      border-radius:15px;
      background-color: #f39c12;
    }
  </style>
  <h1 class='titulo'>CONTROL DE VENTILADOR</h1>
  <p>Seleccione el modo de su ventilador:</p>
  <div class="botones">
    <a href="/apagado"><button class="btn boton_apagado">APAGADO</button></a> <br>
    <a href="/lento"><button class="btn boton_lento">LENTO</button></a> <br>
    <a href="/medio"><button class="btn boton_medio">MEDIO</button></a> <br>
    <a href="/rapido"><button class="btn boton_rapido">RAPIDO</button></a> <br>
  </div>
  <span> Velocidad Actual: __BOTON_TEXTO__ </span>
</body>
</html>
)rawliteral";

void setup() {
  Serial.begin(115200);

  // conexion al wifi como antes
  Serial.print("Conectando a ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  int timeout = 20; // 20 intentos de 500ms = 10 segundos
  while (WiFi.status() != WL_CONNECTED && timeout > 0) {
    delay(500);
    Serial.print(".");
    timeout--;
  }

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("\nFallo la conexion. Reiniciando...");
    delay(1000);
    ESP.restart();
  }
  Serial.println("\nWiFi conectado!");
  Serial.print("Dirección IP: http://");
  Serial.println(WiFi.localIP());

  server.on("/", []() {
    
    String pagina = pagina_template; //hago una variable para no modificar a la original
    
    
    //pagina.replace busca el texto y lo reemplaza con lo que pongas. 
    pagina.replace("__BOTON_TEXTO__", textoBoton);

    server.send(200, "text/html", pagina);
  });

  server.on("/apagado" ,[](){
    textoBoton = "apagado";
    server.sendHeader("Location", "/");
    server.send(302, "text/plain", "");
  });
  server.on("/lento" ,[](){
    textoBoton = "lento";
    server.sendHeader("Location", "/");
    server.send(302, "text/plain", "");
  });
  server.on("/medio" ,[](){
    textoBoton = "medio";
    server.sendHeader("Location", "/");
    server.send(302, "text/plain", "");
  });
  server.on("/rapido" ,[](){
    textoBoton = "rapido";
    server.sendHeader("Location", "/");
    server.send(302, "text/plain", "");
  });

  /* OTRA FORMA 
  server.on("/toggle", []() {
    digitalWrite(ledPin,ledEncendido);
    ledEncendido = !ledEncendido;
    server.sendHeader("Location", "/");
    server.send(302, "text/plain", ""); // 302 y send Header parFa que no me quede en /header
  });*/
  server.begin();
}

void loop() {
  server.handleClient();
}