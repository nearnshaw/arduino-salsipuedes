#include <ESP8266WiFi.h>
#include <WiFiUDP.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>


// wifi connection variables

const char* ssid = "NICTOPIA";
const char* password = "queganasdejoder";
boolean wifiConnected = false;
const char* pcRemoteHost = "192.168.0.15";
const int pcRemotePort = 7017;
const int localPort = 7017;
const char* controllerId = "hab13tel";


// specific variables

int in = D1;            // resistencia 230 ohms
int ledPin = D5;        // resistencia 230 ohms
int buzzer = D3;

int linetone=0;
int needToPrint = 0;
int count;
int lastState = LOW;
int trueState = LOW;
long lastStateChangeTime = 0;
int cleared = 0;
String fullnum = "";

const String objectivenum = "157";


// constants

const int dialHasFinishedRotatingAfterMs = 100;
const int debounceDelay = 10;
const int hangPhoneTime = 500;
const int resetTime = 2000000;


WiFiUDP UDP;
boolean udpConnected = false;
char packetBuffer[UDP_TX_PACKET_MAX_SIZE]; //buffer to hold incoming packet,
char ReplyBuffer[] = "acknowledged"; // a string to send back

ESP8266WebServer server(80);   //connecto HTTP in


void setup()
{
  Serial.begin(115200);
  delay(1000);
  
  wifiConnected = connectWifi();
  // only proceed if wifi connection successful
  if(wifiConnected){
    
    server.on("/", handleRoot);
    
    server.on("/test", handleTest);
    
    server.on("/reset", handleReset);

    server.on("/manual", handleManual);

    server.onNotFound(handleNotFound);
    
    server.begin();
    Serial.println("HTTP server started");
    udpConnected = connectUDP();
    if (udpConnected){

      
//////// initialise pins

  
  pinMode(in, INPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

/////

  
    }
  }
}


// handle incomimg msg

void handleRoot() {
  digitalWrite(LED_BUILTIN, 1);
  delay(500);

/////// DESCRIPTION 


  String message = "si telefono marca bien, manda WIN";
  message += controllerId;
  message += "\n\n metodos: \n";
  message += "/test /reset  \n\n" ;
  message += "manda a puerto: \n";
  message += pcRemotePort ;
  message += "\n recibe en puerto: \n";
  message += localPort; 
  server.send(200, "text/plain", message);

  Serial.println("root request");
///////

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

////  VARIABLES TO RESET

linetone=0;
needToPrint = 0;
count = 0;
lastState = LOW;
trueState = LOW;
lastStateChangeTime = 0;
cleared = 0;
fullnum = "";
digitalWrite(ledPin, LOW);

////
   
}


void handleManual()
{
/////////////////// VARIABLES TO CHANGE MANUALLY

 fullnum = objectivenum;
  
  
/////////////////// END
  
}


//////  HANDLE OTHER CALLS


/////





void loop()
{
// check if the WiFi and UDP connections were successful
if(wifiConnected){
    server.handleClient();
    
    if(udpConnected){
      
      UDPRead();
      delay(15);


  ///////////// SPECIFIC CODE
  
  int reading = digitalRead(in);
  digitalWrite(ledPin, reading);
  
  if ((millis() - lastStateChangeTime) > dialHasFinishedRotatingAfterMs) {
    // the dial isn't being dialed, or has just finished being dialed.
    if (needToPrint) {
      // if it's only just finished being dialed, we need to send the number down the serial
      // line and reset the count. We mod the count by 10 because '0' will send 10 pulses.
      
      //Serial.println(count % 10, DEC);
      fullnum = fullnum + (count % 10);
      Serial.println(fullnum);
      needToPrint = 0;
      count = 0;
      cleared = 0;
    }
  } 



  if (reading != lastState) {
    lastStateChangeTime = millis();
  }
  if ((millis() - lastStateChangeTime) > debounceDelay) {
    // debounce - this happens once it's stablized
      if (reading != trueState) {
      // this means that the switch has either just gone from closed->open or vice versa.
      trueState = reading;
      if (trueState == HIGH) {
        // increment the count of pulses if it's gone high.
        count++; 
        needToPrint = 1; // we'll need to print this number (once the dial has finished rotating)
      } 
    }
  }

//   if (trueState = LOW &&  (millis() - lastStateChangeTime) > hangPhoneTime) {
//    // no dialing for a while - number resets
//      if(fullnum.length() > 0)
//        {
//          fullnum = "";
//      
//          Serial.println("hanging phone");
//        }
//   }  

  
  
  if ((millis() - lastStateChangeTime) > resetTime) {
    // no dialing for a while - number resets
      if(fullnum.length() > 0)
        {
          fullnum = "";
      
          Serial.println("reset number");
        }
   }  

  if (fullnum.equals(objectivenum))
  {
      Serial.println("Got it!");
      digitalWrite(ledPin, HIGH);
      delay (500);
      digitalWrite(ledPin, LOW);
      delay (500);
      digitalWrite(ledPin,  HIGH);
      delay (500);
      digitalWrite(ledPin, LOW);
      delay (500);
      digitalWrite(ledPin,  HIGH);
      delay (500);     
    
    }


  lastState = reading;


//linetone = analogRead(in);
//Serial.println(linetone);

/////////////////// END
    
    }
  }
}


















