#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SH1106G display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define incrButton 0 //D3
unsigned long lastPressIncr = 0;

#define decrButton 10 //SD3
unsigned long lastPressDecr = 0;

const int debounceDelay = 50; //in millis

#define startButton 2 //D4
unsigned long lastPressStart = 0;


bool timerRunning = false;
bool timerStarted = false;
bool timerEnded = false;
unsigned long startTime;
unsigned long countdownTime = 0; // in milliseconds



void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  pinMode(incrButton, INPUT_PULLUP);
  pinMode(decrButton, INPUT_PULLUP);
  pinMode(startButton, INPUT_PULLUP);

  display.begin(0x3c, true);

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(5, 2);
  display.println("Initializing...");
  display.display();
  delay(500);

  startTime = millis();

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(5, 2);
  display.println("Ready");
  display.display();
  delay(500);

}

void loop() {
  // put your main code here, to run repeatedly:
  if (!timerStarted && !timerEnded) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0, 10);
    display.printf("Set Timer: \n\n%lu s", countdownTime / 1000);
    display.setTextSize(1);
    display.setCursor(0, 40);
    display.println("Press Start!");
    display.display();
  }
  
  if (digitalRead(incrButton) == LOW) {
    if (millis() - lastPressIncr > debounceDelay) {
      countdownTime += 5000; 
      lastPressIncr = millis();
    }
  }

  if (digitalRead(decrButton) == LOW) {
    if (millis() - lastPressDecr > debounceDelay) {
      countdownTime -= 5000; 
      lastPressDecr = millis();
    }
  }

  if (digitalRead(startButton) == LOW) {
    if (millis() - lastPressStart > debounceDelay && !timerRunning) {
      timerRunning = true;
      timerStarted = true;
      timerEnded = false;
      
      startTime = millis();
      lastPressStart = millis();
    }
  }

  if (timerRunning) {
    unsigned long elapsed = millis() - startTime; // elapsed is in millis
    unsigned long remaining = (countdownTime > elapsed) ? countdownTime - elapsed : 0;

    display.clearDisplay();
    display.setCursor(0, 10);
    display.setTextSize(2);

    if (remaining > 0) {
      int totalSeconds = remaining / 1000;
      int mins = totalSeconds / 60;
      int secs = totalSeconds % 60;

      display.printf("%02d : %02d", mins, secs);

    } else {
      display.println("Time up");
      timerRunning = false;
      timerEnded = true;
    }

    display.display();
    delay(200);
    
  }
}
