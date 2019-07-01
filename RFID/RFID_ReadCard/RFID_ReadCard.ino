/*
 * PinWiring to Arduino Uno

SDA------------------------Digital 10

SCK------------------------Digital 13

MOSI----------------------Digital 11

MISO----------------------Digital 12

IRQ------------------------unconnected

GND-----------------------GND

RST------------------------Digital 9

3.3V------------------------3.3V (DO NOT CONNECT TO 5V) 
 * 
 */

#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         9
#define SS_PIN          10
MFRC522 mfrc522(SS_PIN, RST_PIN);

void setup()
{

SPI.begin();
mfrc522.PCD_Init();
}

void loop() {
RfidScan();
}

void dump_byte_array(byte *buffer, byte bufferSize) {
for (byte i = 0; i < bufferSize; i++) {
//Serial.print(buffer[i] < 0x10 ? ” 0″ : ” “);
Serial.print(buffer[i], HEX);
}
}

void RfidScan()
{
if ( ! mfrc522.PICC_IsNewCardPresent())
return;

if ( ! mfrc522.PICC_ReadCardSerial())
return;
dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
}
