#include <WiFi.h>

void setup() {
  Serial.begin(115200);
  Serial.println();
  
  // Obtindre l'adreça MAC
  String macAddress = WiFi.macAddress();
  Serial.println("L'adreça MAC del ESP32 és: " + macAddress);
}

void loop() {
  // No fa res
}
