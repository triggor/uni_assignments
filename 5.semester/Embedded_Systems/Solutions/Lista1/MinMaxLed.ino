 // pin assignments
 int buttonMax = 8;
 int buttonMin = 9;
 int pinLed = 10;
 int lightRes = 0;

 int Min = 0;
 int Max = 1023;
 boolean maxPress=false;
 boolean minPress=false;
 // initialize the serial port
 // and declare inputs and outputs
 void setup() {
   pinMode(buttonMax, INPUT);
   pinMode(buttonMin, INPUT);
   pinMode(pinLed, OUTPUT);
   Serial.begin(9600);
 }
 
 void loop() {

   maxPress = digitalRead(buttonMax);
   minPress = digitalRead(buttonMin);
   int light = analogRead(lightRes);
   Serial.print(light);
   Serial.print(" ");
   Serial.print(Max);
   Serial.print(" ");
   Serial.println(Min);
   if(maxPress==HIGH){
     Max = light;
   }
   else if (minPress==HIGH){
     Min = light;
   }
  
   int ledout = map(light,Min,Max,255,0);
   if(ledout > 255)
   {
    ledout = 255;
   }
   if(ledout < 0)
   {
    ledout = 0;
   }
   analogWrite(pinLed,ledout);
   delay(50);
 }
