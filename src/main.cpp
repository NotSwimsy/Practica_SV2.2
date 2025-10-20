#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

const char* ssid = "CESJT";
const char* password = "itisjtsmg";
int est;
const int ldr = 25;

// Configuro sv asincrono
AsyncWebServer server(80);

// No cambia la página
const char pagina_template[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang ="es">
<head>
    <meta charset="UTF-8">
    <title>Monitor de Luz Ambiental</title>
    <style>
        body {
            background-color: COLOR; 
            color: white;
        }
        .titulo{
            font-family: 'Segoe UI';
            text-align: center;
        }
        .readout{
            border: 3px dotted white;
            border-radius: 20px;
            padding: 25px;
            font-size: 40px;
        }
    </style>
    <body>
        <h1 class = "titulo">Monitor de Luz Ambiental</h1>
        <div class = "readout">
            <span>Nivel de Luz: <b>XXX%</b></span>
        </div>
    </body>
</head>
</html>
)rawliteral";


// Funciones dedicadas para cada ruta (/...)

// Función para manejar la petición a la página principal (/)
void handleRootRequest(AsyncWebServerRequest *request) {
    String pagina = pagina_template; //hago una variable para no modificar a la original
    //parte para cambiar lo que dice el boton una vez que se prende o apaga
    String textoBoton;
    switch(est){
      case 1:
        textoBoton = "Apagado";
        analogWrite(motorPin, 0);
        break;
        case 2:
        textoBoton = "Baja";
        analogWrite(motorPin, 85);
        break;
        case 3:
        textoBoton = "Media";
        analogWrite(motorPin, 170);
        break;
        case 4:
        textoBoton = "Alta";
        analogWrite(motorPin, 170);
        break;
    }  
    
    //pagina.replace busca el texto y lo reemplaza con lo que pongas. 
    pagina.replace("XXXX", textoBoton);
    
    request->send(200, "text/html", pagina);
}

void apagado(AsyncWebServerRequest *request){
  est = 1;
  request->redirect("/");
}
void baja(AsyncWebServerRequest *request){
  est = 2;
  request->redirect("/");
}
void media(AsyncWebServerRequest *request){
  est = 3;
  request->redirect("/");
}
void alta(AsyncWebServerRequest *request){
  est = 4;
  request->redirect("/");
}

void setup() {
  Serial.begin(115200);
  Serial.print("Conectando a ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  // Conexion wifi como antes
  int timeout = 20; 
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

  // defino las rutas para las request 
  // Ahora ponemos el nombre de nuestra función dedicada
  server.on("/", HTTP_GET, handleRootRequest);
  server.on("/off", HTTP_GET, apagado);
  server.on("/low", HTTP_GET, baja);
  server.on("/medium", HTTP_GET, media);
  server.on("/high", HTTP_GET, alta);

  // Iniciar servidor
  server.begin();
}

void loop() {
  // El loop sigue libre para las tareas de hardware

}