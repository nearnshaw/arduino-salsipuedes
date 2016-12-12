/*
 * Author: SMRAZA KEEN
 * Date:2016/07/21
 * IDE V1.6.9
 * Address:http://www.amazon.com/shops/smraza
 * Function:
 */
const int RGBled=8;
void setup() 
{
  pinMode(RGBled,OUTPUT);
}
void loop() 
{
  digitalWrite(RGBled,HIGH);
  delay(10000);
  digitalWrite(RGBled,LOW);
  delay(1000);
}
