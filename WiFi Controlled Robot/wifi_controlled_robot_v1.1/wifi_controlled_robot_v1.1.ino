#include <ESP8266WiFi.h>

const char* ssid = "TP-Link_9E82";
const char* password = "85011984";

WiFiServer server(80);

const int motorPins[4] = {16, 5, 14, 12}; // order - in1, in2, in3, in4
// in1, in2 = rm1, rm2 ; in3, in4 - lm1, lm2
const int ena = 4;
const int enb = 13; // ena - rightmotor, enb - leftmotor
// 16 - D0, 5 - D1, 14 - D5, 12 - D6, 4 - D2, 13 - D7

int speed = 650;

void setup() {
  // put your setup code here, to run once:
  for (int i = 0; i < 4; i++) {
    pinMode(motorPins[i], OUTPUT);
  }
  pinMode(ena, OUTPUT);
  pinMode(enb, OUTPUT);

  Serial.begin(115200);

  Serial.println("Connecting to Network");
  WiFi.begin(ssid, password);

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
}

void loop() {
  // put your main code here, to run repeatedly:
  WiFiClient client = server.available();

  if (!client) {
    return;
  }

  Serial.println("Waiting for new client");

  while(!client.available()) {
    delay(1);
  }

  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();

  client.println("HTTP/1.1 200 OK"); //
  client.println("Content-Type: text/html");
  client.println("");
  client.println("<!doctype html>");
  client.println("<html>");
  client.println("<head>");
  client.println("<title>WiFi Controlled Robot</title>");
  client.println("</head>");
  client.println("<body style='display: flex;justify-content: center;align-items: center;height: 100vh;flex-direction: column;font-family: sans-serif;'>");
  client.println("<h1>Esp8266 Robot Control</h1>");
  client.println("<div style='display: grid;grid-template-columns: repeat(3, 1fr);grid-gap: 10px;margin-top: 20px;'>");
  client.println("<div></div>");
  client.println("<form action='/forward' method='get'>");
  client.println("<button style='width: 100px; height: 60px'>Forward</button>");
  client.println("</form>");
  client.println("<div></div>");
  client.println("<form action='/left' method='get'>");
  client.println("<button style='width: 100px; height: 60px'>Left</button>");
  client.println("</form>");
  client.println("<form action='/stop' method='get'>");
  client.println("<button style='width: 100px; height: 60px'>Stop</button>");
  client.println("</form>");
  client.println("<form action='/right' method='get'>");
  client.println("<button style='width: 100px; height: 60px'>Right</button>");
  client.println("</form>");
  client.println("<div></div>");
  client.println("<form action='/reverse' method='get'>");
  client.println("<button style='width: 100px; height: 60px'>Reverse</button>");
  client.println("</form>");
  client.println("<div></div>");
  client.println("<div class='slidecontainer'>");
  client.println("<p>Speed: <span id='speedValue'>650</span></p>");
  client.println("<input type='range' min='0' max='1023' value='650' class='slider' id='myRange' oninput='updateSpeed(this.value)'/>");
  client.println("</div>");
  client.println("</div>");
  client.println("<script>function updateSpeed(val) {document.getElementById('speedValue').innerText = val; fetch(`/speed?val=${val}`);}</script>");
  client.println("</body>");
  client.println("</html>");

  if (request.indexOf("/forward") != -1) {
    runMotor(true, speed);
  }
  else if (request.indexOf("/reverse") != -1) {
    runMotor(false, speed);
  }
  else if (request.indexOf("/stop") != -1) {
    stopMotor();
  }
  else if (request.indexOf("/left") != -1) {
    turnMotor(true, speed);
  }
  else if (request.indexOf("/right") != -1) {
    turnMotor(false, speed);
  }
  else if (request.indexOf("speed?val=") != -1) {
    int valIndex = request.indexOf("=") + 1;
    speed = constrain(request.substring(valIndex).toInt(), 0, 1023);
  }


  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");
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

void turnMotor(bool left, int speed) {
  if (left) {
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

void stopMotor() {
  digitalWrite(motorPins[0], LOW);
  digitalWrite(motorPins[1], LOW);

  digitalWrite(motorPins[2], LOW);
  digitalWrite(motorPins[3], LOW);
}