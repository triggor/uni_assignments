int IR = 10;
int sensor = 0;
int LED = 12;

void setup() {
  // put your setup code here, to run once:
pinMode(LED, OUTPUT);
pinMode(IR, OUTPUT);
pinMode(sensor, INPUT);
Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  float val = (exp(sin(millis()/2000.0*PI)) - 0.36787944)*108.0;
  analogWrite(IR,val);
  int signal = analogRead(sensor);
  Serial.println(signal);
  signal = map(signal,0,1023,0,255);
  analogWrite(LED, signal);
  
}
