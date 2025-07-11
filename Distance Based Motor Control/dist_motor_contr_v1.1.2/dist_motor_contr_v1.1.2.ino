#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
// SCL - A5, SDA - A4

const int trigPin = 9;
const int echoPin = 10;
const int in1 = 5;
const int in2 = 6;
const int ena = 3;

long duration;
// float distance;
int distance; // use int distance if decimal accuracy is not required

int zone;

void setup() {
  // put your setup code here, to run once:
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(ena, OUTPUT);

  Serial.begin(9600);

  display.begin(0x3c, true);

  delay(2000);

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0, 10);
  display.println("Initializing...");
  display.display();

  runMotor(255, true);
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(5, 2);
  display.println("Motor Speed: 100%");
  display.display();
  delay(3000);

  runMotor(128, true);
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(5, 2);
  display.println("Motor Speed: 50%");
  display.display();
  delay(3000);

  runMotor(0, true);
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(5, 2);
  display.println("Motor Speed: 0%");
  display.display();
  delay(3000);

  // delay(2500);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(trigPin, LOW);
  delay(2);
  digitalWrite(trigPin, HIGH);
  delay(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.0343/2; //0.0343 = speed of sound in air in cm/microsec

  if (distance >= 25) {
    zone = 0;
  } 
  else if (distance >= 15 && distance < 25) {
    zone = 1;
  }
  else if (distance >= 10 && distance < 15) {
    zone = 2;
  } else {
    zone = 3;
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(10, 15);
  display.print("Distance: ");
  display.print(String(distance));
  display.println(" cm");
  display.display();

  display.setTextSize(1);
  display.setCursor(10, 30);

  switch (zone) {
    case 0:
      runMotor(180, true);
      display.println(String("Speed: ") + String(180));
      display.setTextSize(2);
      display.println("\nSAFE");
      break;

    case 1:
      runMotor(150, true);
      display.println(String("Speed: ") + String(150));
      display.setTextSize(2);
      display.println("\nWARNING");
      break;

    case 2:
      runMotor(128, true);
      display.println(String("Speed: ") + String(128));
      display.setTextSize(2);
      display.println("\nDANGER");
      break;
      
    case 3:
      runMotor(180, false);
      display.println(String("Speed: ") + String(180));
      display.setTextSize(2);
      display.println("\nREVERSE");
      break;
  }
  display.display();
}

void runMotor (int speed, bool forward) {
  if (forward) {
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
  } else {
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
  }

  analogWrite(ena, speed);
}