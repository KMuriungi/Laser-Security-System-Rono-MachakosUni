enum {INIT_LCD, SCAN_TAG, KEY_IN_PASSWORD} state = INIT_LCD;

// define for nfc
#include <SPI.h>
#include <MFRC522.h>

#define NOT_FOUND -1

#define RST_PIN 9  
#define SS_PIN  10 

MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance

#define NR_KNOWN_CARDS  2
#define CARD_SIZE       4

// init paralle arrays
byte knownCards[NR_KNOWN_CARDS][CARD_SIZE] = {
  {0x93, 0x47, 0xe0, 0x00},
  {0xe5, 0xa2, 0x9c, 0x2c}
};

char name[NR_KNOWN_CARDS][10] = {
  "James",
  "Richard"
};

char password[NR_KNOWN_CARDS][5] = {
  "1234", 
  "8888"
};

int n;  // current tag index

// define for keypad
#include <Keypad.h>

const byte ROWS = 4; 
const byte COLS = 4; 

//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
//byte rowPins[ROWS] = {6, 7, 8, 9}; 
//byte colPins[COLS] = {2, 3, 4, 5}; 
byte rowPins[ROWS] = {5, 4, 3, 2}; 
byte colPins[COLS] = {14, 15, 16, 17}; 

int count = 0;
char keys[5];
unsigned long time;

Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

// define for lcd
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address

// define for led
#include <DigitalToggle.h>

int ledPin = 7;

void setup() {
  // setup nfc
  Serial.begin(9600); 
  while (!Serial);  
  SPI.begin();      
  mfrc522.PCD_Init();  

  // setup keypad (assumed serial began)
  keys[4] = 0;
  
  // setup lcd
  lcd.begin(16,2);

  // setup led
  pinMode(ledPin, OUTPUT);

}

void loop() {
  init_lcd();
  scan_new_card();
  key_in_password();

}

void scan_new_card() {
  if (state != SCAN_TAG) {
    return;  
  }
  
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  mfrc522.PICC_HaltA();
  
  dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
  Serial.print(F(" ("));
  Serial.print(mfrc522.uid.size, DEC);
  Serial.println(F(")"));

  n = indexOf(mfrc522.uid.uidByte, mfrc522.uid.size);
  
  if ( n != NOT_FOUND) {
    Serial.println(name[n]);
    show_user(name[n]);
    time = millis();
    Serial.println(time);
    state = KEY_IN_PASSWORD;
  } else {
    user_not_found();
    state = INIT_LCD;
  }
}

void dump_byte_array(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
      Serial.print(buffer[i] < 0x10 ? " 0" : " ");
      Serial.print(buffer[i], HEX);
  }
}

int indexOf(byte *buffer, byte bufferSize) {
  if (CARD_SIZE == bufferSize) {
    for (byte i = 0; i < NR_KNOWN_CARDS; i++) {
      byte j;
      for (j = 0; j < CARD_SIZE; j++) {
        if (buffer[j] != knownCards[i][j]) break;
      }

      if (j == CARD_SIZE) return i;
    }
  }

  return NOT_FOUND;
}

void key_in_password() {
  if (state != KEY_IN_PASSWORD) {
    return;  
  }
  
  char customKey = customKeypad.getKey();
  
  if (customKey){
    //Serial.println(customKey);
    keys[count] = customKey;
    count++;

    if (count == 4) {
      Serial.println(keys);
      if (strcmp(keys, password[n]) == 0) {
        toggle_led();
        state = INIT_LCD;
      }
      count = 0;
    }
  }

  if (millis() - time > 10000) {  // exceed 10 seconds ?
    state = INIT_LCD;
  }
    
}

void init_lcd() {
  if (state != INIT_LCD) {
    return;
  }

  lcd.clear();
  for(int i = 0; i< 3; i++)
  {
    lcd.backlight();
    delay(250);
    lcd.noBacklight();
    delay(250);
  }
  lcd.backlight();

  lcd.setCursor(0,0);
  lcd.print("Halo World!");
  lcd.setCursor(0,1);
  lcd.print("Tag a NFC card"); 

  state = SCAN_TAG;
}

void user_not_found() {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("User not found!");

  delay(3000);
}

void show_user(char name[10]) {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Hi ");
  lcd.print(name);
  lcd.setCursor(0,1);
  lcd.print("Key in your PIN");
}

void toggle_led() {
  digitalToggle(ledPin);
}
