#include "DHT.h"

#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#define DHTPIN 2 // D4
#define PIRPIN 14 // D5
#define LDRPIN A0
#define LIGHTRELAY 12 // D6
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
Adafruit_SH1106G display(128, 64, &Wire, -1);

float temperature = 0.0;
float humidity = 0.0;
int lightLevel = 0;
bool motionDetected = false;

void setup() {
  // put your setup code here, to run once:
  pinMode(PIRPIN, INPUT);

  dht.begin();

  display.begin(0x3c, true);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(10, 25);
  display.println("Initializing...");
  display.display();
  delay(500);

  Serial.begin(9600);

  display.clearDisplay();
  display.setCursor(35, 25);
  display.println("Ready");
  display.display();
  delay(500);
}

void loop() {
  // put your main code here, to run repeatedly:
  readSensors();
  updateDisplay();
  delay(1500);
}

void readSensors() {
  motionDetected = digitalRead(PIRPIN);
  lightLevel = analogRead(LDRPIN);
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();
}

void updateDisplay() {
  display.clearDisplay();
  display.setCursor(0, 6);

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    display.println("DHT read error!");
  } else {
    display.printf("Humidity: %.1f %%\n\n", humidity);
    display.printf("Temperature: %.1f C\n\n", temperature);
  }

  display.printf("Light: %d\n\n", lightLevel);
  display.printf("Motion detected: %s\n\n", motionDetected ? "Yes" : "No");

  display.display();
}