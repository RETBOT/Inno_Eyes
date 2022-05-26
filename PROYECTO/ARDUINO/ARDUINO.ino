// Librería para el funcionamiento del sensor ultrasónico
#include "SR04.h"
// Librería para el funcionamiento del sensor de temperatura y humedad
#include <DHT.h>

// Definimos los pines del ultrasonido (el 12 para el TRIG y el 11 para el ECHO)
#define TRIG_PIN 12
#define ECHO_PIN 11

// Se crea un objeto, indicando los pines TRIG y ECHO que se usaran en el sensor ultrasónico,
// para poder hacer el cálculo de la distancia
SR04 sr04 = SR04(ECHO_PIN, TRIG_PIN);

// Almacena la distancia del sensor de ultrasonido
long a;

// Definimos el pin digital donde se conecta el sensor de temperatura y humedad
#define DHTPIN 6
// E indicamos cuál sensor de temperatura y humedad usaremos
#define DHTTYPE DHT11
// Inicializamos el sensor DHT11
DHT dht(DHTPIN, DHTTYPE);

// Variable para la recepción de los datos del Bluetooth
char blueToothVal = ' ';

// Definimo el pin del buzzer
int pinBuzzer = 8;
// Bandera para activar el buzzer
bool alarma = false;

// Definimos el pin del  PIR sensor
int PIRPin = 2;
// Estado del sensor pir sin actividad
int pirState = LOW;

//Definimo el pin del boton
int botonPin = 10;
// Estado del sensor boton sin actividad
int presionado = 0;

void setup() {
  // Inicialización de la comunicación serial
  Serial.begin(115200);
  // Imprime un mensaje en consola
  Serial.println("Arduino Mega");
  // Indicamos que el pin del buzzer actuara como salida
  pinMode(pinBuzzer, OUTPUT);
  // Indicamos que el pin del pir actuara entrada
  pinMode(PIRPin, INPUT);
  // Indicamos que el pin del boton como entrada
  pinMode(botonPin, INPUT);

  // Inicializamos el sensor DHT
  dht.begin();

  while (!Serial)
  {
    ; //espere a que se conecte el puerto serie. Necesario solo para puerto USB nativo
  }

  while (Serial.read() >= 0) {} // Vaciar el caché del puerto serie
  Serial1.begin(115200);
}

