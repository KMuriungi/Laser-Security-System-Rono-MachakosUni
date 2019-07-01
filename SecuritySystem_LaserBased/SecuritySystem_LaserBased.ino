/*
 * SECURITY SYSTEM - RFID Based With Laser and LDR
 * Arduino Controlled System
*/

/*
 ************************************************************
 *----------------------LCD LIBRARY-------------------------*
 ************************************************************
*/
#include<LiquidCrystal.h>  // Including LCD Library
LiquidCrystal lcd(12, 11, 10, 9, 8, 7); //Configuring pins used 

/*
 ************************************************************
 *-------------------KEYPAD LIBRARY-------------------------*
 ************************************************************
*/
#include <Keypad.h> //Keypad Library

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns

//define the symbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {6, 5, 4, 3}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {14, 15, 16, 17}; //connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad : myKeypad
Keypad myKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

String rono_Pass = "C123#"; // Password verification for RONO's Card
String mary_Pass = "A456#"; // Password verification for MARY's Card

int compareCODE(String alfaCode)    //We type a code on keypad and this will be 
  {                             //compared with the rono_Pass;
   if(alfaCode.equals(rono_Pass))
   return 1;
   
   else if(alfaCode.equals(mary_Pass))  //compared with the mary_Pass;
   return 2;
  
   else return 0;  
}//END compareCODE

String takeCode(char x) //Will display on the LCD when the Password is typed
{ char vec[10];

 vec[0]=x;

 lcd.setCursor(0,4);

 lcd.clear();

 lcd.print('*');

 for(int i=1;i<5;i++)

    {vec[i] = myKeypad.waitForKey(); //Waits for 5 keys to be pressed and after that  

     lcd.print('*');}               //is taking the decision

 vec[5]=NULL;

 String str(vec);

 return str;

} //END takeCode

/*
 ************************************************************
 *---------------------RFID LIBRARY-------------------------*
 ************************************************************
*/
#include <SPI.h> // Including SPI library which is used together with MFRC522 library
#include <MFRC522.h> //Including RC522 Library
 
#define SS_PIN 53 // Configuring SDA pin used
#define RST_PIN 48  // Configuring RST / RESET pin used
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

/*
 ************************************************************
 *-----------------CUSTOM DESIGN SYMBOL---------------------*
 ************************************************************
*/
// Designing a custom HEART Symbol ( Initialize the Function)
byte heart[8] = {
  0b00000,0b01010,0b11111,0b11111,0b11111,0b01110,0b00100,0b00000
};

// Designing a custom SMILEY Symbol ( Initialize the Function)
byte smiley[8] = { 
  0b00000,0b00000,0b01010,0b00000,0b00000,0b10001,0b01110,0b00000
};

// Designing a custom FROWNIE Symbol ( Initialize the Function)
byte frownie[8] = { 
  0b00000,0b00000,0b01010,0b00000,0b00000,0b00000,0b01110,0b10001
};

// Designing a custom ARMSDOWN Symbol ( Initialize the Function)
byte armsDown[8] = { 
  0b00100,0b01010,0b00100,0b00100,0b01110,0b10101,0b00100,0b01010
};

int redLed = 22;
int greenLed = 24;
int yellowLed = 26;

int alarm = 39;

int laser1 = 47;
int laser2 = 45;
int laser3 = 43;

//int resetButton = 2;

int resetBnState = 0;
//int resetBnState = LOW;

/* // LDR configurations on the Arduino Analog Pins
 * ldr1 = A0;
 * ldr2 = A2;
 * ldr3 = A3;
*/

/*
 ************************************************************
 *----------------INITIALIZATION OF DATA--------------------*
 ************************************************************
*/
void setup() {
  lcd.begin(16,2);  // Initialize LCD
  lcd.createChar(1, heart); // Initializing the Symbol Creation Function in LCD
  lcd.createChar(2, smiley); // Initializing the Symbol Creation Function in LCD
  lcd.createChar(3, frownie); // Initializing the Symbol Creation Function in LCD
  lcd.createChar(4, armsDown); // Initializing the Symbol Creation Function in LCD
  
  SPI.begin();      // Initialize SPI bus
  mfrc522.PCD_Init();   // Initialize MFRC522
  mfrc522.PCD_DumpVersionToSerial();  // Show details of PCD - MFRC522 Card Reader details
  
  Serial.begin(9600);   // Initialize serial communications with the PC
  while (!Serial);    // Do nothing if no serial port is opened
  Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));

  pinMode(redLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(yellowLed, OUTPUT);
  
  pinMode(alarm, OUTPUT);
  
  pinMode(laser1, OUTPUT);
  pinMode(laser2, OUTPUT);
  pinMode(laser3, OUTPUT);
  
          //lcd.write(1); // Print/Draw Heart symbol to the LCD
          //lcd.write(2); // Print/Draw Smiley symbol to the LCD
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.write(1);lcd.write(1);lcd.write(2); 
  lcd.print(" SYSTEM ");
  lcd.write(2);lcd.write(1);lcd.write(1); 
  lcd.setCursor(2,1);
  lcd.write(2);lcd.write(2);
  lcd.print(" READY ");
  lcd.write(2);lcd.write(2);

  digitalWrite(greenLed, HIGH);
  digitalWrite(redLed, HIGH);
  digitalWrite(yellowLed, HIGH);

  digitalWrite(laser1, HIGH);
  digitalWrite(laser2, HIGH);
  digitalWrite(laser3, HIGH);

} //END SETUP()

