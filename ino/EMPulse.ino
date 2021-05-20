/*
 * EMPulse (Haptic book of babel)
 * by Jawstin
 * May 2021
 * 
 * Description :
 * Electro Magnetic Pulse is a simple experimentation with a 
 * coil woven to a piece of textile that pulse thanks to a 
 * magnet and different current intensities.This code has 4 
 * different mode, the first one transcribe in binary pulses 
 * random chartacters, while the second one does it in morse. 
 * The third is a sinwave and the last a simple vibration.
 * 
 * This work was done at ENSAD to experiment with arduino, 
 * e-textile and 3D printing (case) and was inspired 
 * with the concept of haptic communication and Jorge Borges 
 * themes (The Library of Babel and Book of sand).
 * 
 * 
 */

#include <math.h>

int coilPin = 8;
int modePin = 9;

// MODE
// 0 -> BINARY
// 1 -> MORSE
// 2 -> FADE IN/OUT
// 3 -> FAST PULSE

int mode = 1; 

int intChar;
char randChar;
char binChar[9];
char morseChar[8];

const char * morseTable[/*55*/29][2]={
  {"A",".-"}, {"B","-..."}, {"C","-.-."}, {"D","-.."}, {"E","."},
  {"F","..-."}, {"G","--."}, {"H","...."}, {"I",".."}, {"J",".---"},
  {"K","-.-"}, {"L",".-.."}, {"M","--"}, {"N","-."}, {"O","---"},
  {"P",".--."}, {"Q","--.-"}, {"R",".-."}, {"S","..."}, {"T","-"},
  {"U","..-"}, {"V","...-"}, {"W",".--"}, {"X","-..-"}, {"Y","-.--"},
  {"Z","--.."}, /*{"0","-----"}, {"1",".----"}, {"2","..---"}, {"3","...--"},
  {"4","....-"}, {"5","....."}, {"6","-...."}, {"7","--..."}, {"8","---.."},
  {"9","----."}, */{".","._"}, {",","--..--"}, /*{"?","..--.."}, {"'",".----."},
  {"!","-.-.--"}, {"/","-..-."}, {"(","-.--."}, {")","-.--.-"}, {"&",".-..."},
  {":","---..."}, {";","-.-.-."}, {"=","-...-"}, {"+",".-.-."}, {"-","-....-"},
  {"_","..--.-"}, {"\"",".-..-."}, {"$","...-..-"}, {"@",".--.-."},*/ {" "," "}
};


//Determine binary value of character
int setBinary(int intChar){
  for (int i = 7; i >= 0; i--) {
      int k = intChar >> i;
      if (k & 1){
          binChar[8-i] = '1';
      } else {
          binChar[8-i] = '0';
      }
      Serial.print(binChar[8-i]);
  }
  return 0;
}

//Pulse character in binary
int readBinary(){
  for(int i = 7; i >= 0; i--){
    Serial.print(binChar[8-i]);
    if(binChar[8-i] == '1'){
      analogWrite(coilPin, 255);
      delay(200);
      analogWrite(coilPin, 0);
      delay(100);
    } else {
      analogWrite(coilPin, 0);
      delay(300);
    }    
  }
  Serial.println("");
  return 0;
}

//Determine morse code of character
int setMorse(char randChar){
  
  memset(morseChar,0,sizeof(morseChar));
  for(int i = 0; i < /*54*/29; i++)
    {
        if(morseTable[i][0][0] == toupper(randChar))
        {
            int j = 0;
            while (morseTable[i][1][j] != '\0'){
              morseChar[j] = morseTable[i][1][j];
              Serial.print(morseChar[j]);
              j++;
            }
            break;
        }
    }

  return 0;
}

//Pulse character in morse code
int readMorse(){

  int j = 0;
  while (morseChar[j] != '\0'){
    Serial.print(morseChar[j]);
    if(morseChar[j] == '.'){
      analogWrite(coilPin, 255);
      delay(100);
    } else {
      analogWrite(coilPin, 255);
      delay(300);
    }
    analogWrite(coilPin, 0);
    delay(100);
    j++;
  }
  delay(200);

  Serial.println("");
  return 0;
}

//Generate random ASCII character : 'A-Z'|','|'.'|' '
int setChar(){
  
  //intChar = 32 + (random() % 95);

  //Generate '.'|','|' '|'A-Z'
  intChar = 32 + (random() % 59);
  while(!(intChar == 32 || intChar == 44 ||intChar == 46 
          || (intChar >= 65 && intChar <= 90))) intChar = 32 + (random() % 59);
        
  randChar = intChar;
  Serial.print(randChar);
  Serial.print("[ ");
  Serial.print(intChar);
  Serial.print("\t ; ");
  setBinary(intChar);
  Serial.print("\t ; ");
  setMorse(randChar);
  Serial.print("     \t] (M");
  Serial.print(mode);
  Serial.print(") : \t");
  
  return 0;
}

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(0));
  pinMode(modePin, INPUT_PULLUP);
}

void loop() {

  //Set mode from button
  if(digitalRead(modePin) == LOW){
    if(mode < 3) mode++;
    else mode = 0;
    Serial.println("");
    Serial.print("MODE CHANGE (M");
    Serial.print(mode);
    Serial.println(")");
  }

  //Start choosen mode
  switch (mode) {
    case 0:
      //Serial.println("BINARY MODE (0)");

      setChar();
      readBinary();
      
      break;
    case 1:
      //Serial.println("MORSE MODE (1)");

      setChar();
      readMorse();
      
      break;
    case 2:
      //Serial.println("FADE IN/OUT MODE (8)");
      
      // fade in
      for (int i = 0 ; i <= 255; i +=5) {
        analogWrite(coilPin, i);
        Serial.println(i);
        //delay(10);
      }
      
      // fade out
      for (int i = 255 ; i >= 0; i -=5) {
        analogWrite(coilPin, i);
        Serial.println(i);
        //delay(10);
      }
 
      break;
    case 3:
      //Serial.println("FAST PULSE MODE (9)");
      Serial.println(255);
      analogWrite(coilPin, 255);
      delay(10);
      Serial.println(0);
      analogWrite(coilPin, 0);
      delay(10);
      
      break;
    default:
      Serial.println("MODE NOT AVAILABLE");
      break;
  }
  return 0;
}
