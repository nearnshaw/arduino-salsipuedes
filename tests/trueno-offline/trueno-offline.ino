
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
const int localPort = 1307;
const char* controllerId = "hab13trueno";


int counter = 0;
const int entreTruenos = 10000;


IPAddress ip(192,168,0,58);
IPAddress gateway(192,168,11,1);
IPAddress subnet(255,0,0,0);//255,255,0); 
IPAddress dns(10,0,2,200);//(192,168,1,249);//(10,8,0,10);

/////////////////// END

/////////////////// VARIABLES

#define DATA_PIN   D1    // Digital IO pin connected to the NeoPixels.
#define CLOCK_PIN    D2    // Digital IO pin connected to the NeoPixels.

#define NUM_LEDS 10

CRGB leds[NUM_LEDS];


int truenoCounter = 0;

const int tiempoTrueno = 10000;

int pixelArray[NUM_LEDS];


/////////////////// END



WiFiUDP UDP;
boolean udpConnected = false;
char packetBuffer[UDP_TX_PACKET_MAX_SIZE]; //buffer to hold incoming packet,
char ReplyBuffer[] = "acknowledged"; // a string to send back

ESP8266WebServer server(80);   //connecto HTTP in

void setup() {

  Serial.begin(115200);
  delay(1000);
 // wifiConnected = connectWifi();
  // only proceed if wifi connection successful
 // if(wifiConnected){
    
//    server.on("/", handleRoot);
//    
//    server.on("/test", handleTest);
//    
//    server.on("/reset", handleReset);
//
//    server.on("/manual", handleManual);
//
//    server.on("/trueno", handleTrueno);
//
//    server.onNotFound(handleNotFound);

///////////////////     MORE URL OPTIONS


//more calls go here

   FastLED.addLeds<APA102, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
   lightsOff();
/////////////////// END

    
    //server.begin();
    Serial.println("HTTP server started");
    //udpConnected = connectUDP();
    //if (udpConnected){

      
///////////////////   initialise pins

  
/////////////////// END
      
    //}
  //}
}




// handle incomimg msg

void handleRoot() {
  digitalWrite(LED_BUILTIN, 1);
  delay(500);

/////////////////// DESCRIPTION 

  String message = " luz de truenos /trueno  \n\n";
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

truenoCounter = 0;
lightsOff();

/////////////////// END
   
}

void handleManual()
{
/////////////////// VARIABLES TO CHANGE MANUALLY
truenoCounter = 1000;
  
//add here  
  
  
/////////////////// END
  
}



///////////////////  HANDLE OTHER CALLS


void handleTrueno()
{
   
   server.send(200, "text/plain", "luz de trueno");
   truenoCounter = random(tiempoTrueno, tiempoTrueno*3);
   Serial.print("trueno ");   
   Serial.print(truenoCounter);
   server.send(200, "text/plain", "trueno");  
   updateTrueno(); 
}

/////////////////// END





void loop() {
  
// check if the WiFi and UDP connections were successful

/////////////////// ACTUAL CODE
  
  if (truenoCounter > 0)
  {
      truenoCounter -=1;
      Serial.print(truenoCounter);
  }
  else
  {
    lightsOff();
  }

  counter += 1;
  if (counter > entreTruenos)
  { 
    Serial.print(counter);
    counter = 0;
    handleTrueno();
  }
  
  
/////////////////// END
    

}

void updateTrueno()
{

    int shapes = random(2,5);
    for(int s=0; s < shapes; s++) 
    {
  
      newShapeTrueno();
      ApplyShape();          
      FastLED.show();
      
      int repetitions = random(1,10);
      for(int r=0; r < repetitions; r++) 
      {
   
        FastLED.setBrightness(255);         
        FastLED.show();      
        delay(random(5,200));

        
        FastLED.setBrightness(random(5,100));
        FastLED.show();        
        delay(random(5,25));     

        FastLED.setBrightness(0);
        FastLED.show();       
        delay(random(10,250));
      
      }
   }
   lightsOff();
}

void newShapeTrueno()
{
   int lastPx = 0;
    for(int i=0; i< NUM_LEDS; i++) {

      // random y de fuerza exponencial
      
      float lum = random(0,  sqrt(255)); 
      pixelArray[i] =  lum * lum;   //exponencial hasta 225  

      // cambio gradual
       
      //int lumChange = random(0, 255 ) - 127;      
      //pixelArray[i] = abs( (lumChange + lastPx) % 255 );
      //lastPx = pixelArray[i];
    }
}

void ApplyShape()
{
     for(int j=0; j< NUM_LEDS; j++) 
     {
      leds[j] = CRGB(pixelArray[j], pixelArray[j], pixelArray[j]);
     }
}


void lightsOff()
{
        for(int b=0; b< NUM_LEDS; b++) 
        {
          leds[b] = CRGB(0,0,0);
        }
        FastLED.show();
}


