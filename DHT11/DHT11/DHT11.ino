// Librería sensor de temperatura y humedad 
#include <DHT.h>
// Definimos el pin digital donde se conecta el sensor de temperatura y humedad 
#define DHTPIN 6
// E indicamos cuál sensor de temperatura y humedad usaremos 
#define DHTTYPE DHT11
// Inicializamos el sensor DHT11
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  // Inicialización de la comunicación serial
  Serial.begin(9600);

  // Inicializamos el sensor DHT
  dht.begin();
}

void loop() {
  // Leemos la humedad relativa
    float h = dht.readHumidity();
    // Leemos la temperatura en grados centígrados (por defecto)
    float t = dht.readTemperature();
    // Comprobamos si ha habido algún error en la lectura
    if (isnan(h) || isnan(t)) {
      Serial.println("Error obteniendo los datos del sensor DHT11");
      return;
    }
    // mandamos la humedad y la temperatura 
    Serial.print("Humedad: ");
    Serial.print(h);
    Serial.println(" % ");
    Serial.print("Temperatura: ");
    Serial.print(t);
    Serial.println(" *C ");
}
