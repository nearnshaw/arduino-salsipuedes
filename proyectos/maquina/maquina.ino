/*
hab13maquina

ip:  192.168.0.53

port:  1304

MANDA:

WIN
FAIL
KWIN
RWIN
SWIN
ON   enchufe
OFF  enchufe

RECIBE:

reset
ON
OFF
s_on
s_off
trueno

*/

#include <Bounce2.h>   // https://github.com/thomasfredericks/Bounce2
#include "FastLED.h"

/*************************************************
* Public Constants
*************************************************/

// enchufe principal

#define ENCHUFE_PIN 8

// leds simon

#define LED_RED     22
#define LED_GREEN   24
#define LED_BLUE    26
#define LED_YELLOW  28


// botones simon

#define BUTTON_RED    23
#define BUTTON_GREEN  25
#define BUTTON_BLUE   27
#define BUTTON_YELLOW 29

// buzzers

#define BUZZER1  10
#define BUZZER2  11

// relays a plasmas

#define plasma1   30
#define plasma2   31
#define plasma3   32
#define plasma4   33

// tiras de leds

#define strip1Data     41    // leds trueno
#define strip1Clock    40   

#define turbinaData     35    // leds trueno
#define turbinaClock    36   

//knobs to turn
#define knob1   A13
#define knob2   A14
#define knob3   A15

#define BUTTON_KNOBS    23   //   cambiar!!!  x ahora es la roja de simon says


//radio
#define radio1   5
#define radio2   6
#define radio3   7





// parametros de leds

#define PIXEL_COUNT1 3  // tramo pared a plasma1
#define PIXEL_COUNT2 3  // tramo plasma1 a plasma2
#define PIXEL_COUNT3 3  // tramo plasma2 a plasma3
#define PIXEL_COUNT4 1 // tramo plasma3 a maquina

// para tener el index dentro de la tira
#define PIXELS1 PIXEL_COUNT1                // tramo pared a plasma1
#define PIXELS2 PIXELS1 + PIXEL_COUNT2      // tramo plasma1 a plasma2
#define PIXELS3 PIXELS2 + PIXEL_COUNT3      // tramo plasma2 a plasma3
#define PIXELS4 PIXELS3 + PIXEL_COUNT4     // tramo plasma3 a maquina

#define PIXEL_COUNT5 15  // turbina 1
#define PIXEL_COUNT6 15  // turbina 2

const int total_strip1 = PIXEL_COUNT1 + PIXEL_COUNT2 + PIXEL_COUNT3 + PIXEL_COUNT4;
const int total_turbina = PIXEL_COUNT5 + PIXEL_COUNT6;

//  led strips
CRGB strip1[total_strip1];
CRGB ledsTurbina[total_turbina];




///////  VALORES

//valores knobs    entre 0 y 1023

int valorK1 = 100;
int valorK2 = 0;
int valorK3 = 800;

int margenKnobs = 50;

//valores juego 3

int valorR1 = 100;
int valorR2 = 0;
int valorR3 = 800;

// enchufe




//estado maquina


bool enchufada = false;
bool knobsWon = false;
bool radioWon = true;
bool simonWon = false;
bool sillaOn = false;

int gamesWon = 0;
bool efectoTurbina = false;




const int baseBrightness = 50;   // brightness piso
int turbinaPos = 0;   //para que giren locas
bool luzRayos = false;
const int totalRayos = 1500;    // tiempo total
int tiempoRayos = 0;    //  cuanto queda de tiempo 
int velocidad_rayos = 500;    // que tan rapido avanza
int intensidadTurbina = 30;   // brillo de luces, arranca tranca va increcendo
int intensidadRayos = 30;    // brillo de luces, que pegue spike con rayos

int offCounter = 0;   // contador, ciclos desde que esta desenchufada


// Define simon parameters
#define ROUNDS_TO_WIN      4     //Number of rounds to succesfully remember before you win. 13 is do-able.
#define ENTRY_TIME_LIMIT   8000 //Amount of time to press a button before game times out. 3000ms = 3 sec

#define MODE_MEMORY  0


// Game state variables
byte gameMode = MODE_MEMORY; //By default, let's play the memory game
byte gameBoard[20]; //Contains the combination of buttons as we advance
byte gameRound = 0; //Counts the number of succesful rounds the player has made it through

////////////debounce

Bounce buttons0 = Bounce();
Bounce buttons1 = Bounce();
Bounce buttons2 = Bounce();
Bounce buttons3 = Bounce();

Bounce botonKnobs = Bounce();

const int debounceInterval = 25;   // for all buttons


///  del simon, no tocar

#define CHOICE_OFF      0 //Used to control LEDs
#define CHOICE_NONE     0 //Used to check buttons
#define CHOICE_RED  (1 << 0)
#define CHOICE_GREEN    (1 << 1)
#define CHOICE_BLUE (1 << 2)
#define CHOICE_YELLOW   (1 << 3)


//////////  wifi helper
String incomingData = "";
char character;
int lastCharTime = 0;

//////////




