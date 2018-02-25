#include <LiquidCrystal.h>
const int wPWM = 9;
const int wADC = A0;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2); 

int sensorReading;
int baseSignal = 0;
float temp;
float upperBound=40;
float lowerBound=39;

void setup() {
  lcd.begin(16, 2);
  lcd.print("Thermostat");
  delay(1000);
  pinMode(wPWM, OUTPUT);
  Serial.begin(9600);
  analogReference(INTERNAL);
}

void loop() {
  lcd.clear();
  pinMode(wPWM, INPUT);
  sensorReading = analogRead(wADC);
  pinMode(wPWM, OUTPUT);
  Serial.print("At ");
  Serial.print(baseSignal);
  lcd.setCursor(0, 0);
  lcd.print("At PWM=");
  lcd.print (baseSignal);
  Serial.print(" sensor reads ");
  sensorReading = map(sensorReading, 0, 1023, -50, 60);
  lcd.setCursor(0, 1);
  lcd.print("Sensor=");
  lcd.print(sensorReading);
  lcd.print(" C");
  Serial.println(sensorReading);
  if(sensorReading<lowerBound){
    baseSignal = (baseSignal+5)%255;
  }
  else if(sensorReading>upperBound && baseSignal>5){
    baseSignal = (baseSignal-10)%255;
  }
  analogWrite(wPWM, baseSignal);
  delay(1000);

}

