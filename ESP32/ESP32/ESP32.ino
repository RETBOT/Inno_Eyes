// Librería para la comunicación del teléfono al esp32
#include "BluetoothSerial.h"

// verifica que la conexión Bluetooth sea correcta 
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

// Definimos la comunicación serial del teléfono al esp32
BluetoothSerial SerialBT;

void setup() {
  Serial.begin(115200); // Inicialización de la comunicación serial
  Serial.println("ESP32");// Imprime un mensaje en consola
  SerialBT.begin("ESP32"); //Nombre del Bluetooth
}

void loop() {
  while(Serial.available()) { // Espera a que se envíen datos al puerto serial 0 (TX0 y RX0)   
     SerialBT.write((char)Serial.read()); // Lo manda por Bluetooth al teléfono
  }
  while(SerialBT.available()) {   // Espera a que se envíen datos al puerto serial Bluetooth (Datos enviados desde el teléfono)
      Serial.write((char)SerialBT.read()); // lo escribe en el monitor 
  }

  delay(1000); // Espera 1 segundo
}
