int door = 8;
int doorState = LOW;

void setup() {
  // put your setup code here, to run once:
  pinMode(door, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  doorState = digitalRead(door);
  if (doorState == LOW){
    digitalWrite(door, HIGH);
    delay(100);
    digitalWrite(door, LOW);
    delay(100);
    }else {
      digitalWrite(door, LOW);
      delay(100);
      }
}
