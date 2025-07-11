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

  if (request.indexOf("/up") != -1) {
    runMotor(true, speed);
  }
  else if (request.indexOf("/down") != -1) {
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
    Serial.print("Updated Speed to: ");
    Serial.println(speed);
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("");
    client.println("Speed updated");
    return;
  }

  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("");
  
  client.print(F(R"rawliteral(
    <!doctype html>
    <html>
        <head>
            <title>WiFi Controlled Robot</title>
            <meta name='viewport' content='width=device-width, initial-scale=1.0'>
            <meta name="apple-mobile-web-app-capable" content="yes">
            <meta name="mobile-web-app-capable" content="yes"> 
            <style>
              body {
                margin: 0;
                padding: 0;
                font-family: sans-serif;
                display: flex;
                flex-direction: column;
                align-items: center;
                justify-content: center;
                height: 100vh;
              }
              #joystick-zone {
                width: 150px;
                height: 150px;
                touch-action: none;
                margin-bottom: 20px;
              }

              .slider {
                width: 80vw;
                max-width: 400px;
              }

              button {
                width: 60px;
                height: 60px;
                font-size: 24px;
                border-radius: 10px;
              }

              .slidecontainer {
                margin-top: 10px;
                text-align: center;
              }

              .dpad {
                display: flex;
                flex-direction: column;
                align-items: center;
                margin-bottom: 20px;
              }

              .dpad-middle {
                display: flex;
                justify-content: center;
                gap: 10px;
                margin: 10px 0;
              }

              .dpad button {
                width: 60px;
                height: 60px;
                font-size: 24px;
              }
              
              .slidecontainer {
                margin-top: 10px;
                text-align: center;
              }
            </style>
            <script src='https://cdn.jsdelivr.net/npm/nipplejs@0.9.0/dist/nipplejs.min.js'></script>
        </head>
        <body
            style='
                display: flex;
                justify-content: center;
                align-items: center;
                height: 100vh;
                flex-direction: column;
                font-family: sans-serif;
            '
        >
            <h1>Esp8266 Robot Control</h1>
                <div id='joystick-zone' style='width:150px; height:150px;'></div>
                <div class='slidecontainer'>
                    <p>Speed: <span id="speedValue">650</span></p>
                    <input
                        type='range'
                        min='0'
                        max='1023'
                        value='650'
                        class='slider'
                        id='myRange'
                        oninput='updateSpeed(this.value)'
                    />
                </div>
            
            <script>
                function updateSpeed(val) {
                    document.getElementById('speedValue').innerText = val;
                    fetch('/speed?val=' + val)
                      .then(response => response.text())
                      .then(data => {console.log("Speed Updated: ", val);
                      });
                }
                var joystick = nipplejs.create({
                  zone: document.getElementById('joystick-zone'),
                  mode: 'static',
                  position: { left: "50%", top: "50%" },
                  color: 'grey',
                });

                joystick.on('dir', function (evt, data) {
                  if (data.direction) {
                    const dir = data.direction.angle;
                    console.log("Direction: ", dir);
                    sendCommand(dir);
                  }
                });

                joystick.on('end', function () {
                  fetch('/stop');
                });

                function sendCommand(cmd) {
                  fetch('/' + cmd)
                    .then(response => response.text())
                    .then(data => {console.log("Command sent: ", cmd)})
                    .catch(err => {console.error("Error sending command: ", err);
                    });
                }

            </script>
        </body>
    </html>)rawliteral"));

  delay(1);
  Serial.println("Client disconnected");
  Serial.println("");
}

