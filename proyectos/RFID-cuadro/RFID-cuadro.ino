#include <ESP8266WiFi.h>
#include <WiFiUDP.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>


#include <SPI.h>
#include <MFRC522.h>


/*
 * --------------------------------------------------------------------------------------------------------------------
 * Example sketch/program showing how to read data from a PICC to serial.
 * --------------------------------------------------------------------------------------------------------------------

 * -----------------------------------------------------------------------------------------
 *             MFRC522      Wemos       Arduino   Arduino    Arduino          Arduino
 *             Reader/PCD   D1       Mega      Nano v3    Leonardo/Micro   Pro Micro
 * Signal      Pin          ESP8266           Pin       Pin        Pin              Pin
 * -----------------------------------------------------------------------------------------
 * RST/Reset   RST          D1           
 * SPI SS      SDA(SS)      D8            
 * SPI MOSI    MOSI         D7  
 * SPI MISO    MISO         D6   
 * SPI SCK     SCK          D5 
 * IRQ                      null
 * 
 *
 *
 *Laser                   D0
 *Buzzer                  D3
  */


/////////////////// wifi connection variables

const char* ssid = "NICTOPIA";
const char* password = "queganasdejoder";
boolean wifiConnected = false;
const char* pcRemoteHost = "192.168.0.15";
const int pcRemotePort = 1303;
const int localPort = 1303;
const char* controllerId = "hab13cuadro";

IPAddress ip(192,168,0,52);
IPAddress gateway(192,168,11,1);
IPAddress subnet(255,0,0,0); 
IPAddress dns(10,0,2,200);

/////////////////// END

/////////////////// VARIABLES

#define RST_PIN         D1          // Configurable, see typical pin layout above
#define SS_PIN          D8         // Configurable, see typical pin layout above

#define laser           D2
#define buzzPin         D3

bool validado = false;

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

// UID  95 6D D0 65
int byte0 = 149;
int byte1 = 109;
int byte2 = 208;
int byte3 = 101;

String currentCode = "";

/////////////////// END

WiFiUDP UDP;
boolean udpConnected = false;
char packetBuffer[UDP_TX_PACKET_MAX_SIZE]; //buffer to hold incoming packet,
char ReplyBuffer[] = "acknowledged"; // a string to send back

ESP8266WebServer server(80);   //connecto HTTP in



void setup() {
  Serial.begin(115200);
  delay(1000);
  SPI.begin();      // Init SPI bus
  pinMode(SS_PIN, OUTPUT);
  mfrc522.PCD_Init();   // Init MFRC522
  mfrc522.PCD_DumpVersionToSerial();  // Show details of PCD - MFRC522 Card Reader details
  Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));

  wifiConnected = connectWifi();
  // only proceed if wifi connection successful
  if(wifiConnected){
    
    server.on("/", handleRoot);
    
    server.on("/test", handleTest);
    
    server.on("/reset", handleReset);

    server.on("/manual", handleManual);

    server.onNotFound(handleNotFound);

///////////////////     MORE URL OPTIONS

/////////////////// END
    
    server.begin();
    Serial.println("HTTP server started");
    udpConnected = connectUDP();
    if (udpConnected){

      
///////////////////   initialise pins
 
    pinMode(buzzPin,OUTPUT);
    pinMode(laser, OUTPUT);
/////////////////// END
      
    }
  }
}




// handle incomimg msg

void handleRoot() {
  digitalWrite(LED_BUILTIN, 1);
  delay(500);

/////////////////// DESCRIPTION 

  String message = "detecta rfid y prende laser";
  message += controllerId;
  message += "\n\n metodos: \n";
  message += "/test /reset /manual - responde  WIN FAIL \n\n" ;
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

  digitalWrite(laser, LOW);
  validado = false;
  currentCode = "";

/////////////////// END
   
}


void handleManual()
{
/////////////////// VARIABLES TO CHANGE MANUALLY

  
  digitalWrite(laser, HIGH); 
  
  
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

  // Look for new cards
  if (validado == false)
  {
    if ( ! mfrc522.PICC_IsNewCardPresent()) 
    {
          return;    
    }
  
  
    // Select one of the cards
    if ( ! mfrc522.PICC_ReadCardSerial()) 
    {
      return;
    }


    currentCode = mfrc522.PICC_ReadCardSerial();
    Serial.println(currentCode);


    // Dump debug info about the card; PICC_HaltA() is automatically called
    mfrc522.PICC_DumpToSerial(&(mfrc522.uid));
  
    Serial.println(mfrc522.uid.uidByte[0]);
    Serial.println(mfrc522.uid.uidByte[1]);
    Serial.println(mfrc522.uid.uidByte[2]);
    Serial.println(mfrc522.uid.uidByte[3]);
  
  


    if ( mfrc522.uid.uidByte[0] == byte0 && 
         mfrc522.uid.uidByte[1] == byte1 &&
         mfrc522.uid.uidByte[2] == byte2 &&
         mfrc522.uid.uidByte[3] == byte3 )
       {  
  
          Serial.println("WIN");
          tone(buzzPin,70,600);
          digitalWrite(laser,HIGH);
          UDP.beginPacket(pcRemoteHost, pcRemotePort);
          UDP.print("WIN");
          UDP.endPacket(); 
  
          validado = true;
          delay(2000);
          return;
       }
       else
       {
         //
         Serial.println("FAIL");
         UDP.beginPacket(pcRemoteHost, pcRemotePort);
         UDP.print("FAIL");
         UDP.endPacket(); 
       }
    }  // if validado = false
/////////////////// END
    
    }
  }
}

