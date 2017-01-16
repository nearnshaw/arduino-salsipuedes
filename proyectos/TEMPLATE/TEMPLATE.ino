#include <ESP8266WiFi.h>
#include <WiFiUDP.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>


/////////////////// wifi connection variables

const char* ssid = "NICTOPIA";
const char* password = "queganasdejoder";
boolean wifiConnected = false;
const char* pcRemoteHost = "192.168.0.15";
const int pcRemotePort = 7016;
const int localPort = 7016;
const char* controllerId = "hab13tel";


/////////////////// END

/////////////////// VARIABLES

const int led = D5;       //normal led w resistance
const int magnet = D1;    //reed sensor (magnet) on the digital out pin

int mag = 0;
boolean magnetState = false;


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

///////////////////  VARIABLES TO RESET

  int mag = 0;
  boolean magnetState = false;

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
  
      mag = digitalRead(magnet);
     // Serial.println(mag);
      if(mag == 1)
      {
        if (magnetState == false)
        {
        // send a reply, to the IP address and port that sent us the packet we received
        UDP.beginPacket(pcRemoteHost, pcRemotePort);
        //UDP.write(5);   // write returns nonsense
        UDP.print("puto");
        UDP.print(123);
        UDP.endPacket();
        Serial.print("request sent, response:");          
        delay(1000);
        magnetState = true;    
      }
    }
    else
    {
       magnetState = false;
    }


/////////////////// END
    
    }
  }
}


