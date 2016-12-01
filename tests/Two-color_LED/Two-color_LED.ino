/*
 * Author: SMRAZA KEEN
 * Date:2016/07/21
 * IDE V1.6.9
 * Address:http://www.amazon.com/shops/smraza
 * Function:
 */
const int Rled=7;
const int Yled=8;
void setup() 
{
  pinMode(Rled,OUTPUT);
  pinMode(Yled,OUTPUT);
}
void loop() 
{
  digitalWrite(Rled,HIGH);
  digitalWrite(Yled,LOW);
  delay(1000);
  digitalWrite(Rled,LOW);
  digitalWrite(Yled,HIGH);
  delay(1000);
}
