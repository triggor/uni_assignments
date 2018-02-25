const int redPin = 11;
const int greenPin = 10;
const int bluePin = 9;

void setup() {
  // put your setup code here, to run once:
pinMode(redPin, OUTPUT);
pinMode(greenPin, OUTPUT);
pinMode(bluePin, OUTPUT);
Serial.begin(9600);
}

void loop() {

  //float val = (exp(sin(millis()/2000.0*PI)) - 0.36787944)*108.0;
  float redVal, greenVal, blueVal;
  int rounds;
  while(1){
    int r = random(255);
    int g = random(255);
    int b = random(255);
    float red = r/255.0;
    float green = g/255.0;
    float blue = b/255.0;

    for (float in = 0; in < 6.283; in = in + 0.05)
    {
      redVal = sin(in) * 127.5 * red + 127.5;
      greenVal = sin(in) * 127.5 * green + 127.5;
      blueVal = sin(in) * 127.5 * blue + 127.5;
      
      Serial.print(in);
      Serial.print(" ");
      Serial.print(greenVal);
      Serial.print(" ");
      Serial.println(blueVal);
      
      analogWrite(redPin, redVal);
      analogWrite(greenPin, greenVal);
      analogWrite(bluePin, blueVal);
    }
  }
}
