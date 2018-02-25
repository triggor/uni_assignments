int sensor = 0;
int freq = 0;
const int bipperPin = 2;
const int magnetPin = 0;
const int ledPin = 9;


void setup() {
  //start serial connection
  Serial.begin(9600);
  //configure pin2 as an input and enable the internal pull-up resistor
  pinMode(magnetPin, INPUT);
  pinMode(bipperPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
}
unsigned long previousMillis = 0;
int del = 1000;
void loop() {
  unsigned long currentMillis = millis();
  
  if(currentMillis - previousMillis >= del)
  {
    previousMillis = currentMillis;
    sensor = analogRead(magnetPin);
    freq = map(sensor,507,940,0,1000);
    Serial.println(freq);
    if(abs(freq)<=5) noTone(bipperPin);
    else myTone(bipperPin, abs(freq));
    if(abs(freq)>=1150) digitalWrite(ledPin, HIGH);
  }
  
}

void myTone(int pin, int freq){
  tone(pin, freq);
}
/*
  
void noTone(pin){
  
}*/
