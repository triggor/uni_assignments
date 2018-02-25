/*
Zaimplementuj grę typu zgadnij liczbę od 0-99. Wprowadzanie danych powinno się odbywać za pomocą impulsatora 
(inaczej enkodera, jest w zestawie), wybór ma być zatwierdzany przyciskiem. Informację o tym, czy szukana liczba 
jest większa, czy mniejsza od wybranej ma być pokazana za pomocą diod LED. Obecnie wybrana liczba ma być wyświetlona 
na wyświetlaczu 8-segmentowym przy pomocy układu drivera SCT2168. [3] Ładowanie danych do układu można zrealizować 
programowo lub przy pomocy sprzętowego SPI. [1] Dodatkowe punkty za wersję używającą dwóch Arduino, komunikujących 
się przez SPI: pierwsze ma pracować jako interfejs dla użytkownika jako SPI master, drugie jako slave ma losować 
liczbę do zgadnięcia, odbierać zapytania z wybraną liczbą od mastera i odpowiadać (również po SPI) w jakiej relacji 
jest wybrana liczba od liczby wylosowanej. [2]
*/

const int RIDDLE = 10;
const int outputA = 3;
const int outputB = 2;
const int button = 4;
const int greaterLED = 5;
const int lessLED = 6;


int counter = 0; 
int aState;
int bState;
int buttonState;
int aLastState;
int lastChosen = -1;  

void setup() { 
  pinMode (outputA,INPUT);
  pinMode (outputB,INPUT);
  pinMode (button,INPUT);
  pinMode (greaterLED,OUTPUT);
  pinMode (lessLED,OUTPUT);
  
  Serial.begin (9600);
  // Reads the initial state of the outputA
  aLastState = digitalRead(outputA);   
} 

void loop() { 
  buttonState = digitalRead(button); 
  aState = digitalRead(outputA); // Reads the "current" state of the outputA
  bState = digitalRead(outputB);
  // If the previous and the current state of the outputA are different, that means a Pulse has occured
  if(buttonState == HIGH){
    if(lastChosen != counter){
      Serial.print("Chosen value = ");
      Serial.println(counter);
      lastChosen = counter;
      if(lastChosen == RIDDLE){
        Serial.println("You won!");
        digitalWrite(lessLED, HIGH);
        digitalWrite(greaterLED, HIGH);
        delay(1000);
        exit(0);
      }
      else if(lastChosen < RIDDLE){
        Serial.println("GREEN==lesser!");
        digitalWrite(lessLED, HIGH);
        delay(500);
        digitalWrite(lessLED, LOW);
      }
      else{
        Serial.println("RED==greater!");
        digitalWrite(greaterLED, HIGH);
        delay(500);
        digitalWrite(greaterLED, LOW);
      }
    }
  }
  
   
  if (aState != aLastState){     
    // If the outputB state is different to the outputA state, that means the encoder is rotating clockwise
    if (bState != aState) { 
      counter = (counter+1)%100;
    } else {
      counter = counter<=0 ? 99-(counter%100) : --counter;
    }
    Serial.print("Position(A, B, button, counter): ");
    Serial.print(aState);
    Serial.print(", ");
    Serial.print(bState);
    Serial.print(", ");
    Serial.print(buttonState);
    Serial.print(", ");
    Serial.println(counter);
  } 
  aLastState = aState; // Updates the previous state of the outputA with the current state
  //delay(100);
}
