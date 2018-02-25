const int ledPin = 9;

boolean timetable[100];
int current = 0;
unsigned long previousMillis = 0;
void setup() {
  //start serial connection
  Serial.begin(9600);
  //configure pin2 as an input and enable the internal pull-up resistor
  pinMode(2, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  for(int i = 0; i<100; i++)
  {
    timetable[i]=false;
  }
}

void loop() {
  unsigned long currentMillis = millis();
  
  if(currentMillis - previousMillis >= 10)
  {
    previousMillis = currentMillis;
    if(timetable[current])
    {
      digitalWrite(ledPin, HIGH);
    }
    else
    {
      digitalWrite(ledPin, LOW);
    }
    
    int sensorVal = digitalRead(2);
    if (sensorVal == HIGH) {
      timetable[current] = false;
    }
    else {
      timetable[current] = true;
    }
    current++;
    current = current % 100;
  }
}