void loop() {
  // Determine si el puerto serie 1 recibe datos (aquí el puerto serie 1 se refiere a TX1 y RX1)
  if (Serial1.available() > 0)
  {
    // Almacena el valor enviado al puerto serial 1
    blueToothVal = Serial1.read();
  }
  // Determine si el puerto serie 0 ha recibido datos
  if (Serial.available() > 0)
  {
    // Enviar los datos leídos por el puerto serie 0 al puerto serie 1
    Serial1.write(Serial.read());
  }

  // si el valor dado por la aplicacion es un char de 'a' o un char de 'c',
  // se indicara que es la temperatura y la humedad
  // Si el blueToothVal es diferente al char de 'a' o a un char de 'c'
  //  solo dara mediciones (pueden ser objetos o personas)

  // Pero si el valor ingresado de la aplicacion movil, es un char de 'a' o un char de 'c'
  // 'a' = para el uso del sensor de temperatura y humedad
  // 'c' = para la alarma

  if (blueToothVal != 'a' || blueToothVal != 'c')
  {
    // en caso de no tener ningun valor dado por la aplicacion, entra en el else
    // Calcula la distancia con el  ultrasónico
    a = sr04.Distance();

    // Si el sensor PIR está activo o la distancia del ultrasónico es menor a 70 entra en el if
    if (digitalRead(PIRPin) == HIGH || a <  70)
    { // Si el estado del sensor PIR está previamente apagado o la distancia del ultrasónico es menor a 70 entra en el if
      if (!pirState == HIGH || a <  70)
      { // Se indica que se encuentra un objeto y la manda a la aplicación
        if (a < 70)
        {
          // Se muestra en consola el texto de distancia
          Serial.println("Distancia");
          // Más la distancia
          Serial.print(a);
          // junto con el texto de "cm"
          Serial.println("cm");

          // Se muestra en la aplicación el texto de distancia
          Serial1.print("Objeto a = ");
          Serial1.print(a);
          Serial1.println(" cm");

          // Manda un tono, para verificar
          tone(pinBuzzer, 600);
          // Tiempo que mantiene el tono
          delay(100);
        }
        // Si el estado esta en falso, se cambia a verdadero y entra en el if
        if(digitalRead(PIRPin) == HIGH)
        {
          // Manda un tono, para verificar
          tone(pinBuzzer, 600);
          // Tiempo que mantiene el tono 0.1 seg
          delay(100);
          // se muestra en la aplicación movil
          Serial1.println("Movimiento Detectado");
          // y en consola
          Serial.println("Movimiento Detectado");
        }
      }
      // Pone el estado del pir en HIGH
      pirState = HIGH;
    }
    // Si el PIR esta desactivado
    else
    { //si el estado del PIR estaba encendido
      if (pirState == HIGH)
      {
        // Manda un mensaje en consola indicando que el sensor PIR está desactivado
        Serial.println("Sensor detenido");
        // Cambia el estado del PIR
        pirState = LOW;
      }
    }
  }

  if (blueToothVal == 'a' || blueToothVal == 'c')
  {
    // Si el valor recibido de la aplicación es un char 'a' activa el sensor de temperatura y humedad
    if (blueToothVal == 'a') {
      // cambia el valor dado del bluetooth
      blueToothVal = ' ';
      // Leemos la humedad relativa
      float h = dht.readHumidity();
      // Leemos la temperatura en grados centígrados (por defecto)
      float t = dht.readTemperature();

      // Comprobamos si ha habido algún error en la lectura
      if (isnan(h) || isnan(t)) {
        Serial.println("Error obteniendo los datos del sensor DHT11");
        return;
      }
      // Mandamos a la aplicación movil la humedad y la temperatura
      Serial1.print("Humedad: ");
      Serial1.print(h);
      Serial1.println(" % ");
      Serial1.print("Temperatura: ");
      Serial1.print(t);
      Serial1.println(" *C ");
      delay(100);
    }
    // Si el valor recibido es un char 'c'
    else if (blueToothVal == 'c') {
      // cambia el valor dado del bluetooth
      blueToothVal = ' ';
      // Cambia el estado de la alarma (de verdadero a falso o de falso a verdadero)
      alarma = !alarma;
      delay(500);
    }
  }

  // Comprueba si el botón está pulsado
  if (digitalRead(botonPin) == LOW)
  {
    // Cambia el valor del estado del botón en 1, indicando que está pulsado
    presionado = 1;
  }
  // // Comprueba si el botón está pulsado y la variable de pulsado se encuentra en uno
  if(digitalRead(botonPin) == HIGH && presionado == 1)
  {
   if(alarma == false) {
      // se indica en la aplicacion que se activo la alarma
      Serial1.println("b");
      delay(100);
    }
    // Indica que el botón no está pulsado
    presionado = 0;
    // Cambia el estado de la alarma
    alarma = !alarma;
    // Espera 0.1 segundos
    delay(100);
  }

  // Si la alarma está activa, activa los tonos para la señal
  if (alarma)
  {
    // tone (PIN DEL ZUMBADOR, FREQÜÉNCIA);
    tone(pinBuzzer, 600);
    // Tiempo que maniente el tono
    delay(100);
    tone(pinBuzzer, 294);
    delay(100);
    tone(pinBuzzer, 494);
    delay(100);
  }
  else if (!alarma) // Si la alarma esta en falso,
  {
    // Quitamos los tonos
    noTone(pinBuzzer);
  }
  // Espera 0.1 segundos
  delay(100);
}
