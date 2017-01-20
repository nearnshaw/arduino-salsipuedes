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
const char* controllerId = "hab13silla";


// specific variables

const int led = D2;       //normal led w resistance
const int silla = D1;    //a makey makey D14, con 10K resistencia
bool sillaOn = false;    //ya - hace contacto o no
bool sillaOnOld = false;     //ultima reportada - hace contacto o no
int offCounter = 0;          // contador, cuantos ciclos desde que se apago


WiFiUDP UDP;
boolean udpConnected = false;
char packetBuffer[UDP_TX_PACKET_MAX_SIZE]; //buffer to hold incoming packet,
char ReplyBuffer[] = "acknowledged"; // a string to send back

ESP8266WebServer server(80);   //connecto HTTP in

void setup() {
  // Initialise Serial connection
  Serial.begin(115200);
  // Initialise wifi connection
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

      pinMode(led,OUTPUT);
      pinMode(silla,INPUT);

/////
      
    }
  }
}




// handle incomimg msg

void handleRoot() {
  digitalWrite(LED_BUILTIN, 1);
  delay(500);

/////// DESCRIPTION 


  String message = "Si hace contacto en la silla, manda msg a 7016\n\n";
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

sillaOn = false;  
sillaOnOld = false; 

////
   
}


void handleManual()
{
/////////////////// VARIABLES TO CHANGE MANUALLY

  sillaOn = true;
  sillaOnOld = false;
  offCounter = 0;
  digitalWrite(led, HIGH);
  
  
/////////////////// END
  
}


//////  HANDLE OTHER CALLS


/////



void loop() {
  
// check if the WiFi and UDP connections were successful
if(wifiConnected){
    server.handleClient();
    
    if(udpConnected){
      
      UDPRead();
      delay(15);


  ///////////// SPECIFIC CODE
  
      sillaOn = digitalRead(silla);

      
     // Serial.println(mag);
      if(sillaOn == HIGH)
      {
        if( sillaOnOld == LOW)    // se acaba de prender
        {
            UDP.beginPacket(pcRemoteHost, pcRemotePort);
            //UDP.write(5);   // write returns nonsense
            UDP.print("silla on");
            UDP.endPacket();
            Serial.println("silla se acaba de prender");          

           sillaOnOld = HIGH;
           delay(500);
        }
        offCounter = 0;
      }
      else 
      {
          offCounter += 1;
          sillaOnOld = LOW;  
                  
       }
      
      //Serial.println(offCounter);   
      
      if (offCounter == 50)   // se acaba de apagar
      {

          UDP.beginPacket(pcRemoteHost, pcRemotePort);
          //UDP.write(5);   // write returns nonsense
          UDP.print("silla off");
          UDP.endPacket();
          Serial.println("silla se acaba de apagar");          

      }



  //////////
    
    }
  }
}

