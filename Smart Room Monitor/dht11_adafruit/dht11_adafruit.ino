#include "DHT.h"

#define DHTPIN 4
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("DHT11 Sensor Test!");
  dht.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(2000);

  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.println("Humidity: " + String(humidity) + "%");
  Serial.println("Temperature: " + String(temperature) + "°C");

  // for serial plotter
  // Serial.print(humidity);
  // Serial.print(",");
  // Serial.println(temperature);
}
