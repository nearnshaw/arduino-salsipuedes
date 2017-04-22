
#include <ESP8266WiFi.h>
#include <WiFiUDP.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#include "FastLED.h"

/////////////////// wifi connection variables

const char* ssid = "NICTOPIA";
const char* password = "queganasdejoder";
boolean wifiConnected = false;
const char* pcRemoteHost = "192.168.0.15";
const int pcRemotePort = 1307;
const int localPort = 1309;
const char* controllerId = "hab13velador";


IPAddress ip(192,168,0,59);
IPAddress gateway(192,168,11,1);
IPAddress subnet(255,0,0,0);//255,255,0); 
IPAddress dns(10,0,2,200);//(192,168,1,249);//(10,8,0,10);

/////////////////// END

/////////////////// VARIABLES

#define DATA_PIN   D2    // Digital IO pin connected to the NeoPixels.
#define CLOCK_PIN    D3    // Digital IO pin connected to the NeoPixels.

#define buzzPin    D1    // buzzer -  no resistance

#define NUM_LEDS 10

CRGB leds[NUM_LEDS];


int buzzCounter = 0;

const int tiempoBuzz = 75;

int pixelArray[NUM_LEDS];

bool isOn = true;

int bri = 255;

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

    server.on("/manual", handleManual);

    server.on("/buzz", handleBuzz);

    server.on("/on", handleOn);
    
    server.on("/off", handleOff);

    
    server.onNotFound(handleNotFound);

///////////////////     MORE URL OPTIONS


//more calls go here

   FastLED.addLeds<APA102, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
   FastLED.setBrightness(255);
   
/////////////////// END

    
    server.begin();
    Serial.println("HTTP server started");
    udpConnected = connectUDP();
    if (udpConnected){

      
///////////////////   initialise pins
    pinMode(buzzPin,OUTPUT);
  
/////////////////// END
      
    }
  }
}




// handle incomimg msg

void handleRoot() {
  digitalWrite(LED_BUILTIN, 1);
  delay(500);

/////////////////// DESCRIPTION 

  String message = " luz de velador /buzz  /on /off  \n\n";
  message += controllerId;
  message += "\n\n metodos: \n";
  message += "/test /reset /manual  \n\n" ;
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

    buzzCounter = 0;
    handleOn();
    
    int bri = 255;
    noTone(buzzPin);
    


/////////////////// END
   
}

void handleManual()
{
/////////////////// VARIABLES TO CHANGE MANUALLY

    server.send(200, "text/plain", "buzz manual");
    handleOn();  
  
/////////////////// END
  
}



///////////////////  HANDLE OTHER CALLS


void handleBuzz()
{

   server.send(200, "text/plain", "buzz velero");
   buzzCounter = random(tiempoBuzz, tiempoBuzz*2);
   Serial.print("buzz ");   
   Serial.print(buzzCounter);
   server.send(200, "text/plain", "buzz");  
}


void handleOn()
{

    isOn = true;
    server.send(200, "text/plain", "veladorOn");
    buzzCounter = random(tiempoBuzz/2, tiempoBuzz*2);
  
}

void handleOff()
{

  isOn = false;
  server.send(200, "text/plain", "veladorOff");
  buzzCounter = random(tiempoBuzz/4, tiempoBuzz);  
  
}


/////////////////// END





void loop() {
  
// check if the WiFi and UDP connections were successful
if(wifiConnected){
    server.handleClient();
    
    if(udpConnected){
      
      UDPRead();
      delay(15);


/////////////////// ACTUAL CODE
  
  if (buzzCounter > 1)
  {
      buzzCounter -=1;
      updateBuzz();
      Serial.print(buzzCounter);
  }
  else if (buzzCounter == 1)
  {
    buzzCounter -=1;
    
    if (isOn == true)
    {
      bri = 255;
    }
    else
    {
      bri = 0;
    }
    
    for(int b=0; b< NUM_LEDS; b++) 
    {
      leds[b] = CRGB(bri,bri,bri);
    }

    noTone(buzzPin);

  }

  FastLED.show();  
  
/////////////////// END
    
    }
  }
}

void updateBuzz()
{

    float lum = random(0,  sqrt(255)); 
    bri =  lum * lum;   //exponencial hasta 225  

     for(int j=0; j< NUM_LEDS; j++) 
     {
      leds[j] = CRGB(bri, bri, bri);
     }
   
    int del = random(0,10);
    del = del* del;

    if (bri > 30)
    {
        tone(buzzPin,70); 
    }
    else
    {
      noTone(buzzPin);
    }

    
    delay(del);

  
      
  
}