/*
 ************************************************************
 *                                                          *
 *-----------------------MAIN CODE--------------------------*
 *                                                          *
 ************************************************************
*/

void loop() 
{
  // Scanning New Cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
    {
      return;
    } //END IF() scanning cards
  // Selecting one of the Card
  if ( ! mfrc522.PICC_ReadCardSerial()) 
    {
      return;
    } //END IF() selecting cards
  
  //Show UID on serial monitor
  Serial.print("UID tag :");
  String content= ""; // Tag Name in Serial Monitor
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
    {
       Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
       Serial.print(mfrc522.uid.uidByte[i], HEX); // Show UID as HEX code
       content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
       content.concat(String(mfrc522.uid.uidByte[i], HEX)); // Show UID as HEX code
    } //END FOR()
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase(); // Display content in UpperCase

  if (analogRead(0) < 600)
  {
    digitalWrite(yellowLed,HIGH);
    
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print( "Scan Your Card" );
    delay(1000);

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print( "Input Password" );
    
      if (content.substring(1) == "62 D9 EA D5") //UID of the card that will be given access
        {
          Serial.println("CARD 1");

          keypadCheck();
    
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.write(1); //Heart Symbol
          lcd.print(" WELCOME RONO ");
          lcd.write(1); //Heart Symbol
          delay(500);

          accessGranted();

          delay(1000);

          generalState();
          
        } //END IF() ** CARD 1
    
      else if (content.substring(1) == "A3 B7 8D 00") //UID of the card that will be given access
        {
          Serial.println("CARD 2");

          keypadCheck();
    
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.write(1); //Heart Symbol
          lcd.print(" WELCOME MARY ");
          lcd.write(1); //Heart Symbol
          delay(500);
          
          accessGranted();

          delay(1000);

          generalState();
          
          
        } //END ELSE IF() ** CARD 2
       
      else {
        Serial.println("CARD 3");
        accessDenied();
        
        delay(1000);

        generalState();
          
        
      } //END ELSE()
  } // END if (analogRead(0) < 600)

  if (analogRead(2) < 600 || analogRead(3) < 600 && analogRead(0) > 600 )
  {
    unauthorizedAccess();
    
    delay(1000);

    generalState();
          
    } //END if || ()
} //END VOID LOOP()

/*
 ************************************************************
 *                                                          *
 **********************CUSTOM FUNCTIONS**********************
 *                                                          *
 ************************************************************
*


------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------


/*
 ************************************************************
 *-----------------KEYPAD CHECK FUNCTION--------------------*
 ************************************************************
*/
void keypadCheck()
{
  char c = myKeypad.getKey();

  if(c != NO_KEY){

String currentCode = takeCode(c);

int A = compareCODE(currentCode);

  if(A==0){         //A is a variable that stores the current code

   lcd.clear();

   lcd.print("INVALID CODE");

   delay(2000);

    lcd.setCursor(0,0);

    lcd.clear();

    lcd.print("ACCESS DENIED");

   return;
  } //END if (A0)

  if(A==1){

     lcd.setCursor(0,0);

     lcd.clear();

     lcd.print( "VALID CODE " );

delay(2000);

   //state = 1;

   return;
  } //END if (A1)

  if(A==2); {

      //state=2;

         lcd.clear();

         lcd.setCursor(0,0);

        lcd.print( "VALID CODE " );

      delay(2000);

    return;} //END if (A2)
    }
  } //END  keypadCheck()

/*
 ************************************************************
 *----------------ACCESS GRANTED FUNCTION-------------------*
 ************************************************************
*/

