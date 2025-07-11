const int motorPins[4] = {16, 5, 14, 12}; // order - in1, in2, in3, in4
// in1, in2 = rm1, rm2 ; in3, in4 - lm1, lm2
const int ena = 4;
const int enb = 13; // ena - rightmotor, enb - leftmotor
// 16 - D0, 5 - D1, 14 - D5, 12 - D6, 4 - D2, 13 - D7

void setup() {
  // put your setup code here, to run once:
  for (int i = 0; i < 4; i++) {
    pinMode(motorPins[i], OUTPUT);
  }
  pinMode(ena, OUTPUT);
  pinMode(enb, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  runMotor(true, 512);
  delay(5000);
  turnMotor(true, 512);
  delay(5000);
  runMotor(false, 512);
  delay(5000);
  turnMotor(false, 512);
  delay(5000);

}

void runMotor(bool forward, int speed) {
  if (forward) {
    digitalWrite(motorPins[0], HIGH);
    digitalWrite(motorPins[1], LOW);

    digitalWrite(motorPins[2], HIGH);
    digitalWrite(motorPins[3], LOW);
  } else {
    digitalWrite(motorPins[0], LOW);
    digitalWrite(motorPins[1], HIGH);

    digitalWrite(motorPins[2], LOW);
    digitalWrite(motorPins[3], HIGH);
  }

  analogWrite(ena, speed);
  analogWrite(enb, speed);
}

void turnMotor(bool right, int speed) {
  if (right) {
    digitalWrite(motorPins[0], HIGH);
    digitalWrite(motorPins[1], LOW);

    digitalWrite(motorPins[2], LOW);
    digitalWrite(motorPins[3], HIGH);
  } else {
    digitalWrite(motorPins[0], LOW);
    digitalWrite(motorPins[1], HIGH);

    digitalWrite(motorPins[2], HIGH);
    digitalWrite(motorPins[3], LOW);
  }

  analogWrite(ena, speed);
  analogWrite(enb, speed);
}