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

const int led = D5;       //normal led w resistance
const int silla = D7;    //a makey makey D14, con 10K resistencia
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
  digitalWrite(led, 1);
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
  digitalWrite(led, 0);
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

bool sillaOn = false;  
bool sillaOnOld = false; 

////
   
}



//////  HANDLE OTHER CALLS


/////




void handleNotFound(){
  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(led, 0);
}












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


// read UDP incoming packages

void UDPRead()
{
     // if there’s data available, read a packet
      
      int packetSize = UDP.parsePacket();
      if(packetSize)
      {
        Serial.println("");
        Serial.print("Received packet of size ");
        Serial.println(packetSize);
        Serial.print("From ");
        IPAddress remote = UDP.remoteIP();
        for (int i =0; i < 4; i++)
        {
          Serial.print(remote[i], DEC);
          if (i < 3)
          {
            Serial.print(".");
          }
        }
        Serial.print(", port ");
        Serial.println(UDP.remotePort());
        
        // read the packet into packetBufffer
        UDP.read(packetBuffer,UDP_TX_PACKET_MAX_SIZE);
        Serial.println("Contents:");
        int value = packetBuffer[0]*256 + packetBuffer[1];
        Serial.println(value);
        
        Serial.print("Packet contents: [");
        Serial.print(packetBuffer[0], packetSize);
        Serial.println("]");
        
        // turn LED on or off depending on value recieved
        digitalWrite(D5,HIGH);
        delay(1000);
        digitalWrite(D5,LOW);        
      }
}


// connect to UDP – returns true if successful or false if not
boolean connectUDP(){
  boolean state = false;
  
  Serial.println("");
  Serial.println("Connecting to UDP”");
  
  if(UDP.begin(localPort) == 1){
    Serial.println("Connection successful");
    state = true;
  }
  else{
    Serial.println("Connection failed");
  }
  
  return state;
  }
// connect to wifi – returns true if successful or false if not
boolean connectWifi(){
  boolean state = true;
  int i = 0;
  WiFi.begin(ssid, password);
  Serial.println("");
  Serial.println("Connecting to WiFi");
  
  // Wait for connection
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (i > 10){
      state = false;
      break;
    }
    i++;
  }
  if (state){
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }
  else {
    Serial.println("");
    Serial.println("Connection failed.");
  }
  return state;
}
