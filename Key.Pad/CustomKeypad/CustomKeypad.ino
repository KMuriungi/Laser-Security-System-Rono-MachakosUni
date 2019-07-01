/* @file CustomKeypad.pde
|| @version 1.0
|| @author Alexander Brevig
|| @contact alexanderbrevig@gmail.com
||
|| @description
|| | Demonstrates changing the keypad size and key values.
|| 
*/
#include <Keypad.h>

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

/*
 * The leftmost pin is pin 8 on the keypad and the rightmost is pin 1
 * Pins 8, 7, 6, 5 on the keypad should be connected to 
 * digital pins 3, 2, 1, 0 on the Arduino respectively.
 * Pins 4, 3, 2, 1 on the keypad should be connected 
 * to digital pins 7, 6, 5, 4 on the Arduino respectively.
*/

byte rowPins[ROWS] = {3, 2, 1, 0}; //connect to the row pinouts of the keypad : 8,7,6,5
byte colPins[COLS] = {7, 6, 5, 4}; //connect to the column pinouts of the keypad : 4,3,2,1

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

void setup(){
  Serial.begin(9600);
}
  
void loop(){
  char customKey = customKeypad.getKey();
  
  if (customKey){
    Serial.println(customKey);
  }
}
