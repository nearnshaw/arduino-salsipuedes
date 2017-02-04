#include <ESP8266WiFi.h>
#include <WiFiUDP.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>


/////////////////// wifi connection variables

const char* ssid = "NICTOPIA";
const char* password = "queganasdejoder";
boolean wifiConnected = false;
const char* pcRemoteHost = "192.168.0.15";
const int pcRemotePort = 1307;
const int localPort = 1307;
const char* controllerId = "hab13trueno";


IPAddress ip(192,168,0,58);
IPAddress gateway(192,168,11,1);
IPAddress subnet(255,0,0,0);//255,255,0); 
IPAddress dns(10,0,2,200);//(192,168,1,249);//(10,8,0,10);

/////////////////// END

/////////////////// VARIABLES

int ledPin = D1;        // resistencia 230 ohms    o a MOSFET

int truenoCounter = 0;

const int tiempoTrueno = 100;


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

    server.on("/trueno", handleTrueno);

    server.onNotFound(handleNotFound);

///////////////////     MORE URL OPTIONS


//more calls go here


/////////////////// END

    
    server.begin();
    Serial.println("HTTP server started");
    udpConnected = connectUDP();
    if (udpConnected){

      
///////////////////   initialise pins

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  
/////////////////// END
      
    }
  }
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

digitalWrite(ledPin, LOW);
truenoCounter = 0;
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
   Serial.print(truenoCounter);
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


/////////////////// END
    
    }
  }
}


