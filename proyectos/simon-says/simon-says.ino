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
const char* controllerId = "hab13simon";





// specific configurable variables

const int leds[] = { D0, D5, D6, D7} ;       //normal led w 230 resistance

const int buttons[] = {D1, D2, D3, D4};       //button w 1k / 10k resistance

const int buzzer = D8;       //buzzer

const int pitches[] = {20, 25, 30, 35};    //NOTE_C4,  NOTE_A4   ...etc

const unsigned long debounceDelay = 75;    // the debounce time; increase if the output flickers



const int pattern[][5] = { 
  {0, 1, 1, 1, 1},
  {3, 2, 1, 3, 2},
  {1, 3, 2, 1, 2},
  {2, 2, 0, 2 ,2},
};

const int noteDuration = 1;   //maybe it can speed up over time,  remove const then


// specific program variables

bool simonMode = true;

int currentPattern[] = {9,9,9,9,9};

int patternIndex = 1;

int currentLevel = 1;

int ledState[] = { LOW ,LOW ,LOW ,LOW } ;

int buttonReading[] = { LOW ,LOW ,LOW ,LOW } ;

int buttonState[] = { LOW ,LOW ,LOW ,LOW } ;

int lastButtonState[] = { LOW ,LOW ,LOW ,LOW } ;

unsigned long lastDebounceTime[] = { 0, 0, 0, 0};  // the last time the output pin was toggled



//const int buzzer =  ;




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

      pinMode(leds[0],OUTPUT);
      pinMode(leds[1],OUTPUT);
      pinMode(leds[2],OUTPUT);
      pinMode(leds[3],OUTPUT);
      
      pinMode(buttons[0],INPUT);
      pinMode(buttons[1],INPUT);
      pinMode(buttons[2],INPUT);
      pinMode(buttons[3],INPUT);

/////
      
    }
  }
}




// handle incomimg msg

void handleRoot() {
  digitalWrite(LED_BUILTIN, 1);
  delay(500);

/////// DESCRIPTION 

  String message = "simon says\n\n";
  message += controllerId;
  message += "\n\n metodos: \n";
  message += "/test /reset  \n\n" ;
  message += "manda a puerto: \n";
  message += pcRemotePort ;
  message += "\n recibe en puerto: \n";
  message += localPort; 
  server.send(200, "text/plain", message);

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
   String ids = String(controllerId);
   String rmessage =  "reset " +  ids;
   server.send(200, "text/plain", rmessage);
   Serial.println("reset");

////  VARIABLES TO RESET

    simonMode = true;

    for (int m = 0; m <4; m++)
    {         
        
        ledState[m] = LOW;
        
        buttonReading[m] = LOW ;
        
        buttonState[m] = LOW ;
        
        lastButtonState[m] = LOW ;
        
        lastDebounceTime[m] = 0 ;

    }

    for (int p = 0; p < sizeof(currentPattern); p++)
    {     
        currentPattern[p] =9 ;
    }    
        simonMode = true;
        
        currentLevel = 1;

        patternIndex = 1;

    
    // also reset millis()  somehow

////
   
}



//////  HANDLE OTHER CALLS


/////




void handleNotFound(){
  digitalWrite(LED_BUILTIN, 1);
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
  digitalWrite(LED_BUILTIN, 0);
}












void loop() {
  
// check if the WiFi and UDP connections were successful
if(wifiConnected){
    server.handleClient();
    
    if(udpConnected){
      
      UDPRead();
      delay(15);


  ///////////// SPECIFIC CODE

  // show pattern

  
  if (simonMode == true)
  {

      for (int thisNote = 0; thisNote < patternIndex; thisNote++) 
      {
      
//        tone(buzzer, pitches[pattern[thisNote]], noteDuration);
        //led
        Serial.print(pattern[currentLevel][thisNote]);
        delay(noteDuration);
      }

      delay(500);
      simonMode = false;
      Serial.println("finished melody");    

      buttonState[0] = LOW;
      buttonState[1] = LOW;
      buttonState[2] = LOW;
      buttonState[3] = LOW;  

      digitalWrite(leds[0], LOW);
      digitalWrite(leds[1], LOW);
      digitalWrite(leds[2], LOW);
      digitalWrite(leds[3], LOW);           
  }



  /// press buttons


 for (int n = 0; n < 3; n++)
 { 
    buttonReading[n] = digitalRead(buttons[n]);
 
    if (buttonReading[n] != buttonState[n]) {
      // reset the debouncing timer
      lastDebounceTime[n] = millis();
    }
     
    if (((millis() - lastDebounceTime[n]) > debounceDelay)&& (buttonReading[n] != buttonState[n]))
    {

          lastButtonState[n] = buttonState[n];
          buttonState[n] = buttonReading[n];
          digitalWrite(leds[n], buttonState[n]);
          if (buttonState[n] == HIGH)
          {
            Serial.println("button pressed");
            Serial.println(n);
            //tone(buzzer, pitches[n], 1);
            if ( n == pattern[currentLevel][patternIndex])
            {
              currentPattern[patternIndex]= n;
              patternIndex += 1;
              simonMode = true;
              Serial.print("correct!");
              if (patternIndex + 1 == sizeof(pattern[currentLevel]))
              {
                  Serial.print("level won!");
                  Serial.println(currentLevel);
                  currentLevel += 1;
                  UDP.beginPacket(pcRemoteHost, pcRemotePort);
                  UDP.print("LEVELUP");
                  UDP.endPacket();
              }
            }
            else
            {
               //reset
               Serial.println("FAIL");
               UDP.beginPacket(pcRemoteHost, pcRemotePort);
               UDP.print("FAIL");
               UDP.endPacket();
             //  handleReset();
            }
            
          }
    }

   
    lastButtonState[n] = buttonReading[n];
 }  

   /// end press buttons
 
  ////////////////////////////////////
    
      
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
