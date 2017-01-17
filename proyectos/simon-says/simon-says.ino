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

const int pitches[] = {30, 40, 50, 60};    //NOTE_C4,  NOTE_A4   ...etc

const unsigned long debounceDelay = 75;    // the debounce time; increase if the output flickers



const int pattern[][5] = { 
  {0, 1, 1, 1, 1},
  {2, 2, 1, 3, 2},
  {1, 3, 2, 1, 2},
  {2, 2, 0, 2 ,2},
};

const int noteDuration = 250;   //maybe it can speed up over time,  remove const then


// specific program variables

bool simonMode = true;

int currentPattern[] = {9,9,9,9,9};

int patternIndex = 0;

int currentLevel = 0;

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

   server.send(200, "text/plain", "reset");
   Serial.println("reset");

////  VARIABLES TO RESET

    for (int m = 0; m <4; m++)
    {         
        
        ledState[m] = LOW;
        
        buttonReading[m] = LOW ;
        
        buttonState[m] = LOW ;
        
        lastButtonState[m] = LOW ;
        
        lastDebounceTime[m] = 0 ;

    }

    for (int p = 0; p < 5; p++)   // p < sizeof(currentPattern)
    {     
        currentPattern[p] =9 ;
    }    
        simonMode = true;
        
        currentLevel = 0;

        patternIndex = 0;

    
    // also reset millis()  somehow

////
   
}



//////  HANDLE OTHER CALLS

void handleLevelUp()
{


    for (int m = 0; m <4; m++)
    {         
        
        ledState[m] = LOW;
        
        buttonReading[m] = LOW ;
        
        buttonState[m] = LOW ;
        
        lastButtonState[m] = LOW ;
        
        lastDebounceTime[m] = 0 ;

    }

    for (int p = 0; p < 5; p++)   // p < sizeof(currentPattern)
    {     
        currentPattern[p] =9 ;
    }    
        simonMode = true;

        patternIndex = 0;

}

/////




void showPattern()
{

   // show pattern
      int cual = 0;
      for (int thisNote = 0; thisNote < patternIndex+1; thisNote++) 
      { 
        cual = pattern[currentLevel][thisNote];
        digitalWrite(ledState[cual],HIGH);
        delay(noteDuration);
        
        tone(buzzer, pitches[cual], noteDuration);

        digitalWrite(ledState[cual],LOW);
        Serial.println(pattern[currentLevel][thisNote]);
        delay(noteDuration);
      }

      delay(1000);



      buttonState[0] = LOW;
      buttonState[1] = LOW;
      buttonState[2] = LOW;
      buttonState[3] = LOW;  

      digitalWrite(leds[0], LOW);
      digitalWrite(leds[1], LOW);
      digitalWrite(leds[2], LOW);
      digitalWrite(leds[3], LOW);           
  
      simonMode = false;
       Serial.println("finished melody");    
}







void loop() {
  
// check if the WiFi and UDP connections were successful
if(wifiConnected){
    server.handleClient();
    
    if(udpConnected){
      
      UDPRead();
      delay(15);


  ///////////// SPECIFIC CODE

 

  if (simonMode == true)
  {
    showPattern();
    Serial.print("showing pattern");
  }

  /// press buttons


 for (int n = 0; n < 3; n++)
 { 
    buttonReading[n] = digitalRead(buttons[n]);
 
    if (buttonReading[n] != lastButtonState[n]) {
      // reset the debouncing timer
      lastDebounceTime[n] = millis();
    }
     
    if (((millis() - lastDebounceTime[n]) > debounceDelay) && buttonReading[n] == HIGH && buttonState[n] == LOW)
    {


          buttonState[n] = buttonReading[n];
          digitalWrite(leds[n], buttonState[n]);
          lastDebounceTime[n] = millis();
          if (buttonState[n] == HIGH)
          {
            Serial.println("button pressed");
            Serial.println(n);
            
            String debugtext = "state: ";
            debugtext += buttonState[n];
            debugtext +=  " reading: ";
            debugtext +=  buttonReading[n];
            debugtext += " laststate: ";
            debugtext +=  lastButtonState[n];
            debugtext += " lastDebounce: ";
            debugtext +=  lastDebounceTime[n] - millis();
            Serial.println(debugtext);
            
            //tone(buzzer, pitches[n], 1);
            if ( n == pattern[currentLevel][patternIndex])
            {
              currentPattern[patternIndex]= n;
              patternIndex += 1;
              simonMode = true;
              Serial.print("correct! pattern index:");
              Serial.print(patternIndex);
              Serial.print("current level");
              Serial.print(currentLevel);             
              if (patternIndex == 5 )  //== sizeof(pattern[currentLevel])
              {
                  Serial.println("level won!");
                  Serial.println(currentLevel);
                  //currentLevel += 1;
                  UDP.beginPacket(pcRemoteHost, pcRemotePort);
                  UDP.print("LEVELUP");
                  UDP.endPacket();
                  handleLevelUp();
              }
            }
            else
            {
               //reset
               Serial.println("FAIL");
               Serial.print(patternIndex);
               Serial.print("current level");
               Serial.print(currentLevel);                
               UDP.beginPacket(pcRemoteHost, pcRemotePort);
               UDP.print("FAIL");
               UDP.endPacket();
               handleReset();
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


