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
son      //silla on
soff     // silla off
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

#define BUZZER  11


//knobs to turn

#define knob1   A13
#define knob2   A14
#define knob3   A15

#define BUTTON_KNOBS    23   //   cambiar!!!  x ahora es la roja de simon says


//radio
#define radio1   A13  // potenciometro lineal  44
#define radio2   A14  // switch 1       45
#define radio3   A15  //   switch 2       46
 




// parametros de leds

// relays a plasmas

#define plasma1   30
#define plasma2   31
#define plasma3   32

#define botonSalida   33   // relay para buzzer de puerta

#define luzSalida    53    // led que alumbra boton   

#define strip1Data     50    // leds trueno
#define strip1Clock    51    

#define turbinaData     41    // leds trueno
#define turbinaClock    40   



// cantidades de leds

#define PIXEL_COUNT1 1  // tramo pared a plasma1
#define PIXEL_COUNT2 1  // tramo plasma1 a plasma2
#define PIXEL_COUNT3 4  // tramo plasma2 a plasma3
#define PIXEL_COUNT4 4 // tramo plasma3 a maquina

// para tener el index dentro de la tira
#define PIXELS1 PIXEL_COUNT1                // tramo pared a plasma1
#define PIXELS2 PIXELS1 + PIXEL_COUNT2      // tramo plasma1 a plasma2
#define PIXELS3 PIXELS2 + PIXEL_COUNT3      // tramo plasma2 a plasma3
#define PIXELS4 PIXELS3 + PIXEL_COUNT4     // tramo plasma3 a maquina

#define PIXEL_COUNT5 5  // turbina 1
#define PIXEL_COUNT6 5  // turbina 2

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

int valorR1 = 650;   // 0 a 1100
int valorR2 = HIGH;
int valorR3 = HIGH;

int tiempoRadio = 1000;
const int tiempoRadioTotal = 1000;

// enchufe




//estado maquina


bool enchufada = LOW;
bool knobsWon = false;
bool radioWon = false;
bool simonReady = false;
bool simonWon = false;
bool sillaOn = false;

int gamesWon = 0;
bool efectoTurbina = false;




const int baseBrightness = 50;   // brightness piso
bool luzRayos = false;
const int totalRayos = 1500;    // tiempo total
int tiempoRayos = 0;    //  cuanto queda de tiempo 
int velocidad_rayos = 500;    // que tan rapido avanza
int intensidadRayos = 30;    // brillo de luces, que pegue spike con rayos

int turbinaPos = 0;   //para que giren locas
float intensidadTurbina = 30;   // brillo de luces, arranca tranca va increcendo
int turbinaArray[total_turbina];   // para flashes mantenidos
int turbinaCounter = 0;

int enchufeCounter = 0;   // contador, ciclos desde que esta desenchufada
bool yaGano = false;   // si ya destrabo todo, no vuelve a fijarse el switch

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

  pinMode(BUZZER, OUTPUT);

  pinMode(plasma1, OUTPUT);
  pinMode(plasma2, OUTPUT);
  pinMode(plasma3, OUTPUT);
  pinMode(botonSalida, OUTPUT);
  pinMode(luzSalida, OUTPUT);



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


  shutUp();
  lucesOff();    // todas las luces apagadas

  //Mode checking
  gameMode = MODE_MEMORY; // By default, we're going to play the memory game


}



void loop()
{
  
  if ( checkEnchufe() == true)
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
    if (simonReady== true && turbinaCounter == 0 &&  simonWon == false)
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
  
    if ( luzRayos == true)
    {
      Serial.print("rayos");
      rayoPega();
    }
  
        
  }
  updateTurbina(); 
  enchufada = checkEnchufe();     // si esta enchufado
  checkIncoming();    // check for incoming messages from wemos
  FastLED.show();
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
            shutUp();
            winner_sound();
            knobsWon = true;
            gamesWon += 1;
            Serial1.print("KWIN");
            Serial.println("knobs won");
            rayoAvanza();
        }
      else
      {
        tone(BUZZER,60,100); 
        //shutUp();
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
  int r2 = digitalRead(radio2);
  int r3 = digitalRead(radio3);


  ///    DEBUG   sacar estas lineas
  //r1 = valorR1;
  //r2 = valorR2;
  //r3 = valorR3;
  
    if( r2 == valorR2 &&
        r3 == valorR3)
      {
         int dif = abs(r1 - valorR1);
         //dif = dif * dif;
         int frecu = map(dif, 0, 800, 3400, 500);
         tone(BUZZER,frecu,2); 

         if (dif < 40) // margen mas chico que knobs
         {
            tiempoRadio -= 1;
         }
         else
         {  
            tiempoRadio = tiempoRadioTotal; 
         }

         Serial.println(dif);

         if (tiempoRadio == 0)
         {
            winner_sound();
            radioWon = true;
            gamesWon += 1;
            Serial.println("radio won");
            Serial1.print("RWIN");
            rayoAvanza();
         }
         
      }
      else
      {
        Serial.println(r1);
        Serial.print("   ");
        Serial.print(r2);
        Serial.print("   ");
        Serial.print(r3);
        }

  
}

bool checkEnchufe()
{
  if(yaGano == false)
  {
    
    bool now = digitalRead(ENCHUFE_PIN); 
    
    if (now != enchufada)
    {
      enchufeCounter += 1;
  
     if (enchufeCounter == 50)   // aceptamos el cambio
     {  
        enchufada = now;
        enchufeCounter = 0;
        if (enchufada == true)
        { 
          Serial1.print("ON");
          Serial.println("enchufe on");
          gamesWon += 1;
          rayoAvanza();
          delay(25);
        }
        else
        {
           Serial1.print("OFF");
           Serial.println("enchufe off");
           handleOff();
        }
      } 
    }
    else
    {
      enchufeCounter = 0;
    }
    return enchufada;    //el estado en el que estaba antes 
  }
  else
  {
    return false;
  }
}    
    
