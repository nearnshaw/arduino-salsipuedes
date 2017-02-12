/*

ip:  192.168.0.53

port:  1304

*/

#include <Bounce2.h>   // https://github.com/thomasfredericks/Bounce2
#include <Adafruit_NeoPixel.h>

/*************************************************
* Public Constants
*************************************************/

// enchufe principal

#define ENCHUFE 38

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

// tiras de leds

#define TIRA1     33    // solo enchufe
#define TIRA2     34
#define TIRA3     35
#define TIRA4     36

#define TIRA5     37    //dentro del lavarropas
#define TIRA6     38    //dentro del lavarropas


//knobs to turn
#define knob1   1
#define knob2   2
#define knob3   3

#define BUTTON_KNOBS    4


//juego 3
#define control1   5
#define control2   6
#define control3   7



// parametros de leds

// REVISAR EL TIPO DE LEDS!!!!

#define velocidad_rayos 3    // que tan rapido avanza
int intensidadTurbina = 30;   // brillo de luces, arranca tranca va increcendo
int intensidadRayos = 30;    // brillo de luces, que pegue spike con rayos


#define PIXEL_COUNT1 6
#define PIXEL_COUNT2 6
#define PIXEL_COUNT3 6
#define PIXEL_COUNT4 6

#define PIXEL_COUNT5 15
#define PIXEL_COUNT6 15

