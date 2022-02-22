#include<SoftwareSerial.h>
int analog = A0;
const int buzzer =7; 
const int limit = 100;
int gasvalue;
SoftwareSerial espserial(4,5); //rx,tx
void setup() {
  Serial.begin(115200);
  espserial.begin(115200);
  pinMode(analog,INPUT);
  pinMode(buzzer,OUTPUT);
}

void loop() {
  gasvalue=analogRead(analog);
  Serial.print("Pin A0: ");
  Serial.println(gasvalue);
  espserial.write(gasvalue);
  if(gasvalue>limit)
{
  espserial.write(1);
  tone(buzzer, 2000);
  delay(500);
  noTone(buzzer);
  delay(500);
}
else
{
  espserial.write(2);
  noTone(buzzer);
}
}
