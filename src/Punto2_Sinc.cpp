#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "CESJT";
const char* password = "itisjtsmg";

WebServer server(80);
String textoLuz="XXX%";

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
      background-color: __LUZ_FONDO_; 
      text-align: center; 
    }
    .titulo { 
      font-family: 'Segoe UI';
    }
    .lectura{
      display:block;
      border: 3px dotted white;
      padding: 25px;
      border-radius: 20px;
}
  </style>
  <h1 class='titulo'>Monitor de luz ambiental</h1>
  <div class="lectura">
    <span> Nivel de luz: <b>__LUZ_FONDO__ </b> </span>
  </div>
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
    pagina.replace("__LUZ_FONDO__", textoLuz);

    server.send(200, "text/html", pagina);
  });

  server.on("/dia" ,[](){
    textoLuz = "apagado";
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