Adafruit_NeoPixel strip1 = Adafruit_NeoPixel(PIXEL_COUNT1, TIRA1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(PIXEL_COUNT2, TIRA2, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip3 = Adafruit_NeoPixel(PIXEL_COUNT3, TIRA3, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip4 = Adafruit_NeoPixel(PIXEL_COUNT4, TIRA4, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip5 = Adafruit_NeoPixel(PIXEL_COUNT5, TIRA5, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip6 = Adafruit_NeoPixel(PIXEL_COUNT6, TIRA6, NEO_GRB + NEO_KHZ800);



///////  VALORES

//valores knobs    entre 0 y 1023

int valorK1 = 100;
int valorK2 = 0;
int valorK3 = 800;

int margenKnobs = 50;

//valores juego 3

int valorC1 = 100;
int valorC2 = 0;
int valorC3 = 800;



//estado maquina


bool enchufada = false;

bool sillaOn = false;
bool simonWon = false;
bool knobsWon = false;
bool radioWon = false;

int gamesWon = 0;
bool efectoTurbina = false;
int turbinaPos = 0;   //para que giren locas

bool luzRayos = false;
const int totalRayos = 500;
int tiempoRayos = 0;



// Define simon parameters
#define ROUNDS_TO_WIN      10 //Number of rounds to succesfully remember before you win. 13 is do-able.
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
  //Setup hardware inputs/outputs. These pins are defined in the hardware_versions header file

  //Enable pull ups on inputs
  
  pinMode(ENCHUFE, INPUT);
  
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

  pinMode(knob1, INPUT);
  pinMode(knob2, INPUT);
  pinMode(knob3, INPUT);

  pinMode(control1, INPUT);
  pinMode(control2, INPUT);
  pinMode(control3, INPUT);
  

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

    strip1.begin();
    strip1.show(); // Initialize all pixels to 'off'
    strip2.begin();
    strip2.show(); 
    strip3.begin();
    strip3.show(); 
    strip4.begin();
    strip4.show(); 
    strip5.begin();
    strip5.show();
    strip6.begin();
    strip6.show(); 
    
  Serial1.begin(115200);

  //Mode checking
  gameMode = MODE_MEMORY; // By default, we're going to play the memory game

}




void handleReset()
{ 
   Serial.println("doiing a reset");

///////////////////  VARIABLES TO RESET

  digitalWrite(LED_RED,LOW);
  digitalWrite(LED_GREEN,LOW);
  digitalWrite(LED_BLUE,LOW);
  digitalWrite(LED_YELLOW,LOW);
  gameRound = 0;

  digitalWrite(plasma1,LOW);
  digitalWrite(plasma2,LOW);
  digitalWrite(plasma3,LOW);


  for(int i=0; i< PIXEL_COUNT1; i++) 
  {
      strip1.setPixelColor(i, 0,0,0);
  }
  for(int i=0; i< PIXEL_COUNT2; i++) 
  {
      strip2.setPixelColor(i, 0,0,0);
  }
  for(int i=0; i< PIXEL_COUNT3; i++) 
  {
      strip3.setPixelColor(i, 0,0,0);
  }
  for(int i=0; i< PIXEL_COUNT4; i++) 
  {
      strip4.setPixelColor(i, 0,0,0);
  }
  for(int i=0; i< PIXEL_COUNT5; i++) 
  {
      strip5.setPixelColor(i, 0,0,0);
  }
  for(int i=0; i< PIXEL_COUNT6; i++) 
  {
      strip6.setPixelColor(i, 0,0,0);
  }

  enchufada = false;
  gamesWon = 0;
  sillaOn = true;
  simonWon = false;
  knobsWon = false;
  luzRayos = false;
  tiempoRayos = 0;
  
  efectoTurbina = false;
  
  attractMode(); // After setup is complete, say hello to the world
 /////////////////// END
   
}

void handleOn()
{ 
   Serial.println("machine ON");

///////////////////  VARIABLES TO RESET

  digitalWrite(LED_RED,LOW);
  digitalWrite(LED_GREEN,LOW);
  digitalWrite(LED_BLUE,LOW);
  digitalWrite(LED_YELLOW,LOW);
  gameRound = 0;
  gamesWon = 0;

  digitalWrite(plasma1,LOW);
  digitalWrite(plasma2,LOW);
  digitalWrite(plasma3,LOW);


  enchufada = true;
  simonWon = false;
  knobsWon = false;
  radioWon = false;
  sillaOn = false;
  efectoTurbina = false;  
  luzRayos = false;
  tiempoRayos = 0;
   
  attractMode(); // After setup is complete, say hello to the world
 /////////////////// END
   
}

void handleOff()
{ 
   Serial.println("turning on");

///////////////////  VARIABLES TO RESET

  digitalWrite(LED_RED,LOW);
  digitalWrite(LED_GREEN,LOW);
  digitalWrite(LED_BLUE,LOW);
  digitalWrite(LED_YELLOW,LOW);


  digitalWrite(plasma1,LOW);
  digitalWrite(plasma2,LOW);
  digitalWrite(plasma3,LOW);


  for(int i=0; i< PIXEL_COUNT1; i++) {
      strip1.setPixelColor(i, 0,0,0);
  }
  for(int i=0; i< PIXEL_COUNT2; i++) {
      strip2.setPixelColor(i, 0,0,0);
  }
  for(int i=0; i< PIXEL_COUNT3; i++) {
      strip3.setPixelColor(i, 0,0,0);
  }
  for(int i=0; i< PIXEL_COUNT4; i++) {
      strip4.setPixelColor(i, 0,0,0);
  }
  for(int i=0; i< PIXEL_COUNT5; i++) {
      strip5.setPixelColor(i, 0,0,0);
  }
  for(int i=0; i< PIXEL_COUNT6; i++) {
      strip6.setPixelColor(i, 0,0,0);
  }

  enchufada = false;
  simonWon = false;
  knobsWon = false;
  radioWon = false; 
  sillaOn = false; 
  efectoTurbina = false; 
  luzRayos = false;
  tiempoRayos = 0;

  gamesWon = 0;
  gameRound = 0;


  
 /////////////////// END
   
}


void handleSillaOn()
{ 
  sillaOn = true;
}

void handleSillaOff()
{ 
  sillaOn = false;
}


void handleTrueno()
{
  luzRayos = true;
  tiempoRayos = totalRayos;
}


/////// nexo con wemos

void checkIncoming()
{
  
  
  
    //   messages from wemos
    while (Serial1.available()) 
    {
        // read the incoming byte:
        character = Serial1.read();
        incomingData = String(incomingData + character);
        lastCharTime = 0;
    }
    
    lastCharTime +=1; 
  
    
    if (incomingData != "" && lastCharTime > 25) 
    {
       // say what you got:
       Serial.print("I received: ");
       Serial.println(incomingData);
       incomingData.trim();   //remove spaces & enters

       if(incomingData.equals("reset"))
       {
          handleReset();
       }

       if(incomingData.equals("on"))
       {
          handleOn();
       }

       if(incomingData.equals("off"))
       {
          handleOff();
       }


       if(incomingData.equals("s_on"))
       {
          handleSillaOn();
       }

       if(incomingData.equals("s_off"))
       {
          handleSillaOff();
       }

       if(incomingData.equals("trueno"))
       {
          handleTrueno();
       }
 
       incomingData = "";
     }

  
  
  }




void loop()
{

  if (enchufada == false)
  {
    enchufada = digitalRead(ENCHUFE);
  }
  else   // esta enchufada
  {
 

    if (simonWon == false)
      {
      attractMode(); // Blink lights while waiting for user to press a button
    
      // Indicate the start of game play
      setLEDs(CHOICE_RED | CHOICE_GREEN | CHOICE_BLUE | CHOICE_YELLOW); // Turn all LEDs on
      delay(1000);
      setLEDs(CHOICE_OFF); // Turn off LEDs
      delay(250);
    
      if (gameMode == MODE_MEMORY)
      {
        // Play memory game and handle result
        if (play_memory() == true) 
          play_winner(); // Player won, play winner tones
        else 
          play_loser(); // Player lost, play loser tones
      }
    }
    
    if (knobsWon == false)
    {
      checkKnobs();
    }

    
    if (radioWon == false)
    {
      checkRadio();
    }

    if (efectoTurbina == true)
    {
        updateTurbina();  
    }

    if ( luzRayos == true)
    {
      rayoPega();
    }
  
        
  }

    
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
      rayoAvanza();
    }
  }
  
}



void checkRadio()
{
  int c1 = analogRead(control1);
  int c2 = analogRead(control2);
  int c3 = analogRead(control3);
  
  if( abs(c1 - valorC1) < margenKnobs &&
      abs(c2 - valorC2) < margenKnobs &&
      abs(c3 - valorC3) < margenKnobs)
  {
    winner_sound();
    radioWon = true;
    gamesWon += 1;
    Serial1.print("CWIN");
    rayoAvanza();
  }
  
}






