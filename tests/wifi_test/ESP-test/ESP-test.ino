#include <doxygen.h>
#include <ESP8266.h>

#include <doxygen.h>
#include <ESP8266.h>   //https://github.com/itead/ITEADLIB_Arduino_WeeESP8266


#include <SoftwareSerial.h>
SoftwareSerial BT1(12, 13); // RX | TX

void setup()
  {  Serial.begin(9600);
     BT1.begin(9600);
  }

void loop()
  {  String B= "." ;
     if (BT1.available())
         { char c = BT1.read() ;
           Serial.print(c);
         }
     if (Serial.available())
         {  char c = Serial.read();
            BT1.print(c);
         }
   }
