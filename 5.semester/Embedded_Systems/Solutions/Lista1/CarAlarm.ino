//Author: Igor Tryhub, 275235
const int vigilantButton = 2;
const int alarmButton = 3;
const int vigilantLED = 4;
const int buzzer = 5;
unsigned long lasttime = millis();

void setup() {
  pinMode(vigilantButton, INPUT);
  pinMode(alarmButton, INPUT);
  pinMode(vigilantLED, OUTPUT);
  pinMode(buzzer, OUTPUT);
  Serial.begin(9600);
}

// the loop function runs over and over again forever
void loop() {
  Serial.print("Time: ");
  time = millis();
  //prints time since program started
  Serial.println(time);
  // wait a second so as not to send massive amounts of data
  delay(1000);
  //digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
  //delay(1000);              // wait for a second
  //digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
  //delay(1000);              // wait for a second
}
