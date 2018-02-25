#define MANUAL_MODE 1
#define SENSITIVITY 40
#include <SoftwareServo.h>

SoftwareServo XServo;  // create rotating servo object to control a servo 
SoftwareServo YServo; // create tilting servo object to control a servo 


const int XJoy = 0;  // analog pin used to connect the potentiometer
const int YJoy = 1;  // analog pin used to connect the potentiometer
int XVal;    // variable to read the value from the analog pin 
int YVal;    // variable to read the value from the analog pin 

const int UL = 2;  // analog pin used to connect the BROWN photoresistor
const int UR = 3;  // analog pin used to connect the GREY photoresistor
const int DR = 4;  // analog pin used to connect the BLACK photoresistor
const int DL = 5;  // analog pin used to connect the WHITE photoresistor
int ULval;    // variable to read the value from the analog pin 
int URval;    // variable to read the value from the analog pin 
int DRval;    // variable to read the value from the analog pin 
int DLval;    // variable to read the value from the analog pin 
int LEFT;
int RIGHT;
int UP;
int DOWN;
int midXVal;
int midYVal;



void setup() 
{ 
  Serial.begin(9600);
  XServo.attach(5);  // attaches the servo on pin 5 to the servo object 
  YServo.attach(3);  // attaches the servo on pin 3 to the servo object 
  int Xangle = XServo.read();
  int Yangle = YServo.read();
  midXVal = analogRead(XJoy);            // reads the value of the rotating potentiometer (value between 0 and 1023)
  midYVal = analogRead(YJoy);
  Serial.print("  XAngle: ");
  Serial.println(Xangle);
  Serial.print("  YAngle: ");
  Serial.println(Yangle);
  XServo.write(90);
  YServo.write(30);
  delay(50);                           // waits for the servos to get there 
  SoftwareServo::refresh();
  /*
  while(Xangle != 90 || Yangle != 0){
    if(Xangle > 90){
      XServo.write(Xangle-1); 
    }
    else if(Xangle < 90){
      XServo.write(Xangle+1); 
    }

    if(Yangle > 0){
      YServo.write(Yangle-1); 
    }

    delay(20);
    Xangle = XServo.read();
    Yangle = YServo.read();
    SoftwareServo::refresh();
  }
  */
}

void loop() 
{ 
  int Xangle = XServo.read();
  Serial.print("  XAngle: ");
  Serial.println(Xangle);
  
  int Yangle = YServo.read();
  Serial.print("  YAngle: ");
  Serial.println(Yangle);


  
  if (MANUAL_MODE==2){
    XVal = analogRead(XJoy);            // reads the value of the rotating potentiometer (value between 0 and 1023)
    YVal = analogRead(YJoy); 
    Serial.print("  XVal: ");
    Serial.println(XVal);
    Serial.print("  YVal: ");
    Serial.println(YVal);
    //Xangle += (midXVal-XVal)/50;
    //Yangle += (midYVal-YVal)/50;
    
    XServo.write(Xangle);
    YServo.write(Yangle); 
    delay(20); 
  }
  
  else if(MANUAL_MODE==1){
    XVal = analogRead(XJoy);            // reads the value of the rotating potentiometer (value between 0 and 1023)
    YVal = analogRead(YJoy);           // reads the value of the tilting potentiometer (value between 0 and 1023) 
    
    XVal = map(XVal, 10, 915, 0, 10);     // scale it to use it with the servo (value between 0 and 180) 
    XVal -= 5;
    if(XVal < -4) XVal = -4;
    if(5<=Xangle && Xangle<=175) 
      XServo.write(Xangle + XVal);                  // sets the servo position according to the scaled value 
    delay(20);                           // waits for the servos to get there 
    
    YVal = map(YVal, 10, 900, 0, 10);     // scale it to use it with the servo (value between 0 and 180)
    YVal -= 5;
    if(YVal < -4) YVal = -4;
    if(10<=Yangle && Yangle<=85)   
      YServo.write(Yangle + YVal);                  // sets the servo position according to the scaled value 
    
   
  }
  else{
    ULval = analogRead(UL);            // reads the value of the rotating potentiometer (value between 0 and 1023)
    URval = analogRead(UR);            // reads the value of the rotating potentiometer (value between 0 and 1023)
    DRval = analogRead(DR);            // reads the value of the rotating potentiometer (value between 0 and 1023)
    DLval = analogRead(DL);            // reads the value of the rotating potentiometer (value between 0 and 1023)
    LEFT = (ULval + DLval)/2;
    RIGHT = (URval + DRval)/2;
    UP = (ULval + URval)/2;
    DOWN = (DLval + DRval)/2;
    Serial.print("LEFT: ");
    Serial.print(LEFT);
    Serial.print(" |  RIGHT: ");
    Serial.println(RIGHT);
    Serial.print(" | UP: ");
    Serial.println(UP);
    Serial.print(" |  DOWN: ");
    Serial.println(DOWN);

    if(LEFT-RIGHT>=SENSITIVITY && Xangle>3){
      XServo.write(Xangle - 3);
    }
    else if(RIGHT-LEFT>=SENSITIVITY && Xangle<177){
       XServo.write(Xangle + 3);
    }
    if(UP-DOWN>=SENSITIVITY && Yangle>7){
      YServo.write(Yangle - 3);
    }
    else if(DOWN-UP>=SENSITIVITY && Yangle<97){
       YServo.write(Yangle + 3);
    }
  }
  

  delay(50);                           // waits for the servos to get there 
  SoftwareServo::refresh();
} 