void accessGranted()
{
  digitalWrite(laser1, HIGH);
  digitalWrite(laser2, LOW);
  digitalWrite(laser3, LOW);

  digitalWrite(yellowLed, HIGH);
  digitalWrite(greenLed, LOW);
  digitalWrite(redLed, LOW);
  delay(2000);
  digitalWrite(yellowLed, LOW);
  
  lcd.setCursor(0,1);
  lcd.write(2); //Smiley Symbol
  lcd.print("ACCESS GRANTED");
  lcd.write(2); //Smiley Symbol

  delay(1000);
  } //END ACCESS GRANTED ()

/*
 ************************************************************
 *----------------ACCESS DENIED FUNCTION--------------------*
 ************************************************************
*/
void accessDenied()
{
  digitalWrite(laser1, HIGH);
  digitalWrite(laser2, HIGH);
  digitalWrite(laser3, HIGH);

  digitalWrite(yellowLed, LOW);
  digitalWrite(greenLed, LOW);
  digitalWrite(redLed, HIGH);
  digitalWrite(alarm, HIGH);
  delay(2000);
  digitalWrite(redLed, LOW);
  digitalWrite(alarm, LOW);
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.write(3); //Frownie Symbol
  lcd.print(" UNAUTHORIZED ");
  lcd.write(3);
  
  lcd.setCursor(0,1);
  lcd.write(4); //ArmsDown Symbol
  lcd.print("ACCESS DENIED");
  lcd.write(4);

  delay(500);
  } //END ACCESS DENIED ()

/*
 ************************************************************
 *-------------UNAUTHORIZED ACCESS FUNCTION-----------------*
 ************************************************************
*/
void unauthorizedAccess()
{
  digitalWrite(laser1, HIGH);
  digitalWrite(laser2, HIGH);
  digitalWrite(laser3, HIGH);

  digitalWrite(redLed, HIGH);
  digitalWrite(alarm, HIGH);
  digitalWrite(yellowLed, LOW);
  digitalWrite(greenLed, LOW);
  delay(2000);
  digitalWrite(redLed, LOW);
  digitalWrite(alarm, LOW);
  
  lcd.setCursor(0,0);
  lcd.write(4); //ArmsDown Symbol
  lcd.print(" UNAUTHORIZED ");
  lcd.write(4); //ArmsDown Symbol

  lcd.setCursor(0,1);
  lcd.write(4);lcd.write(4);lcd.write(4); //ArmsDown Symbol
  lcd.print("  ACCESS  ");
  lcd.write(4);lcd.write(4);lcd.write(4); //ArmsDown Symbol

  delay(1000);
  } //END UNAUTHORIZED ACCESS ()
  
/*
 ************************************************************
 *----------------GENERAL STATE FUNCTION--------------------*
 ************************************************************
*/
void generalState() 
{
  digitalWrite(greenLed, HIGH);

  digitalWrite(laser1, HIGH);
  digitalWrite(laser2, HIGH);
  digitalWrite(laser3, HIGH);

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.write(1);lcd.write(1);lcd.write(2); 
  lcd.print("  SYSTEM  ");
  lcd.write(2);lcd.write(1);lcd.write(1); 
  lcd.setCursor(2,1);
  lcd.write(2);lcd.write(2);
  lcd.print("  READY  ");
  lcd.write(2);lcd.write(2);

  delay(2000);

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.write(1); 
  lcd.print(" SYSTEM READY ");
  lcd.write(1);
  symbolBlink();
  } //END GENERAL STATE ()

/*
 ************************************************************
 *----------------SYMBOL BLINK FUNCTION---------------------*
 ************************************************************
*/
void symbolBlink() 
  {
  lcd.setCursor(0,1); lcd.write(1); delay(500);
  lcd.setCursor(1, 1); lcd.write(2); delay(500);
  lcd.setCursor(2, 1); lcd.write(1); delay(500);
  lcd.setCursor(3, 1); lcd.write(2); delay(500);
  lcd.setCursor(4, 1); lcd.write(1); delay(500);
  lcd.setCursor(5, 1); lcd.write(2); delay(500);
  lcd.setCursor(6, 1); lcd.write(1); delay(500);
  lcd.setCursor(7, 1); lcd.write(2); delay(500);
  lcd.setCursor(8, 1); lcd.write(1); delay(500);
  lcd.setCursor(9, 1); lcd.write(2); delay(500);
  lcd.setCursor(10, 1); lcd.write(1); delay(500);
  lcd.setCursor(11, 1); lcd.write(2); delay(500);
  lcd.setCursor(12, 1); lcd.write(1); delay(500);
  lcd.setCursor(13, 1); lcd.write(2); delay(500);
  lcd.setCursor(14, 1); lcd.write(1); delay(500);
  lcd.setCursor(15, 1); lcd.write(2); delay(500);
  } //END SYMBOL BLINK ()

