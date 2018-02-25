//Author:Igor Tryhub, 275235
#include <elapsedMillis.h>
#include <LiquidCrystal.h> //Dołączenie bilbioteki
//Define the LED Pin
#define PIN_OUT        5
//Define unit length in ms
#define UNIT_LENGTH    250

//Build a struct with the morse code mapping
static const struct {const char letter, *code;} MorseMap[] =
{
	{ 'A', ".-" },
	{ 'B', "-..." },
	{ 'C', "-.-." },
	{ 'D', "-.." },
	{ 'E', "." },
	{ 'F', "..-." },
	{ 'G', "--." },
	{ 'H', "...." },
	{ 'I', ".." },
	{ 'J', ".---" },
	{ 'K', "-.-" },
	{ 'L', ".-.." },
	{ 'M', "--" },
	{ 'N', "-." },
	{ 'O', "---" },
	{ 'P', ".--." },
	{ 'Q', "--.-" },
	{ 'R', ".-." },
	{ 'S', "..." },
	{ 'T', "-" },
	{ 'U', "..-" },
	{ 'V', "...-" },
	{ 'W', ".--" },
	{ 'X', "-..-" },
	{ 'Y', "-.--" },
	{ 'Z', "--.." },
	{ ' ', "     " }, //Gap between word, seven units 
		
	{ '1', ".----" },
	{ '2', "..---" },
	{ '3', "...--" },
	{ '4', "....-" },
	{ '5', "....." },
	{ '6', "-...." },
	{ '7', "--..." },
	{ '8', "---.." },
	{ '9', "----." },
	{ '0', "-----" },
		
	{ '.', "·–·–·–" },
	{ ',', "--..--" },
	{ '?', "..--.." },
	{ '!', "-.-.--" },
	{ ':', "---..." },
	{ ';', "-.-.-." },
	{ '(', "-.--." },
	{ ')', "-.--.-" },
	{ '"', ".-..-." },
	{ '@', ".--.-." },
	{ '&', ".-..." },
};
elapsedMillis timeElapsed;
const int button = 2;
LiquidCrystal lcd(13, 12, 11, 10, 9, 8); //Informacja o podłączeniu nowego wyświetlacza
String todisplay = "";
int ind = -1;

void setup()
{
  pinMode( PIN_OUT, OUTPUT );
  pinMode( button, INPUT );
  digitalWrite( PIN_OUT, LOW );
  Serial.begin(9600);
}


void loop()
{  
  
  String msg = collect_input();
  Serial.println(msg);
  String text_msg = decode(msg);
  todisplay += text_msg;
  ind++;
  Serial.println(todisplay);
  
  lcd.begin(16, 2); //Deklaracja typu
  lcd.setCursor(0, 0); //Ustawienie kursora
  lcd.print(msg); //Wyświetlenie tekstu
  lcd.setCursor(0, 1); //Ustawienie kursora
  lcd.print(todisplay.substring((ind-(ind%16))+1,ind+1)); //Wyświetlenie tekstu
  lcd.blink(); //Włącznie kursora
}

String decode(String morse)
{
  String msg = "";
  
    for( int i = 0; i < sizeof MorseMap / sizeof *MorseMap; ++i ){
      if( morse == MorseMap[i].code ){
        msg += MorseMap[i].letter;
        break;
      }
    }
  return msg;
}



String collect_input(void){
  
  String input = "";
  int sign_index = 0;
  boolean pushedButton = false;
  int count_hi = 0;
  int count_lo = 0;
  boolean dirty = false;
  char letter[8];
  Serial.println("");
  
  while (count_lo < 40 && sign_index < 7){
    pushedButton = digitalRead(button);
    if(pushedButton == HIGH){ 
      tone(PIN_OUT, 4000);
      delay(30);
      noTone(PIN_OUT);
      count_hi++;
      if(10<count_lo && count_lo<30){ //next letter
        if(dirty){
        letter[sign_index] = '\0';
        input = letter; 
        break;
        }
      }
      else if(count_hi >= 15){ //error
        Serial.println("+was too long");
        input = "..--..";
        break;
      }
      dirty = true;
      count_lo=0;
    }
    else {
      delay(30);
      count_lo++;
      if(0<count_hi && count_hi<5){ //dit
        letter[sign_index] = '.';
        sign_index++;
      }
      else if(4<count_hi && count_hi<15){ //dah
        letter[sign_index] = '-';
        sign_index++;
      }
      else if(29<count_lo && count_lo<40){ //gap - next word
        if(dirty){
          letter[sign_index] = '\0';
          input = letter;
        }
        else input = "     "; 
        break;
      }
      count_hi=0;
    }
  }
  if(sign_index >= 7) input = "..--.."; //error - should have been letter separator 
  
  return input;
}
