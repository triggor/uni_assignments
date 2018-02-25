
const int seven_seg_digits[11][8] = {  { 1,1,0,1,0,1,1,1 },  // = 0
                                       { 0,0,0,1,0,0,0,1 },  // = 1
                                       { 1,1,1,0,0,0,1,1 },  // = 2
                                       { 0,1,1,1,0,0,1,1 },  // = 3
                                       { 0,0,1,1,0,1,0,1 },  // = 4
                                       { 0,1,1,1,0,1,1,0 },  // = 5
                                       { 1,1,1,1,0,1,1,0 },  // = 6
                                       { 0,0,0,1,0,0,1,1 },  // = 7
                                       { 1,1,1,1,0,1,1,1 },  // = 8
                                       { 0,1,1,1,0,1,1,1 },  // = 9
                                       { 0,0,0,0,1,0,0,0 }   // = dot
                                       };
                                       

void setup() {                
  pinMode(13, OUTPUT);   
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(6, OUTPUT);
}

void loop() {
  for (int digit = 0; digit < 11; digit++) {
    for(int section = 13; section > 5; section--){
      digitalWrite(section, seven_seg_digits[digit][13-section]);  
    }
    delay(1000);
  }
}



