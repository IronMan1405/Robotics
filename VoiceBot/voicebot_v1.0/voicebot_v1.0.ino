#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

Adafruit_SH1106G display(128, 64, &Wire, -1);

#define LED 14 // D5
#define BUZZER 12 // D6
#define M1 13 // D7
#define M2 15 // D8

const char* ssid = "TP-Link_9E82";
const char* pass = "85011984";

ESP8266WebServer server(80);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected"); 
  server.begin();  // Starts the Server
  Serial.println("Server started");

  Serial.print("IP Address of network: ");
  Serial.println(WiFi.localIP());
  Serial.print("Copy and paste the following URL: http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");

  pinMode(LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(M1, OUTPUT);
  pinMode(M2, OUTPUT);

  display.begin(0x3c, true);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(10, 25);
  display.println("Initializing...");
  display.display();
  delay(500);

  display.clearDisplay();
  display.setCursor(35, 25);
  display.println("Ready");
  display.display();
  delay(500);

  display.clearDisplay();
  display.setCursor(5, 25);
  display.println("Checking attached components");
  display.display();
  delay(500);

  server.on("/led_on", []() {
    digitalWrite(LED, HIGH);
    displayMessage("LED ON");
    server.send(200, "text/plain", "LED ON");
  });

  server.on("/led_off", []() {
    digitalWrite(LED, LOW);
    displayMessage("LED OFF");
    server.send(200, "text/plain", "LED OFF");
  });

  server.on("/buzzer_on", []() {
    digitalWrite(BUZZER, HIGH);
    displayMessage("Buzzer ON");
    server.send(200, "text/plain", "Buzzer ON");
  });

  server.on("/buzzer_off", []() {
    digitalWrite(BUZZER, LOW);
    displayMessage("Buzzer OFF");
    server.send(200, "text/plain", "Buzzer OFF");
  });

  server.on("/motor_forward", []() {
    digitalWrite(M1, HIGH);
    digitalWrite(M2, LOW);
    displayMessage("Motor FORWARD");
    server.send(200, "text/plain", "Motor Running");
  });

  server.on("/motor_stop", []() {
    digitalWrite(M1, LOW);
    digitalWrite(M2, LOW);
    displayMessage("Motor STOPPED");
    server.send(200, "text/plain", "Motor Stopped");
  });

  server.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  server.handleClient();
}

void displayMessage(String msg) {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println(msg);
  display.display();
}