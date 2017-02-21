#include <ESP8266WiFi.h>
#include <WiFiUDP.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>




/////////////////// wifi connection variables

const char* ssid = "NICTOPIA";
const char* password = "queganasdejoder";
boolean wifiConnected = false;
const char* pcRemoteHost = "192.168.0.15";
const int pcRemotePort = 1310;
const int localPort = 1310;
const char* controllerId = "hab13cae";

IPAddress ip(192,168,0,59);
IPAddress gateway(192,168,11,1);
IPAddress subnet(255,0,0,0); 
IPAddress dns(10,0,2,200);

/////////////////// END

/////////////////// VARIABLES


#define solenoid           D1

bool falling = false;
bool fallen = false;
bool adentro = false;
const int espera = 25 ;   // ms between changes
const int repeticiones = 3; // sacudidas
int currentRepeticiones = 0;

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

    server.on("/tirar", handleManual);
    

    server.onNotFound(handleNotFound);

///////////////////     MORE URL OPTIONS


/////////////////// END
    
    server.begin();
    Serial.println("HTTP server started");
    udpConnected = connectUDP();
    if (udpConnected){

      
///////////////////   initialise pins
 
  pinMode(solenoid,OUTPUT);

/////////////////// END
      
    }
  }
}




// handle incomimg msg

void handleRoot() {
  digitalWrite(LED_BUILTIN, 1);
  delay(500);

/////////////////// DESCRIPTION 

  String message = "hace caer cuadro con /tirar ";
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

falling = false;
fallen = false;
adentro = false;
digitalWrite(solenoid, LOW);
currentRepeticiones = 0;


/////////////////// END
   
}


void handleManual()
{
/////////////////// VARIABLES TO CHANGE MANUALLY

  falling = true;
  adentro = false;
  digitalWrite(solenoid, LOW);
  currentRepeticiones = repeticiones;
/////////////////// END
  
}


















void loop() {
// check if the WiFi and UDP connections were successful
if(wifiConnected){
    server.handleClient();
    
    if(udpConnected){
      
      UDPRead();
      delay(15);


/////////////////// ACTUAL CODE


    if (falling == true)
    {

      if (adentro == false)
      {
        digitalWrite(solenoid, HIGH);
        adentro = true;
        delay(espera);  
      }
      else    // adentro == true
      {
        digitalWrite(solenoid, LOW);
        adentro = false;
        currentRepeticiones -=1;
        delay(espera);  
      }

      if (currentRepeticiones == 0)
      {
        handleReset();
        fallen == true;
      }

      
    }

/////////////////// END
    
    }
  }
}

