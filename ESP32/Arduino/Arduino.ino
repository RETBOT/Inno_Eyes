void setup() {
// Inicialización de la comunicación serial  
Serial.begin(115200);
// Imprime un mensaje en consola
Serial.println("Arduino Mega");
  while (!Serial)
   {
    ; // Espere a que se conecte el puerto serie. Necesario solo para puerto USB nativo
  }
 
while (Serial.read() >= 0) {} // Vaciar el caché del puerto serie
  Serial1.begin(115200);
}

void loop() {
    while(Serial1.available()) // Espera a que se envíen datos al puerto serial 1 (TX1 y RX1) 
    {
      Serial.write(Serial1.read()); // Escribe el mensaje leído del puerto serial 1 (TX1,RX1) al puerto serial 0 (TX0, RX0)
    }
    
    while(Serial.available ()) // Espera a que se envíen datos al puerto serial 0 (TX0 y RX0)
    {
      Serial1.write (Serial.read()); // Escribe el mensaje leído del puerto serial 0 (TX0,RX0) al puerto serial 1 (TX1, RX1)
    }
}
