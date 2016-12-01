/*
 * Author: SMRAZA KEEN
 * Date:2016/07/21
 * IDE V1.6.9
 * Address:http://www.amazon.com/shops/smraza
 * Function:
 */
const int relay=8;
void setup() {
  // put your setup code here, to run once:
  pinMode(relay,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
   digitalWrite(relay,HIGH);
   delay(2000);
   digitalWrite(relay,LOW);
   delay(2000);
}
