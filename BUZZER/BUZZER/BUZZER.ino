// Definimo el pin del buzzer
int pinBuzzer = 8; 
// Bandera para activar el buzzer
bool alarma = false; 

void setup() {
  // Inicialización de la comunicación serial
  Serial.begin(9600);
  
  // Indicamos que el pin del buzzer actuara como salida
  pinMode(pinBuzzer, OUTPUT); 
}

void loop() {
  // cambia de estado la alarma 
  alarma = !alarma;
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
