int buzzer = 39;

int redLed = 22;
int greenLed = 24;
int yellowLed = 26;

int laser1 = 47;
int laser2 = 45;
int laser3 = 43;


void setup() {

  Serial.begin(9600);
  pinMode(buzzer, OUTPUT);
  
  pinMode(redLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(yellowLed, OUTPUT);
  
  pinMode(laser1, OUTPUT);
  pinMode(laser2, OUTPUT);
  pinMode(laser3, OUTPUT);

  digitalWrite(laser1, HIGH); //0
  digitalWrite(laser2, HIGH); //2
  digitalWrite(laser3, HIGH); //3

  digitalWrite(greenLed, HIGH);
  delay(3000);
  digitalWrite(greenLed, LOW);
}

void loop() {
  if (analogRead(0) < 600 && analogRead(2) > 600 && analogRead(3) > 600){ // AccessGranted & AccessDenied
    //011
    digitalWrite(redLed, HIGH);
    delay(500);
    digitalWrite(redLed, LOW);
    }
    if (analogRead(0) > 600 && analogRead(2) < 600 && analogRead(3) < 600){ // AccessGranted & AccessDenied
    //100 
    digitalWrite(yellowLed, HIGH);
    delay(500);
    digitalWrite(yellowLed, LOW);
    }
    if (analogRead(0) < 600 && analogRead(2) < 600 && analogRead(3) > 600){ // AccessGranted & AccessDenied
    //001
    digitalWrite(redLed, HIGH);
    digitalWrite(greenLed, HIGH);
    delay(500);
    digitalWrite(redLed, LOW);
    digitalWrite(greenLed, LOW);
    }
    if (analogRead(0) < 600 && analogRead(2) > 600 && analogRead(3) < 600){ // AccessGranted & AccessDenied
    //010
    digitalWrite(yellowLed, HIGH);
    digitalWrite(greenLed, HIGH);
    delay(500);
    digitalWrite(yellowLed, LOW);
    digitalWrite(greenLed, LOW);
    }
    if (analogRead(0) > 600 && analogRead(2) < 600 && analogRead(3) > 600){ // AccessGranted & AccessDenied
    //101 
    digitalWrite(redLed, HIGH);
    digitalWrite(yellowLed, HIGH);
    delay(500);
    digitalWrite(redLed, LOW);
    digitalWrite(yellowLed, LOW);
    }
    if (analogRead(0) > 600 && analogRead(2) > 600 && analogRead(3) < 600){ // AccessGranted & AccessDenied
    //110
    digitalWrite(redLed, HIGH);
    digitalWrite(yellowLed, HIGH);
    digitalWrite(greenLed, HIGH);
    delay(500);
    digitalWrite(redLed, LOW);
    digitalWrite(yellowLed, LOW);
    digitalWrite(greenLed, LOW);
    }
    if (analogRead(0) < 600 && analogRead(2) < 600 && analogRead(3) < 600 ) //UnauthorizedAccess
  {//000
    digitalWrite(buzzer, HIGH);
    delay(2000);
    digitalWrite(buzzer, LOW);
    }
    if (analogRead(0) > 600 && analogRead(2) > 600 && analogRead(3) > 600 ) //generalState
  {//111
    digitalWrite(greenLed, HIGH);
    delay(500);
    digitalWrite(greenLed, LOW);
    }
      //delay(2000);
}
