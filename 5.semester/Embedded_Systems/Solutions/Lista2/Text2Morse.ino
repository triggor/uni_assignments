#include <LiquidCrystal.h> //Dołączenie bilbioteki
//Define the LED Pin
#define PIN_OUT        5
//Define unit length in ms
#define UNIT_LENGTH    200

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
char incomingByte[20];
LiquidCrystal lcd(13, 12, 11, 10, 9, 8); //Informacja o podłączeniu nowego wyświetlacza
 
void setup()
{
  pinMode( PIN_OUT, OUTPUT );
  digitalWrite( PIN_OUT, LOW );
  Serial.begin(9600);
}

void loop()
{
  delay(1000);
  int index = 0;
  while (Serial.available() > 0) // Don't read unless
                                   // there you know there is data
    {  
        if(index < 19) // One less than the size of the array
        {
            char inChar = Serial.read(); // Read a character
            incomingByte[index] = inChar; // Store it
            index++; // Increment where to write next
        }
    }
  incomingByte[index] = '\0'; // Null terminate the string

  // say what you got:
  pinMode( PIN_OUT, OUTPUT );
  digitalWrite( PIN_OUT, LOW );
  lcd.begin(16, 2); //Deklaracja typu
  lcd.setCursor(0, 0); //Ustawienie kursora
  lcd.print("Teraz gramy:"); //Wyświetlenie tekstu
  lcd.setCursor(0, 1); //Ustawienie kursora
  lcd.print(incomingByte); //Wyświetlenie tekstu
  lcd.blink(); //Włącznie kursora
  //lcd.clear();
  
  String morseWord = encode(incomingByte);
    
  for(int i=0; i<=morseWord.length(); i++)
  {
    switch( morseWord[i] )
    {
      case '.': //dit
        tone(PIN_OUT, 4000);
        delay( UNIT_LENGTH );
        noTone(PIN_OUT);
        delay( UNIT_LENGTH );
          
        break;

      case '-': //dah
        tone(PIN_OUT, 4000);
        delay( UNIT_LENGTH*3 );
        noTone(PIN_OUT);
        delay( UNIT_LENGTH );
          
        break;

      case ' ': //gap
        delay( UNIT_LENGTH );
    }
  }
}

String encode(const char* string)
{
  size_t i, j;
  String morseWord = "";
  
  for( i = 0; string[i]; ++i )
  {
    for( j = 0; j < sizeof MorseMap / sizeof *MorseMap; ++j )
    {
      if( toupper(string[i]) == MorseMap[j].letter )
      {
        morseWord += MorseMap[j].code;
        break;
      }
    }
    morseWord += " "; //Add tailing space to seperate the chars
  }

  return morseWord;  
}
