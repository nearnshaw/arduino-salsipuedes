#include <ESP8266WiFi.h>
#include <WiFiUDP.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>


/////////////////// wifi connection variables

const char* ssid = "NICTOPIA";
const char* password = "queganasdejoder";
boolean wifiConnected = false;
const char* pcRemoteHost = "192.168.0.15";
const int pcRemotePort = 1304;
const int localPort = 1304;
const char* controllerId = "hab13maquina";


IPAddress ip(192,168,0,53);
IPAddress gateway(192,168,11,1);
IPAddress subnet(255,0,0,0); 
IPAddress dns(10,0,2,200);

/////////////////// END

/////////////////// VARIABLES


String incomingData = "";
char character;
int lastCharTime = 0;

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

    server.on("/on", handleOn);

    server.on("/off", handleOff);

    server.on("/son", handleSOn);

    server.on("/soff", handleSOff);

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

      pinMode(D5,OUTPUT);
      pinMode(D1,INPUT);

/////////////////// END
      
    }
  }
}




// handle incomimg msg

void handleRoot() {
  digitalWrite(LED_BUILTIN, 1);
  delay(500);

/////////////////// DESCRIPTION 

  String message = "detecta iman\n\n";
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
   delay(1000);
   
///////////////////  VARIABLES TO RESET
   server.send(200, "text/plain", "OFF");
   ESP.restart();

/////////////////// END   
}


void handleOn()
{ 
   server.send(200, "text/plain", "on");
   Serial.println("on"); 
}

void handleSOn()
{ 
   server.send(200, "text/plain", "son");
   Serial.println("son"); 
}

void handleOff()
{ 
   server.send(200, "text/plain", "off");
   Serial.println("off"); 
}

void handleSOff()
{ 
   server.send(200, "text/plain", "soff");
   Serial.println("soff"); 
}

void handleTrueno()
{ 
   server.send(200, "text/plain", "trueno");
   Serial.println("trueno"); 
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


        while (Serial.available()) 
        {
                // read the incoming byte:
                character = Serial.read();
                incomingData = String(incomingData + character);
                lastCharTime = 0;
        }
        
        lastCharTime +=1; 
      
        
        if (incomingData != "" && lastCharTime > 25) 
        {
              incomingData.trim();   //remove spaces & enters
              
              UDP.beginPacket(pcRemoteHost, pcRemotePort);
              UDP.print(incomingData);
              UDP.endPacket();

              incomingData = "";
        }    
 

/////////////////// END
    
    }
  }
}


