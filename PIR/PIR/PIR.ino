// Definimos el pin del  PIR sensor
int PIRPin = 2;    
// Estado del sensor pir sin actividad     
int pirState = LOW;        
// Estado del pin   
int val = 0;   

void setup() {
  // Inicialización de la comunicación serial
  Serial.begin(9600);
  
  // Indicamos que el pin del pir actuará entrada
  pinMode(PIRPin, INPUT); 
}

void loop() {
  // Valor de entrada del PIR // HIGH o LOW
  val = digitalRead(PIRPin);
// Si el sensor PIR está activo 
  if (val == HIGH)  
  {// Si el estado del sensor PIR está previamente apagado
    if (pirState == LOW)  
    {
      // Se muestra en consola 
      Serial.println("Sensor activo"); 
    }
    // Pone el estado del pir en HIGH
    pirState = HIGH; 
  }
  else   
  {//si el estado del PIR estaba encendido
    if (pirState == HIGH)  
    {
      // Manda un mensaje en consola indicando que el sensor PIR está desactivado
      Serial.println("Sensor detenido"); 
      // Cambia el estado del PIR  
      pirState = LOW;
    }
  }
}
