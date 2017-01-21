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

int in = A0;            // resistencia 230 ohms
int ledPin = D1;        // resistencia 230 ohms    o a MOSFET
int motorPin = D2;         // a relay a motor


bool tuboUp = false;
int linetone=0;
int needToPrint = 0;
int count;
int lastState = LOW;
int trueState = LOW;
long lastStateChangeTime = 0;
int cleared = 0;
String fullnum = "";

const String objectivenum = "157";
bool done = false;


int truenoCounter = 0;
int aguaCounter = 0;


// constants

const int dialHasFinishedRotatingAfterMs = 100;
const int debounceDelay = 10;
const int hangPhoneTime = 500;
const int resetTime = 2000000;

const int tiempoAgua = 80;
const int tiempoTrueno = 100;

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

    server.on("/trueno", handleTrueno);

    server.on("/agua", handleAgua);

    server.onNotFound(handleNotFound);
    
    server.begin();
    Serial.println("HTTP server started");
    udpConnected = connectUDP();
    if (udpConnected){

      
//////// initialise pins

  
  pinMode(in, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(motorPin, OUTPUT);
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


  String message = "si telefono marca bien, manda WIN, numero entero mal FAIL, UP o DOWN levanta o baja tubo.";
  message += "\n\n Tambien prende el motor del gotero /agua y luz de truenos /trueno \n";
  message += controllerId;
  message += "\n\n metodos: \n";
  message += "/test /reset /manual /agua /trueno  \n\n" ;
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
done = false;
////
   
}


void handleManual()
{
/////////////////// VARIABLES TO CHANGE MANUALLY

 fullnum = objectivenum;
  
  
/////////////////// END
  
}


//////  HANDLE OTHER CALLS


void handleAgua()
{
   server.send(200, "text/plain", "tirar gotitas de agua");
   aguaCounter = tiempoAgua;
   Serial.print(aguaCounter);
}

void handleTrueno()
{
   server.send(200, "text/plain", "luz de trueno");
   truenoCounter = random(tiempoTrueno, tiempoTrueno*3);
   Serial.print(truenoCounter);
}


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
  int analogin = analogRead(in);
  int reading = 0;


  // passar a 3 opciones
  if(analogin > 850)
  {
      reading = HIGH;    // colgado o ticks
  }
  else if (analogin > 400)
  {
      reading = 2;   //  descolgado pasivo
  }
  else
  { 
      reading = LOW;    //en medio de discar
  }


  //Serial.println(reading);
  //digitalWrite(ledPin, reading);
  
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



  if (reading != lastState) 
  {
    lastStateChangeTime = millis();
  }
  
  if ((millis() - lastStateChangeTime) > debounceDelay) 
  {
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




   if ((millis() - lastStateChangeTime) > hangPhoneTime) 
   {
      if (reading == HIGH && tuboUp == true)
      {
      // no dialing for a while - number resets
            fullnum = "";
            UDP.beginPacket(pcRemoteHost, pcRemotePort);
            UDP.print("DOWN");
            UDP.endPacket();
            Serial.println("hanging phone");
            tuboUp = false;
      }
      else if (reading == 2 && tuboUp == false)
      {
         UDP.beginPacket(pcRemoteHost, pcRemotePort);
         UDP.print("UP");
         UDP.endPacket();
         Serial.println("phone up");
         fullnum = "";
         tuboUp = true;
      }
      
   }  


  
  
  if ((millis() - lastStateChangeTime) > resetTime) {
    // no dialing for a while - number resets
      if(fullnum.length() > 0)
        {
          fullnum = "";
          UDP.beginPacket(pcRemoteHost, pcRemotePort);
          UDP.print("FAIL");
          UDP.endPacket();
          Serial.println("reset number");
        }
   }  

  if (fullnum.equals(objectivenum) && done == false)
  {
      UDP.beginPacket(pcRemoteHost, pcRemotePort);
      UDP.print("WIN");
      UDP.endPacket();
      Serial.println("Got it!");
      digitalWrite(ledPin, HIGH);
      delay (500);
      digitalWrite(ledPin, LOW);
      done = true;  
    
   }
  else if (fullnum.length() > objectivenum.length())
  {
      UDP.beginPacket(pcRemoteHost, pcRemotePort);
      UDP.print("FAIL");
      UDP.endPacket();
      Serial.println("wrong number");
      handleReset();
   }



  lastState = reading;



  if (truenoCounter > 0)
  {
      truenoCounter -=1;
      int lit = random(1,3) - 1;
      digitalWrite(ledPin,lit);
      Serial.print(lit);
      if (truenoCounter == 0)
      {
        digitalWrite(ledPin,LOW);       
      }
  }


  if (aguaCounter > 0)
  {
     aguaCounter -=1;
     digitalWrite(motorPin,HIGH);
     if (aguaCounter == 0)
     {
        digitalWrite(motorPin,LOW);     
     }
  }




/////////////////// END
    
    }
  }
}


















