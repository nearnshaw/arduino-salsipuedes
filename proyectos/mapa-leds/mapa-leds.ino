#include <ESP8266WiFi.h>
#include <WiFiUDP.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>


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


const int leds[] = { D0, D5, D6, D4} ;       //normal led w 230 resistance     con D4 hay un tema raro

#define buttonsPin1 D1    //button w 1k / 10k resistance
#define buttonsPin2 D2
#define buttonsPin3 D3
#define buttonsPin4 D7


Bounce buttons[4];

Bounce buttons0 = Bounce();
Bounce buttons1 = Bounce();
Bounce buttons2 = Bounce();
Bounce buttons3 = Bounce();



const int pattern[4][4] = { 
  {LOW, HIGH, HIGH, HIGH},
  {HIGH, LOW, LOW, HIGH},
  {LOW, LOW, HIGH, HIGH},
  {LOW, HIGH, LOW, HIGH},
};

/////////////////// END



WiFiUDP UDP;
boolean udpConnected = false;
char packetBuffer[UDP_TX_PACKET_MAX_SIZE]; //buffer to hold incoming packet,
char ReplyBuffer[] = "acknowledged"; // a string to send back

ESP8266WebServer server(80);   //connecto HTTP in

void setup() {

  Serial.begin(115200);
  delay(1000);
  wifiConnected = connectWifi();
  // only proceed if wifi connection successful
  if(wifiConnected){
    
    server.on("/", handleRoot);
    
    server.on("/test", handleTest);
    
    server.on("/reset", handleReset);

    server.onNotFound(handleNotFound);

///////////////////     MORE URL OPTIONS


//more calls go here


/////////////////// END

    
    server.begin();
    Serial.println("HTTP server started");
    udpConnected = connectUDP();
    if (udpConnected){

      
///////////////////   initialise pins

      pinMode(leds[0],OUTPUT);
      pinMode(leds[1],OUTPUT);
      pinMode(leds[2],OUTPUT);
      pinMode(leds[3],OUTPUT);
      
      pinMode(buttonsPin1,INPUT_PULLUP);
      pinMode(buttonsPin1,INPUT_PULLUP);
      pinMode(buttonsPin1,INPUT_PULLUP);
      pinMode(buttonsPin1,INPUT_PULLUP);

      buttons0.attach(buttonsPin1);
      buttons0.interval(15);
      buttons1.attach(buttonsPin2);
      buttons1.interval(15);
      buttons2.attach(buttonsPin3);
      buttons2.interval(15);      
      buttons3.attach(buttonsPin4);            
      buttons3.interval(15);
      
/////////////////// END
      
    }
  }
}




// handle incomimg msg

void handleRoot() {
  digitalWrite(LED_BUILTIN, 1);
  delay(500);

/////////////////// DESCRIPTION 

  String message = "botones prenden luces en mapa\n\n";
  message += controllerId;
  message += "\n\n metodos: \n";
  message += "/test /reset  \n\n" ;
  message += "manda a puerto: \n";
  message += pcRemotePort ;
  message += "\n recibe en puerto: \n";
  message += localPort; 
  server.send(200, "text/plain", message);

  Serial.println("root request");
  
/////////////////// END

  Serial.println("root request");
  digitalWrite(LED_BUILTIN, 0);
}

void handleTest()
{
   server.send(200, "text/plain", controllerId);
   Serial.println("testing request");
}

void handleReset()
{ 
   server.send(200, "text/plain", "reset");
   Serial.println("reset");

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
  
// check if the WiFi and UDP connections were successful
if(wifiConnected){
    server.handleClient();
    
    if(udpConnected){
      
      UDPRead();
      delay(15);


/////////////////// ACTUAL CODE

        int value[4];
        buttons0.update();
        buttons1.update();  
        buttons2.update();
        buttons3.update();

        
//        value[0] =  buttons0.read();
//        value[1] =  buttons1.read();
//        value[2] =  buttons2.read();
//        value[3] =  buttons3.read();
//
//
//        for (int i=0; i<4; i++)
//        {
//          if (value[i] == HIGH)
//          {
//              Serial.println("pressed button");
//              Serial.println(i);
//              for(int j=0 ; j <4 ; j++)       // j = leds
//              {
//                int ledState = pattern[i][j];
//                digitalWrite(leds[j], ledState);
//                Serial.print(ledState);
//                
//              }
//          }
//
//        }

        if(buttons0.rose())
        {
             Serial.println("pressed button");
              for(int j=0 ; j <4 ; j++)       // j = leds
              {
                int ledState = pattern[0][j];
                digitalWrite(leds[j], ledState);
                Serial.print(ledState);
                
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
                
              }          
        }


/////////////////// END
    
    }
  }
}


