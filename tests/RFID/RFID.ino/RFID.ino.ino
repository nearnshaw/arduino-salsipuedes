int servoPin = 9;
int buzzerPin = 8;
int bledPin = 2;
int rledPin = 3;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  delay(1000);
  pinMode(servoPin,OUTPUT);
  pinMode(buzzerPin,OUTPUT);
  pinMode(bledPin,OUTPUT);
  pinMode(rledPin,OUTPUT);


}

void loop() {
  // put your main code here, to run repeatedly:
    digitalWrite(servoPin,HIGH);
      delay(500);
    digitalWrite(servoPin,LOW);
    tone(buzzerPin,400);
         delay(500);
    tone(buzzerPin,0);
    digitalWrite(bledPin,HIGH);
         delay(500);
    digitalWrite(bledPin,LOW);
    digitalWrite(rledPin,HIGH);
         delay(500);
    digitalWrite(rledPin,LOW);
   
}
