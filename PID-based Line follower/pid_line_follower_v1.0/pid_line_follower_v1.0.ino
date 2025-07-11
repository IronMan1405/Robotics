const int sensors[5] = {A0, A1, A2, A3, A4};
const int weights[5] = {-2, -1, 0, 1, 2}; //pos of sensors from the center

const int lm1 = 12, lm2 = 13, enb = 6; // lm1 - in3, lm2 - in4
const int rm1 = 8, rm2 = 9, ena = 5; // rm1 - in1, rm2 - in2

float lastpos = 0;
float err = 0, prevErr = 0, integral = 0;

float Kp = 20;
float Ki = 0;
float Kd = 8;

int baseSpeed = 150;

void setup() {
  // put your setup code here, to run once:
  for (int i = 0; i < 5; i++) {
    pinMode(sensors[i], INPUT);
  }

  pinMode(lm1, OUTPUT);
  pinMode(lm2, OUTPUT);
  pinMode(enb, OUTPUT);

  pinMode(rm1, OUTPUT);
  pinMode(rm2, OUTPUT);
  pinMode(ena, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  float pos = readLinePos();
  
  err = 0 - pos;

  float P = Kp * err;
  integral += err;
  float I = Ki * integral;
  float D = Kd * (err - prevErr);
  float output = P + I + D;
  prevErr = err;

  int leftSpeed = baseSpeed + output;
  int rightSpeed = baseSpeed - output;

  setMotorSpeeds(leftSpeed, rightSpeed);
}

float readLinePos() {
  int sum = 0;
  int totalActive = 0;

  for(int i = 0; i < 5; i++) {
    int val = digitalRead(sensors[i]);

    val = (val == LOW) ? 1 : 0;

    sum += val * weights[i];
    totalActive += val;
  }

  if (totalActive > 0) {
    lastpos = (float) sum / totalActive;
  }
  return lastpos;
}

void setMotorSpeeds(int leftSpeed, int rightSpeed) {
  digitalWrite(lm1, LOW);
  digitalWrite(lm2, HIGH);
  digitalWrite(rm1, LOW);
  digitalWrite(rm2, HIGH);

  analogWrite(ena, constrain(rightSpeed, 70, 255));
  analogWrite(enb, constrain(leftSpeed, 70, 255));
}