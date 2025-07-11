int led = 13;  
int sensor = 2; 
int val = 0; 

void setup() {
    pinMode(led, OUTPUT); 
    pinMode(sensor, INPUT);
    Serial.begin(9600); 
}

void loop() {
    val = digitalRead(sensor); 

    if (val == HIGH) {                           
        digitalWrite(led, HIGH);
        delay(1000);            
        Serial.println("Motion detected!");
    }
    else {
        digitalWrite(led, LOW);
        delay(1000);           
        Serial.println("Motion stopped!");
    }
}
