#define BLYNK_TEMPLATE_ID "TMPL3AOnAQLn7"
#define BLYNK_TEMPLATE_NAME "Smart Room Monitor"
#define BLYNK_AUTH_TOKEN "4YR5uIrgqe1N3RvhJU_EM4dtqNILq0v-"

#include "DHT.h"
#include <BlynkSimpleEsp8266.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#define DHTPIN 2 // D4
#define PIRPIN 14 // D5
#define LDRPIN A0
#define LIGHTRELAY 12 // D6
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
Adafruit_SH1106G display(128, 64, &Wire, -1);

char ssid[] = "TP-Link_9E82";
char pass[] = "85011984";

float temperature = 0.0;
float humidity = 0.0;
int lightLevel = 0;
bool motionDetected = false;
bool lastMotionState = false;
unsigned long lastMotionAlert = 0;

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
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  display.clearDisplay();
  display.setCursor(35, 25);
  display.println("Ready");
  display.display();
  delay(500);

  // Blynk.logEvent("motion_alert", "Manual test");
}

void loop() {
  // put your main code here, to run repeatedly:
  Blynk.run();
  readSensors();
  updateDisplay();
  sendToBlynk();
  sendMotionAlert();
  delay(1500);
}

void readSensors() {
  lastMotionState = motionDetected;
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

void sendToBlynk() {
  if (!isnan(humidity) && !isnan(temperature)) {
    Blynk.virtualWrite(V0, humidity);
    Blynk.virtualWrite(V1, temperature);
  }
  Blynk.virtualWrite(V2, lightLevel);
  Blynk.virtualWrite(V3, motionDetected ? "Yes" : "No");
}

void sendMotionAlert() {
  if (!lastMotionState && motionDetected) {
    if (millis() - lastMotionAlert > 1000) {
      Blynk.logEvent("motion_alert", "Motion Detected!");
      lastMotionAlert = millis();
    }
  }
}