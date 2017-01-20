#include <ESP8266WiFi.h>
#include <WiFiUDP.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>


// wifi connection variables

const char* ssid = "NICTOPIA";
const char* password = "queganasdejoder";
boolean wifiConnected = false;
const char* pcRemoteHost = "192.168.0.15";
const int pcRemotePort = 7016;
const int localPort = 7016;



//relay pins
const int R1 = D1;
const int R2 = D2;
const int R3 = D7;
const int R4 = D8;

bool R1state = false;
bool R2state = false;
bool R3state = false;
bool R4state = false;



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

    server.on("/r1", handleR1);

    server.on("/r2", handleR2);

    server.on("/r3", handleR3);

    server.on("/r4", handleR4);

    
    server.onNotFound(handleNotFound);
    
    server.begin();
    Serial.println("HTTP server started");
    udpConnected = connectUDP();
    if (udpConnected){

      
      // initialise pins
      pinMode(D5,OUTPUT);
      pinMode(D6,OUTPUT);
      pinMode(D7,OUTPUT);
      pinMode(D8,OUTPUT);
                        

      digitalWrite(D5,LOW);
      digitalWrite(D6,LOW);
      digitalWrite(D7,LOW);
      digitalWrite(D8,LOW);
                        

      
    }
  }
}




// HANDLE INCOMING MSG

void handleRoot() {
  delay(500);


  String message = "Prende 4 relays   ...ojo al iniciar con current al wemos  \n\n"; 
  message += controllerId;
  message += "\n\n metodos: \n";
  message += "/test /reset /r1 /r2 /r3 /r4  \n\n" ;
  message += "manda a puerto: \n";
  message += pcRemotePort ;
  message += "\n recibe en puerto: \n";
  message += localPort; 
  server.send(200, "text/plain", message);
  
  Serial.println("root request");

}

void handleTest()
{
   server.send(200, "text/plain", "relays1");
   Serial.println("testing request");
}


void handleReset()
{ 
   server.send(200, "text/plain", "reset");
   Serial.println("reset");

////  VARIABLES TO RESET

 

////
   
}



void handleR1()
{
   if (R1state)
    {
      digitalWrite(R1, LOW);
      R1state = false;
      server.send(200, "text/plain", "relay 1 on");
      Serial.println("relay1 on");
    }
    else
    {
      digitalWrite(R1, HIGH); 
      R1state = true;   
      Serial.println("relay1 off");
      server.send(200, "text/plain", "relay 1 off");
    }
}


void handleR2()
{
   if (R2state)
    {
      digitalWrite(R2, LOW);
      R2state = false;
      Serial.println("relay2 on");
    }
    else
    {
      digitalWrite(R2, HIGH); 
      R2state = true;   
      Serial.println("relay2 off");
      server.send(200, "text/plain", "relay 2 off");
    }
}

void handleR3()
{
   if (R3state)
    {
      digitalWrite(R3, LOW);
      R3state = false;
      server.send(200, "text/plain", "relay 3 on");
      Serial.println("relay3 on");
    }
    else
    {
      digitalWrite(R3, HIGH); 
      R3state = true;   
      server.send(200, "text/plain", "relay 3 off");
      Serial.println("relay3 off");
    }
}


void handleR4()
{
   if (R4state)
    {
      digitalWrite(R4, LOW);
      R4state = false;
      server.send(200, "text/plain", "relay 4 on");
      Serial.println("relay4 on");
    }
    else
    {
      digitalWrite(R4, HIGH); 
      R4state = true;  
      server.send(200, "text/plain", "relay 4 off");
      Serial.println("relay4 off"); 
    }
}





void handleNotFound(){

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

}












void loop() {
  
// check if the WiFi and UDP connections were successful
if(wifiConnected){
    server.handleClient();
    
    if(udpConnected){
    
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
      delay(15);


    
    }
  
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