void setup()
{
  Serial.begin(115200);
  Serial1.begin(115200);

  //Enable pull ups on inputs
  
  pinMode(ENCHUFE_PIN, INPUT);
  
  pinMode(BUTTON_RED, INPUT_PULLUP);
  pinMode(BUTTON_GREEN, INPUT_PULLUP);
  pinMode(BUTTON_BLUE, INPUT_PULLUP);
  pinMode(BUTTON_YELLOW, INPUT_PULLUP);

  pinMode(BUTTON_KNOBS, INPUT_PULLUP);

  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);

  pinMode(BUZZER1, OUTPUT);
  pinMode(BUZZER2, OUTPUT);

  pinMode(plasma1, OUTPUT);
  pinMode(plasma2, OUTPUT);
  pinMode(plasma3, OUTPUT);
  pinMode(plasma4, OUTPUT);

  pinMode(knob1, INPUT);
  pinMode(knob2, INPUT);
  pinMode(knob3, INPUT);

  pinMode(radio1, INPUT);
  pinMode(radio2, INPUT);
  pinMode(radio3, INPUT);
  

  buttons0.attach(BUTTON_RED);
  buttons0.interval(debounceInterval);
  buttons1.attach(BUTTON_GREEN);
  buttons1.interval(debounceInterval);
  buttons2.attach(BUTTON_BLUE);
  buttons2.interval(debounceInterval);      
  buttons3.attach(BUTTON_YELLOW);            
  buttons3.interval(debounceInterval);

  botonKnobs.attach(BUTTON_KNOBS);            
  botonKnobs.interval(debounceInterval);

  FastLED.addLeds<APA102, strip1Data, strip1Clock, RGB>(strip1, total_strip1);
  FastLED.addLeds<APA102, turbinaData, turbinaClock, RGB>(ledsTurbina, total_turbina);

  FastLED.setBrightness(baseBrightness);

  lucesOff();    // todas las luces apagadas

  //Mode checking
  gameMode = MODE_MEMORY; // By default, we're going to play the memory game


}



void loop()
{
  
  if ( enchufada == true)
  {

   if (knobsWon == false)
    {
      checkKnobs();
    }

    
    if (radioWon == false)
    {
      checkRadio();
    }

    //simon
    if (knobsWon == true && radioWon == true &&  simonWon == false)
    {
      attractMode(); // Blink lights while waiting for user to press a button
    
      // Indicate the start of game play
      setLEDs(CHOICE_RED | CHOICE_GREEN | CHOICE_BLUE | CHOICE_YELLOW); // Turn all LEDs on
      delay(1000);
      setLEDs(CHOICE_OFF); // Turn off LEDs
      delay(250);
    

       if (play_memory() == true)   //check - puede recibir requests mientras se juega???
       {
         play_winner(); // Player won, play winner tones
       }
       else
       { 
         play_loser(); // Player lost, play loser tones
       }
    }

    if (efectoTurbina == true)
    {
        updateTurbina();  
    }

    if ( luzRayos == true)
    {
      Serial.print("rayos");
      rayoPega();
    }
  
        
  }

  FastLED.show();
  enchufada = checkEnchufe();     // si esta enchufado
  checkIncoming();    // check for incoming messages from wemos
}



void checkKnobs()
{

  botonKnobs.update();
  if (botonKnobs.rose())
  { 
    Serial.print("check knobs");
  
    int k1 = analogRead(knob1);
    int k2 = analogRead(knob2);
    int k3 = analogRead(knob3);
    
    if( abs(k1 - valorK1) < margenKnobs &&
        abs(k2 - valorK2) < margenKnobs &&
        abs(k3 - valorK3) < margenKnobs)
    {
      winner_sound();
      knobsWon = true;
      gamesWon += 1;
      Serial1.print("KWIN");
      Serial.print("knobs won");
      rayoAvanza();
    }

      Serial.println("knob values:");
      Serial.print(k1);
      Serial.print(" , ");
      Serial.print(k2);
      Serial.print(" , ");
      Serial.print(k3);
      
    
  }
  
}



void checkRadio()
{
  int r1 = analogRead(radio1);
  int r2 = analogRead(radio2);
  int r3 = analogRead(radio3);
  
  if( abs(r1 - valorR1) < margenKnobs &&
      abs(r2 - valorR2) < margenKnobs &&
      abs(r3 - valorR3) < margenKnobs)
  {
    winner_sound();
    radioWon = true;
    gamesWon += 1;
    Serial.print("radio won");
    Serial1.print("RWIN");
    rayoAvanza();
  }
  
}

bool checkEnchufe()
{
  
  bool now = digitalRead(ENCHUFE_PIN); 
  if (now == HIGH)
  {
    if (enchufada == false)
    { 
      Serial1.print("ON");
      Serial.print("enchufe on");
      offCounter = 0;
      gamesWon += 1;
      rayoAvanza();
      delay(25);

      //  capaz prender un par de luces acá??
    }
    return true;
  }   
  else 
  {
     offCounter += 1;
     //Serial.println(offCounter);   
   }
      
   if (offCounter == 50)   // se acaba de apagar
   {
      Serial1.print("OFF");
      Serial.print("enchufe off");
      handleOff();
      return false;

   }
   return enchufada;    //el estado en el que estaba antes  
   
}
