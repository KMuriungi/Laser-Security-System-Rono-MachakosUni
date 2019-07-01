#include <SPI.h>

#include <MFRC522.h>

#include <Wire.h>

#include <LiquidCrystal_I2C.h>

#include <Keypad.h>

#include<EEPROM.h>



int relPin;

int state=0;

byte  COD[10];

byte  AUX[10];

int k=0;

String accessCode="*123456#";

String codpairing="*654321#";

//NFC

#define RST_PIN   9

#define SS_PIN 10   

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance

#define NEW_UID {0xDE, 0xAD, 0xBE, 0xEF}

MFRC522::MIFARE_Key key;

//LCD

LiquidCrystal_I2C  lcd(0x27,16,2);

//KEYPAD

const byte numRows= 4; //number of rows on the keypad

const byte numCols= 4; //number of columns on the keypad



char keymap[numRows][numCols]=

{

{'1', '2', '3', 'A'},

{'4', '5', '6', 'B'},

{'7', '8', '9', 'C'},

{'*', '0', '#', 'D'}

};

//Code that shows the the keypad connections to the arduino terminals

byte rowPins[numRows] = {2,3,4,5}; //Rows 0 to 3

byte colPins[numCols]= {A0,7,8,9}; //Columns 0 to 3

//initializes an instance of the Keypad class

Keypad myKeypad= Keypad(makeKeymap(keymap), rowPins, colPins, numRows, numCols);

void setup() {

  pinMode(A0,OUTPUT);

  digitalWrite(A0,HIGH);

  pinMode(A3,OUTPUT);

  digitalWrite(A3,HIGH);

  pinMode(A1,OUTPUT);

  digitalWrite(A1,HIGH);

  pinMode(A2,OUTPUT);

  digitalWrite(A2,LOW);

  //NFC

  Serial.begin(9600);  // Initialize serial communications with the PC

  while (!Serial);  // Do nothing if no serial port is opened

  SPI.begin(); // Init SPI bus

 mfrc522.PCD_Init();     // Init MFRC522 card



 for (byte i = 0; i < 6; i++) {        

key.keyByte[i] = 0xFF;

 }

lcd.begin();

lcd.backlight();

lcd.setCursor(0,0);

lcd.clear();

lcd.print( "BLOCKED" );

}

void  readNFC(){   // This function will read the code stored on  

 for (byte i =0; i<(mfrc522.uid.size); i++) {  // the  UID

   COD[i]=mfrc522.uid.uidByte[i];

 }

 Serial.print("COD");

 Serial.print(COD[0]);

 Serial.print(COD[1]);

 Serial.print(COD[2]);

 Serial.print(COD[3]);

}



void pairNFC(){

 Serial.println("COD in pair");

 Serial.print(COD[0]);

 Serial.print(COD[1]);

 Serial.print(COD[2]);

 Serial.print(COD[3]);

  long  r=0;

  int c=0;

  for(int i=1;i<=EEPROM.read(0);i++){                       //The UID cannot be stored on

switch(i%4){                                                     // one variable, it was needed to be

      case 1 :{AUX[0]=EEPROM.read(i); break;}     // split

      case 2 :{AUX[1]=EEPROM.read(i); break;}

      case 3 :{AUX[2]=EEPROM.read(i); break;}

      case 0 :{AUX[3]=EEPROM.read(i); break;}

}

if((i)%4==0)

   {Serial.println(r);

    if( AUX[0]==COD[0] && AUX[1]==COD[1] && AUX[2]==COD[2] && AUX[3]==COD[3] ){                                      //Verify if the code is in EEPROM

         lcd.clear();

         lcd.setCursor(0,0);

         lcd.print("CODE ALREADY IN");

         lcd.setCursor(0,1);

         lcd.print("SYSTEM");

      delay(2000);

          c=1;

      break;}

  }

  }



if(c==0){int aux2=EEPROM.read(0);

             Serial.println("CODE PAIRED");

Serial.print(COD[0]);

Serial.print(COD[1]);

Serial.print(COD[2]);

Serial.print(COD[3]);



         EEPROM.write(aux2+1,COD[0]);  //Writing code in EEPROM

         EEPROM.write(aux2+2,COD[1]);

         EEPROM.write(aux2+3,COD[2]);

         EEPROM.write(aux2+4,COD[3]);



         aux2=aux2+4; // Position for a new code

         Serial.println("aux2");

         Serial.println(aux2);

         EEPROM.write(0,0);

         EEPROM.write(0,aux2);    

         lcd.clear();

         lcd.setCursor(0,0);

         lcd.print("CODE PAIRED");

         delay(2000);}   

 }



