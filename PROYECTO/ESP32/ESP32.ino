// Librería para ESP32 WiFi 
#include "WiFi.h"

// Nombre de la red WiFi
const char* ssid = "ESP32";
// Contraseña
const char* password = "ESP12345678";

//numero de puerto 
WiFiServer server(80);

//almacen de http 
String header;

//estados de salida
String state = "off";

// Tiempo actual
unsigned long currentTime = millis();
// Tiempo anterior 
unsigned long previousTime = 0;
// Define el tiempo de espera en milisegundos (ejemplo: 2000ms = 2s)
const long timeoutTime = 2000;

// Variable para almacenar los datos del Arduino
String cad = "";

void setup() {
  Serial.begin(115200);

  // conecta a la red Wi-Fi con SSID y contraseña
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // imprime la dirección IP local e inicie el servidor web
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop() {
  WiFiClient client = server.available();   // Escucha los clientes entrantes

  if (client) {                             // Si un nuevo cliente se conecta
    currentTime = millis();
    previousTime = currentTime;
    String currentLine = "";                // hacer una cadena para contener los datos entrantes del cliente
    while (client.connected() && currentTime - previousTime <= timeoutTime) {  // Ciclo mientras el cliente está conectado
      currentTime = millis();
      if (client.available()) {             // Si hay bytes para leer del cliente,
        char c = client.read();             // lee un byte, entonces
        header += c;
        if (c == '\n') {                    // si el byte es un carácter de nueva línea
          // si la línea actual está en blanco, tienes dos caracteres de nueva línea seguidos. 
          // ese es el final de la solicitud HTTP del cliente, para enviar una respuesta:
          if (currentLine.length() == 0) {
            // Los encabezados HTTP siempre comienzan con un código de respuesta (por ejemplo, HTTP/1.1 200 OK)
             // y un tipo de contenido para que el cliente sepa lo que viene, luego una línea en blanco:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            // encender y apagar el estado 
            if (header.indexOf("GET /on") >= 0) {
               state = "on";
            }
            // Muestra la página web HTML
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS para el diseño de la pagina 
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".temp { font-size: 24px; color: #444 }</style></head>");

             // Encabezado de la página web
            client.println("<body><h1><marquee>Inno Eyes</marquee></h1>");

            // Si la cadena no esta vacia , lo escribe en la pagina. 
            if(cad != " "){
               client.println("<p>" + cad + "<br></p>");
            }
            client.println("</body></html>");

           // La respuesta HTTP termina con otra línea en blanco
            client.println();
            // Salir del ciclo while
            break;
          } else { // si tiene una nueva línea, borra la línea actual
            currentLine = "";
          }
        } else if (c != '\r') {  // si obtuvo algo más que un carácter de retorno de carro,
          currentLine += c;      // agregarlo al final de currentLine
        }
      }
    }
    // Borrar la variable de encabezado
    header = "";
    // Cierra la conexión
    client.stop();
  }
  delay(1000); // Espera 1 segundo
} 
