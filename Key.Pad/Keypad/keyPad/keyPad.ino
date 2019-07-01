#include <Keypad.h>
const byte ROWS = 4;
const byte COLS = 4;
 
//define the symbols on the buttons of the keypads
char Keys[ROWS][COLS] = 
{
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
 
byte rowPins[ROWS] = {22, 24, 26, 28};
byte colPins[COLS] = {30, 32, 34, 36};
//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad(makeKeymap(Keys),rowPins,colPins,ROWS,COLS);
 
void setup()
{
  Serial.begin(9600);
  Serial.println("Please press the keyboard:");
}
 
void  loop()
{
  char key = customKeypad.getKey();
  if(key!=NO_KEY)
  {
    Serial.print("Key Value : ");
    Serial.println(key);
  }
}
