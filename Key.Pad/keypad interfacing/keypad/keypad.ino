//Projectsdunia
//http://facebook.com/projectsdunia
// how to interface 4*3 keypad with arduino

//connected the pin of keypad as follows
int row_1=6;
int row_2=7;
int row_3=8;
int row_4=9;
int col_1=10;
int col_2=11;
int col_3=12;

void setup()
{
  Serial.begin(9600);
  // set row as a output and coloum as a input
  pinMode(row_1,OUTPUT);
  pinMode(row_2,OUTPUT);
  pinMode(row_3,OUTPUT);
  pinMode(row_4,OUTPUT);
  pinMode(col_1,INPUT);
  pinMode(col_2,INPUT);
  pinMode(col_3,INPUT);
  
}
void loop()
{
  int val;
  //setting the columns as high initially
  digitalWrite(col_1,HIGH);
  digitalWrite(col_2,HIGH);
  digitalWrite(col_3,HIGH);
  
  //checking everycondition one by one

  digitalWrite(row_1,LOW);
  digitalWrite(row_2,HIGH);
  digitalWrite(row_3,HIGH);
  digitalWrite(row_4,HIGH);
  //checking each column for row1 
  if(digitalRead(col_1)==0)
  {
    Serial.println("key 1 pressed");
  }
  else if(digitalRead(col_2)==0)
  {
    Serial.println("Key 2 pressed");
  }
  else if(digitalRead(col_3)==0)
  {
    Serial.println("Key 3 pressed");
  }
  
  //case 2: 
  
  digitalWrite(row_1,HIGH);
  digitalWrite(row_2,LOW);
  digitalWrite(row_3,HIGH);
  digitalWrite(row_4,HIGH);
  //checking each column for row2
  if(digitalRead(col_1)==0)
  {
    Serial.println("key 4 pressed");
  }
  else if(digitalRead(col_2)==0)
  {
    Serial.println("Key 5 pressed");
  }
  else if(digitalRead(col_3)==0)
  {
    Serial.println("Key 6 pressed");
  }
  
  //case 3: 
  digitalWrite(row_1,HIGH);
  digitalWrite(row_2,HIGH);
  digitalWrite(row_3,LOW);
  digitalWrite(row_4,HIGH);
  //checking each column for row3 
  if(digitalRead(col_1)==0)
  {
    Serial.println("key 7 pressed");
  }
  else if(digitalRead(col_2)==0)
  {
    Serial.println("Key 8 pressed");
  }
  else if(digitalRead(col_3)==0)
  {
    Serial.println("Key 9 pressed");
  }
  
  //case 4: 
  digitalWrite(row_1,HIGH);
  digitalWrite(row_2,HIGH);
  digitalWrite(row_3,HIGH);
  digitalWrite(row_4,LOW);
  //checking each column for row4
  if(digitalRead(col_1)==0)
  {
    Serial.println("key * pressed");
  }
  else if(digitalRead(col_2)==0)
  {
    Serial.println("Key 0 pressed");
  }
  else if(digitalRead(col_3)==0)
  {
    Serial.println("Key # pressed");
  }
  delay(100);
}
