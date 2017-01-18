//#include <ESP8266WiFi.h>
//#include <WiFiUDP.h>
//#include <ESP8266WebServer.h>
//#include <ESP8266mDNS.h>


#include <Bounce2.h>   // https://github.com/thomasfredericks/Bounce2


// do en regular arduino!


/////////////////// wifi connection variables

const char* ssid = "NICTOPIA";
const char* password = "queganasdejoder";
boolean wifiConnected = false;
const char* pcRemoteHost = "192.168.0.15";
const int pcRemotePort = 7016;
const int localPort = 7016;
const char* controllerId = "androMapa";


/////////////////// END

/////////////////// VARIABLES


const int leds[] = {28, 22, 24, 26} ;       //normal led w 230 resistance     con D4 hay un tema raro

#define buttonsPin1 23    //button w 1k / 10k resistance
#define buttonsPin2 25
#define buttonsPin3 27
#define buttonsPin4 29


Bounce buttons[4];

Bounce buttons0 = Bounce();
Bounce buttons1 = Bounce();
Bounce buttons2 = Bounce();
Bounce buttons3 = Bounce();

const int debounceInterval = 25;


const int pattern[4][4] = { 
  {LOW, HIGH, HIGH, HIGH},
  {HIGH, LOW, LOW, HIGH},
  {LOW, LOW, HIGH, HIGH},
  {LOW, HIGH, LOW, HIGH},
};


String incomingData = "";
char character;
int lastCharTime = 0;


/////////////////// END



//WiFiUDP UDP;
//boolean udpConnected = false;
//char packetBuffer[UDP_TX_PACKET_MAX_SIZE]; //buffer to hold incoming packet,
//char ReplyBuffer[] = "acknowledged"; // a string to send back
//
//ESP8266WebServer server(80);   //connecto HTTP in

void setup() {

  Serial.begin(115200);
  delay(1000);

      
///////////////////   initialise pins

      Serial1.begin(115200);


      pinMode(leds[0],OUTPUT);
      pinMode(leds[1],OUTPUT);
      pinMode(leds[2],OUTPUT);
      pinMode(leds[3],OUTPUT);
      
      pinMode(buttonsPin1,INPUT_PULLUP);
      pinMode(buttonsPin1,INPUT_PULLUP);
      pinMode(buttonsPin1,INPUT_PULLUP);
      pinMode(buttonsPin1,INPUT_PULLUP);

      buttons0.attach(buttonsPin1);
      buttons0.interval(debounceInterval);
      buttons1.attach(buttonsPin2);
      buttons1.interval(debounceInterval);
      buttons2.attach(buttonsPin3);
      buttons2.interval(debounceInterval);      
      buttons3.attach(buttonsPin4);            
      buttons3.interval(debounceInterval);
      
/////////////////// END
   
}



void handleTest()
{
//   server.send(200, "text/plain", controllerId);
   Serial.println("testing request");
   Serial1.println("ok");
}

void handleReset()
{ 
   Serial.println("doiing a reset");

///////////////////  VARIABLES TO RESET

  digitalWrite(leds[0],LOW);
  digitalWrite(leds[1],LOW);
  digitalWrite(leds[2],LOW);
  digitalWrite(leds[3],LOW);
  
  
 /////////////////// END
   
}



///////////////////  HANDLE OTHER CALLS


// other call functions go here


/////////////////// END





void loop() {
 

/////////////////// ACTUAL CODE

        buttons0.update();
        buttons1.update();  
        buttons2.update();
        buttons3.update();

        


        if(buttons0.rose())
        {
             Serial.println("pressed button");
              for(int j=0 ; j <4 ; j++)       // j = leds
              {
                int ledState = pattern[0][j];
                digitalWrite(leds[j], ledState);
                Serial.print(ledState);
                Serial1.print("button1");                
              }          
        }
        
        if(buttons1.rose())
        {
             Serial.println("pressed button");
              for(int j=0 ; j <4 ; j++)       // j = leds
              {
                int ledState = pattern[1][j];
                digitalWrite(leds[j], ledState);
                Serial.print(ledState);
                Serial1.print("button2");                
              }          
        }
        
        if(buttons2.rose())
        {
             Serial.println("pressed button");
              for(int j=0 ; j <4 ; j++)       // j = leds
              {
                int ledState = pattern[2][j];
                digitalWrite(leds[j], ledState);
                Serial.print(ledState);
                Serial1.print("button3");                
              }          
        }
              
        if(buttons3.rose())
        {
             Serial.println("pressed button");
              for(int j=0 ; j <4 ; j++)       // j = leds
              {
                int ledState = pattern[3][j];
                digitalWrite(leds[j], ledState);
                Serial.print(ledState);
                Serial1.print("button4");
                
              }          
        }


        //   messages from wemos
        while (Serial1.available()) 
        {
                // read the incoming byte:
                character = Serial1.read();
                incomingData = String(incomingData + character);
                lastCharTime = 0;
        }
        
        lastCharTime +=1; 
      
        
        if (incomingData != "" && lastCharTime > 25) 
        {
           // say what you got:
           Serial.print("I received: ");
           Serial.println(incomingData);
           incomingData.trim();   //remove spaces & enters

          if(incomingData.equals("reset"))
          {
            handleReset();
          }




           
           incomingData = "";
         }

        




/////////////////// END
    

}


