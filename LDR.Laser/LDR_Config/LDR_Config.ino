void setup() {
  pinMode(7, OUTPUT); //laserPin1
  Serial.begin(9600);
}

void loop() {
  digitalWrite(7, HIGH);
  Serial.print("LDR1 = ");
  Serial.println(analogRead(1));
  Serial.print("LDR2 = ");
  Serial.println(analogRead(2));
  delay(1000);
}

/*
 * With Max Light or when the laser is 
 * directly focused to the LRD, the value is
 * between 950 - 1000
 * 
 * When the Laser is not fully focused, the
 * value lowers in between 550 - 750
 * 
 * When the Laser is blocked, the value is 
 * between 150 - 450.
*/