boolean validationNFC(){

  boolean c=false;

  for(int i=1;i<=EEPROM.read(0);i++){   //Read the EEPROM

      switch(i%4){

      case 1 :{AUX[0]=EEPROM.read(i); break;}

      case 2 :{AUX[1]=EEPROM.read(i); break;}

      case 3 :{AUX[2]=EEPROM.read(i); break;}

      case 0 :{AUX[3]=EEPROM.read(i); break;}

    }

if((i)%4==0)

   {

    if( AUX[0]==COD[0] && AUX[1]==COD[1] && AUX[2]==COD[2] &&                AUX[3]==COD[3])

      c=true; //Verify if the code is in EEPROM and make flag=true;

     }

}       

  return c;  

}



int compareCODE(String a)    //We type a code on keypad and this will be compared

{                             //with the accessCode;

 if(a.equals(accessCode))    

 return 1;

 else if(a.equals(codpairing)) 
 
 return 2;

 else return 0;  

}


String takeCode(char x) //Will display on the LCD the code typed

{ char vec[10];

 vec[0]=x;

 lcd.setCursor(0,0);

 lcd.clear();

 lcd.print('X');

 for(int i=1;i<8;i++)

    {vec[i]=myKeypad.waitForKey(); //Waits for 8 keys to be pressed and after that  

     lcd.print('X');}               //is taking the decision

 vec[8]=NULL;

 String str(vec);

 return str;

}  

  void loop() {

  switch(state){

  case 0: {

    mfrc522.PCD_Init();

    if (  mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial() ){

    readNFC(); //It will read the card and it will search for UID in its

    if(validationNFC()) //memory

       { state=1;

        lcd.clear();

        lcd.setCursor(0,0);

lcd.print( "VALID NFC CODE" ); //The door will be opened

        delay(1000);

        return;

       }

    else{

         lcd.clear();

         lcd.setCursor(0,0);

         lcd.print( "INVALID NFC CODE" ) //If the code was wrongblocked

         delay(1000);

         lcd.setCursor(0,0);

         lcd.clear();

         lcd.print( "BLOCKED" );

         return;

        }

    }      

char c = myKeypad.getKey();

  if(c != NO_KEY){

String codcurent=takeCode(c);

int A=compareCODE(codcurent);

  if(A==0){                                //A is a variable that stores the current code

   lcd.clear();

   lcd.print("INVALID CODE");

   delay(2000);

    lcd.setCursor(0,0);

    lcd.clear();

    lcd.print("BLOCKED");

   return;

}

  if(A==1){

lcd.setCursor(0,0);

     lcd.clear();

   lcd.print( "VALID CODE " );

delay(2000);

   state = 1;

   Return;

}

  if(A==2); {

state=2;

         lcd.clear();

         lcd.setCursor(0,0);

        lcd.print( " Pairing..." );

      delay(2000);

    return;}

  }

break;

   }



 case 1:{

    lcd.clear();

    lcd.setCursor(0,0);

    lcd.print( "UNLOCKED" );

    digitalWrite(A3,LOW);

    digitalWrite(A1,LOW); //The red LED will be off

    digitalWrite(A2,HIGH); //The green LED will be on

    tone(6,3000,5010); //The buzzer will make a sound

    delay(5000); //After 5 seconds the system will be blocked

    digitalWrite(A3,HIGH);

    digitalWrite(A1,HIGH);

    digitalWrite(A2,LOW);

    state=0;

    lcd.setCursor(0,0);

    lcd.clear();

    lcd.print( "BLOCKED" );

return;

    } //END CASE 1



  case 2:{

      mfrc522.PCD_Init();

      if (  mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial() ){

      readNFC();

      pairNFC();

      state=0;

      delay(2000);

      lcd.clear();

      lcd.setCursor(0,0);

      lcd.print( "BLOCKED" );   }
      
      break;
      
      } //END CASE 2

} //END SWITCH

} //END LOOP()

