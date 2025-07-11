#include <dht11.h>

#define DHTPIN 4

dht11 DHT11;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  Serial.println("Initializing...");
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println();

  int check = DHT11.read(DHTPIN);

  Serial.print("Humidity(%): ");
  Serial.println((float)DHT11.humidity);
  Serial.print("Temp(C): ");
  Serial.println((float)DHT11.temperature);

  delay(2500);
}
