/*
androMapa

ip:  192.168.0.71

port:  7001

MANDA:

-

RECIBE:

reset
alarma

*/




#include <Bounce2.h>   // https://github.com/thomasfredericks/Bounce2
#include <Adafruit_NeoPixel.h>

// do en regular arduino!




/////////////////// VARIABLES


bool alarma = false;

const int leds[] = {7, 8, 9, 10} ;       //normal led w 230 resistance     con D4 hay un tema raro

const int redLeds = 11;

#define buttonsPin1 3    //button w 1k / 10k resistance
#define buttonsPin2 4
#define buttonsPin3 5
#define buttonsPin4 6

#define PIXEL_PIN    3    // Digital IO pin connected to the NeoPixels.
#define PIXEL_COUNT 10
Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

int pos = 0;  // para strip que se mueve

Bounce buttons[4];

Bounce buttons0 = Bounce();
Bounce buttons1 = Bounce();
Bounce buttons2 = Bounce();
Bounce buttons3 = Bounce();

const int debounceInterval = 25;


const int pattern[4][4] = { 
  {LOW, HIGH, HIGH, HIGH},
  {HIGH, LOW, LOW, HIGH},
  {LOW, LOW, HIGH, HIGH},
  {LOW, HIGH, LOW, HIGH},
};


String incomingData = "";
char character;
int lastCharTime = 0;


/////////////////// END



//WiFiUDP UDP;
//boolean udpConnected = false;
//char packetBuffer[UDP_TX_PACKET_MAX_SIZE]; //buffer to hold incoming packet,
//char ReplyBuffer[] = "acknowledged"; // a string to send back
//
//ESP8266WebServer server(80);   //connecto HTTP in

void setup() {

  Serial.begin(115200);
  delay(1000);

      
///////////////////   initialise pins

   
      pinMode(leds[0],OUTPUT);
      pinMode(leds[1],OUTPUT);
      pinMode(leds[2],OUTPUT);
      pinMode(leds[3],OUTPUT);

      pinMode(redLeds,OUTPUT);

      strip.begin();
      strip.show(); // Initialize all pixels to 'off'
         
      pinMode(buttonsPin1,INPUT_PULLUP);
      pinMode(buttonsPin1,INPUT_PULLUP);
      pinMode(buttonsPin1,INPUT_PULLUP);
      pinMode(buttonsPin1,INPUT_PULLUP);

      buttons0.attach(buttonsPin1);
      buttons0.interval(debounceInterval);
      buttons1.attach(buttonsPin2);
      buttons1.interval(debounceInterval);
      buttons2.attach(buttonsPin3);
      buttons2.interval(debounceInterval);      
      buttons3.attach(buttonsPin4);            
      buttons3.interval(debounceInterval);
      
/////////////////// END
   
}



void handleTest()
{
//   server.send(200, "text/plain", controllerId);
   //Serial.println("testing request");
   Serial.println("ok");
}







void loop() {
 

/////////////////// ACTUAL CODE

        buttons0.update();
        buttons1.update();  
        buttons2.update();
        buttons3.update();

        


        if(buttons0.rose())
        {
             Serial.println("pressed button");
              for(int j=0 ; j <4 ; j++)       // j = leds
              {
                int ledState = pattern[0][j];
                digitalWrite(leds[j], ledState);
                Serial.print("button1");
                //Serial1.print("button1");                
              }          
        }
        
        if(buttons1.rose())
        {
             Serial.println("pressed button");
              for(int j=0 ; j <4 ; j++)       // j = leds
              {
                int ledState = pattern[1][j];
                digitalWrite(leds[j], ledState);
                Serial.print("button2");
                //Serial1.print("button2");                
              }          
        }
        
        if(buttons2.rose())
        {
             Serial.println("pressed button");
              for(int j=0 ; j <4 ; j++)       // j = leds
              {
                int ledState = pattern[2][j];
                digitalWrite(leds[j], ledState);
                Serial.print("button3");
                //Serial1.print("button3");                
              }          
        }
              
        if(buttons3.rose())
        {
             Serial.println("pressed button");
              for(int j=0 ; j <4 ; j++)       // j = leds
              {
                int ledState = pattern[3][j];
                digitalWrite(leds[j], ledState);
                Serial.print("button4");
                //Serial1.print("button4");
                
              }          
        }


    if (alarma == true)
    {
        digitalWrite(redLeds,HIGH);
        updateStrip(4);
    }

    checkIncoming();



/////////////////// END
   
}

void updateStrip(uint8_t wait)
{
  
    for(int i=0; i< strip.numPixels(); i++) 
    {
 
      int strength = map(i+ pos, 0, strip.numPixels(), 0, 254);
      
      strip.setPixelColor(i, 0,0, strength);
    }
    delay(wait);
    pos += 1 % strip.numPixels();
}


