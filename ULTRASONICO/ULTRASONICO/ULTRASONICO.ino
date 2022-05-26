// Librería sensor ultrasónico 
#include "SR04.h"

// Definimos los pines del ultrasonido (TRIG y ECHO)
#define TRIG_PIN 12
#define ECHO_PIN 11

// Se crea un objeto, indicando los pines TRIG y ECHO, para poder hacer el cálculo de la distancia
SR04 sr04 = SR04(ECHO_PIN, TRIG_PIN);

// Almacena la distancia del sensor de ultrasonido
long a;

void setup() {
  // Inicialización de la comunicación serial
  Serial.begin(9600);

}

void loop() {
  // Calcula la distancia con el  ultrasónico
  a = sr04.Distance(); 
  // Si la distancia del ultrasónico es menor a 70 entra en el if
  if (a <  70)  
  {
    // Se muestra en consola el texto de distancia
      Serial.println("Distancia"); 
      // Más la distancia
      Serial.print(a); 
      // junto con el texto de "cm"
      Serial.println("cm"); 
  }
}
