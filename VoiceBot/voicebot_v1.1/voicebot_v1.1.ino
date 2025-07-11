#define BLYNK_TEMPLATE_ID "TMPL3WepYwThI"
#define BLYNK_TEMPLATE_NAME "VoiceBot"
#define BLYNK_AUTH_TOKEN "O90fVVKBA_2BlzzOQsZW6YXWQ6mL8pds"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char ssid[] = "TP-Link_9E82";
char pass[] = "85011984";


#define LED_PIN 14
#define BUZZER_PIN 12
#define MOTOR_PIN 13

BLYNK_WRITE(V0) {
  digitalWrite(LED_PIN, param.asInt());
}

BLYNK_WRITE(V1) {
  digitalWrite(BUZZER_PIN, param.asInt());
}

BLYNK_WRITE(V2) {
  digitalWrite(MOTOR_PIN, param.asInt());
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(MOTOR_PIN, OUTPUT);
  Serial.begin(115200);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

void loop() {
  Blynk.run();
}
