// Librería para ESP32 WiFi
#include "WiFi.h"

// Librería para la comunicación del teléfono al esp32
#include "BluetoothSerial.h"

// verifica que la conexión Bluetooth sea correcta
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

// Definimos la comunicación serial del teléfono al esp32
BluetoothSerial SerialBT;

// Nombre de la red WiFi
const char* ssid = "Megacable_2.4G_E42A";
// Contraseña
const char* password = "QfnaSaUj";

//numero de puerto
WiFiServer server(80);

//almacen de http
String header;

// cadena ingresada
String cadena = " ";
// cadena auxiliar donde se almacena la cadena 
String aux = "";
//estados de salida
String outputState = "off"; // Temperatura y humedad 
String outputState2 = "off"; // alarma 


// Tiempo actual
unsigned long currentTime = millis();
// Tiempo anterior
unsigned long previousTime = 0;
// Define el tiempo de espera en milisegundos (ejemplo: 2000ms = 2s)
const long timeoutTime = 2000;

void setup() {
  Serial.begin(115200);
  // SerialBT.begin("ESP32"); //Nombre del Bluetooth // descomentar si solo usan bluetooth
  // conecta a la red Wi-Fi con SSID y contraseña
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  // imprime la dirección IP local e inicie el servidor web
  Serial.println("");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop() {

  while (Serial.available()) { // Espera a que se envíen datos al puerto serial 0 (TX0 y RX0)
    cadena += (char)Serial.read();
    //SerialBT.write((char)Serial.read()); // Lo manda por Bluetooth al teléfono
  }

 
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

            // Manda el mensaje de temperatura y humedad y la alarma
            if (header.indexOf("GET /TH/on") >= 0) {
              outputState = "on";
            } else if (header.indexOf("GET /TH/off") >= 0) {
              outputState = "off";
            } else if (header.indexOf("GET /AL/on") >= 0) {
              outputState2 = "on";
            } else if (header.indexOf("GET /AL/off") >= 0) {
              outputState2 = "off";
            }
            
            // Muestra la página web HTML
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS para diseñar los botones de encendido/apagado
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}</style></head>");

            // Encabezado de la página web
            client.println("<body><h1>Inno Eyes</h1>"); //<marquee>
            // Si outputState está apagado, muestra el botón ON
            if (outputState == "off") {
              client.println("<p><a href=\"/TH/on\"><button class=\"button\">Temperatura y Humedad</button></a></p>");          
            } else {
              client.println("<p><a href=\"/TH/off\"><button class=\"button button2\">OFF</button></a></p>");
              Serial.println("a");
              outputState = "off";
            }
            if (outputState2 == "off") {
              client.println("<p><a href=\"/AL/on\"><button class=\"button\">Alarma</button></a></p>");          
            } else {
              client.println("<p><a href=\"/AL/off\"><button class=\"button button2\">OFF</button></a></p>");
              Serial.println("c");
              outputState = "off";
            }
            client.println("<p> DATOS: <br></p>");  
            
            // Muestra el estado actual
            if (cadena != " ") {
              aux += cadena + "<br>";
              cadena = " ";
            }
            client.println("<p> " + aux + "</p>");     
            client.println("<meta http-equiv=refresh content=2>");
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
}
