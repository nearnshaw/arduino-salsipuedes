#include <ESP8266WiFi.h>
#include <WiFiUDP.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#include <Adafruit_NeoPixel.h>

/////////////////// wifi connection variables

const char* ssid = "NICTOPIA";
const char* password = "queganasdejoder";
boolean wifiConnected = false;
const char* pcRemoteHost = "192.168.0.15";
const int pcRemotePort = 1307;
const int localPort = 1307;
const char* controllerId = "hab13portal";


IPAddress ip(192,168,0,57);
IPAddress gateway(192,168,11,1);
IPAddress subnet(255,0,0,0);//255,255,0); 
IPAddress dns(10,0,2,200);//(192,168,1,249);//(10,8,0,10);

/////////////////// END

/////////////////// VARIABLES


#define PIXEL_PIN    D1    // Digital IO pin connected to the NeoPixels.

#define PIXEL_COUNT 61

#define pixelsInCircle 61

// Parameter 1 = number of pixels in strip,  neopixel stick has 8
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_RGB     Pixels are wired for RGB bitstream
//   NEO_GRB     Pixels are wired for GRB bitstream, correct for neopixel stick
//   NEO_KHZ400  400 KHz bitstream (e.g. FLORA pixels)
//   NEO_KHZ800  800 KHz bitstream (e.g. High Density LED strip), correct for neopixel stick
Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);


int showType = 0;

int state = 0;   //0 off, 1 on

String fase = "off";

bool pulso = false;

int pos = 0;

float deg = 360 / pixelsInCircle;

const int totalPulso = 1000;
int tiempoPulso = 0;
float intensidadPulso = 255;
//time since start of action?



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

    server.onNotFound(handleNotFound);

///////////////////     MORE URL OPTIONS

    server.on("/on", handleOn);

    server.on("/off", handleOff);

    server.on("/shutdown", handleShutDown);

    server.on("/red", handleRed);

    server.on("/cold", handleCold);

    server.on("/pulso", handlePulso);


//more calls go here




/////////////////// END

    
    server.begin();
    Serial.println("HTTP server started");
    udpConnected = connectUDP();
    if (udpConnected){

      
///////////////////   initialise pins

    strip.begin();
    strip.show(); // Initialize all pixels to 'off'
    strip.setBrightness(255);

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
  message += "/test /reset /manual /on /off /shutdown /red /cold /pulso \n\n" ;
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

pos = 0;
state = 0;
fase = "off";

    strip.setBrightness(255);
    
for(int i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, 0,0,0);
}
  
strip.show(); // Initialize all pixels to 'off'

/////////////////// END
   
}

void handleManual()
{
/////////////////// VARIABLES TO CHANGE MANUALLY

//add here  
  state = 1;
  server.send(200, "text/plain", "manual");
/////////////////// END
  
}



///////////////////  HANDLE OTHER CALLS


// other call functions go here


void handleOn()
{
  state = 1;
  fase = "start";
  server.send(200, "text/plain", "on");
  strip.setBrightness(255);
}

void handleOff()
{
  state = 0;
  fase = "off";
  tiempoPulso = 0;
  for(int i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, 0,0,0);
  }
  
  strip.show(); // Initialize all pixels to 'off'
  server.send(200, "text/plain", "off");

}

void handleShutDown()
{
  state = 1;
  fase = "apagando";
  tiempoPulso = 0;
  server.send(200, "text/plain", "shutdown");
}


void handleRed()
{
  state = 1;
  fase = "red";
  server.send(200, "text/plain", "red");
  strip.setBrightness(255);
}


void handleCold()
{
  state = 1;
  fase = "cold";
  server.send(200, "text/plain", "cold");
  strip.setBrightness(255);
}

void handlePulso()
{
  state = 1;
  pulso = true;
  fase = "off";
  tiempoPulso = totalPulso;
  server.send(200, "text/plain", "pulso");
}



/////////////////// END





void loop() {
  
// check if the WiFi and UDP connections were successful
if(wifiConnected){
    server.handleClient();
    
    if(udpConnected){
      
      UDPRead();
      delay(15);


/////////////////// ACTUAL CODE
    
    if (state == 1)
    {
      if (fase == "start")
      {
        updateRedWheel(10);
      }
      else if (fase == "red")
      {
        updateRedFlow(10);      
      }
      else if (fase == "cold")
      {
        updateColdFlow(10);
      }
      else if (pulso == true)
      {
        updatePulso();
      }
   
    }
    else if (state == 0)
    {
          strip.begin();
    }
    
    strip.show();


//diferenciar aro de nubes
// hacer uno que pulse entero el mismo color




/////////////////// END
    
    }
  }
}


////////////  OTRAS FUNCIONES


void updateRedFlow(uint8_t wait) {
    uint16_t i;

    for(int i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, redFlow(i  + pos));
    }
    delay(wait);
    pos += 1 % strip.numPixels();
}


// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t redFlow(byte WheelPos) {
  if(WheelPos < 180) {
    return strip.Color(255, 0,  WheelPos * 0.2);  // R G B
  }
  WheelPos -= 180;
  return strip.Color(255, WheelPos * 0.2 ,0);
}



void updateRedWheel(uint8_t wait) {
    uint16_t i;

    for(int i=0; i< pixelsInCircle; i++) {
      int finalPos = (i + pos) % pixelsInCircle; 
      strip.setPixelColor(i, redWheel(finalPos));
    }
    delay(wait);
    pos += 1;
    pos = pos % pixelsInCircle;    
}


// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t redWheel(byte WheelPos) 
{
  int realPosition = WheelPos * deg;
  //Serial.println(realPosition);
  if(realPosition < 90) {
    int newColor = map(realPosition, 0, 90, 0, 40);
    //Serial.println(newColor);
    return strip.Color(255, 0,  newColor);  // R G B
  }
  else if (realPosition < 180) {
    int newColor = map(realPosition, 90, 180, 40, 0);
    //Serial.println(newColor);
    return strip.Color(255, 0, newColor);  // R G B
  }
  else if (realPosition < 270) {
    int newColor = map(realPosition, 180, 270, 0, 40);
    //Serial.println(newColor);
    return strip.Color(255, newColor, 0);  // R G B
  }
  int newColor = map(realPosition, 270, 359, 40, 0);
  //Serial.println(newColor);
  return strip.Color(255, newColor ,0);
}










void updateColdFlow(uint8_t wait) {
    uint16_t i;
   for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, coldFlow(i  + pos));
    }
    delay(wait);
    pos += 1 % strip.numPixels();
}


// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t coldFlow(byte WheelPos) {
  int newColor = map(WheelPos * deg, 0, 360, 0, 255);
  if(WheelPos < 180) {
    return strip.Color(0,  newColor * 1.5, 255);  //R G B
  }
  WheelPos -= 180;
  return strip.Color(255 - newColor * 1.5 ,0 ,  255);
}

void updatePulso()
{

  int curva = tiempoPulso % 150;  //255 - 30 / 3   // 530;   //255 X 2  - 30

  if (curva >   75)
  {
    intensidadPulso -= 3;
  }
  else if (curva  <   75)
  {
    intensidadPulso += 3;
  }
  
  intensidadPulso = constrain(intensidadPulso,30,255);

  tiempoPulso -= 1;

  if (tiempoPulso == 0)
  {
    pulso = false;
    intensidadPulso = 255;
  }

  Serial.println(intensidadPulso);

  //strip.Color(intensidadPulso, 0, 0);

   for(int i=0; i< pixelsInCircle; i++) 
  {
          strip.setPixelColor(i, intensidadPulso, 0, 15);
  }
  
  
}

