//Se define el pin del boton
int botonPin = 10; 
// Estado del sensor boton sin actividad 
int presionado = 0;

void setup() {
  // Inicialización de la comunicación serial
  Serial.begin(9600);
  // Indicamos que el pin del boton como entrada
  pinMode(botonPin, INPUT); 

}

void loop() {
   // Comprueba si el botón está pulsado
  if (digitalRead(botonPin) == LOW) 
  {
    // Cambia el valor del estado del botón en 1, indicando que está pulsado
    presionado = 1; 
    // Y se muestra en consola
    Serial.println(presionado);
  }
  // // Comprueba si el botón está pulsado y la variable de pulsado se encuentra en uno 
  if (digitalRead(botonPin) == HIGH && presionado == 1)
  {
    // Indica que el botón no está pulsado
    presionado = 0; 
    // Y se muestra en consola
    Serial.println(presionado);

    // Espera 0.1 segundos
    delay(100); 
  }
}
