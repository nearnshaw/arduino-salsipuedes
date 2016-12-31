#include <ESP8266WiFi.h>
#include <WiFiUDP.h>

// wifi connection variables

const char* ssid = "NICTOPIA";
const char* password = "queganasdejoder";
boolean wifiConnected = false;
const char* pcRemoteHost = "192.168.0.15";
const int pcRemotePort = 7016;

// UDP variables

unsigned int localPort = 7916;




const int led = D5;
const int magnet = D1;
int mag = 0;
boolean magnetState = false;




WiFiUDP UDP;
boolean udpConnected = false;
char packetBuffer[UDP_TX_PACKET_MAX_SIZE]; //buffer to hold incoming packet,
char ReplyBuffer[] = "acknowledged"; // a string to send back

void setup() {
  // Initialise Serial connection
  Serial.begin(115200);
  
  // Initialise wifi connection
  wifiConnected = connectWifi();
  
  // only proceed if wifi connection successful
  if(wifiConnected){
    udpConnected = connectUDP();
    if (udpConnected){
      // initialise pins
      pinMode(D5,OUTPUT);
      pinMode(D1,INPUT);
    }
  }
}

void loop() {
  
// check if the WiFi and UDP connections were successful
if(wifiConnected){
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


  // reads magnet and sends msg
  
      mag = digitalRead(magnet);
     // Serial.println(mag);
      if(mag == 1)
      {
        if (magnetState == false)
        {
        // send a reply, to the IP address and port that sent us the packet we received
        UDP.beginPacket(pcRemoteHost, pcRemotePort);
        UDP.write(99);
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
