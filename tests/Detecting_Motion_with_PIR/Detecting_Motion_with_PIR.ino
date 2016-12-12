#include <Servo.h>

/*
 * Author: SMRAZA KEEN
 * Date:2016/6/29
 * IDE V1.6.9
 * Email:TechnicSmraza@outlook.com
 * Function:
 */
const int pirPin = 3; 
const int LED = 4;
const int LED2 = 7;
const int buzzerPin = 8;
Servo myservo;
int pos = 0;



void setup()
{
 Serial.begin(9600); 
 pinMode(pirPin, INPUT);
 pinMode(LED,OUTPUT);
 myservo.attach(10);
}
void loop()
{



  
  int pirVal = digitalRead(pirPin);
  
  if (pirVal == HIGH)
  { //was motion detected
    Serial.println("Motion Detected"); 
    digitalWrite(LED,HIGH);
    digitalWrite(LED2,LOW);
    tone(buzzerPin,200,500);

      if (pos <=180)
    {
      pos += 30;
      myservo.write(pos);
     
    }
    else
    {
      pos = 0;
      myservo.write(pos);
    }
  
    
  } else
  {
    digitalWrite(LED,LOW);
    digitalWrite(LED2,HIGH);
    Serial.println("Still");
 
  }
    delay(1000); 
}
