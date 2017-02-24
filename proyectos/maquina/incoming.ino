void handleSillaOn()
{ 
  sillaOn = true;
  efectoTurbina = true;  
  Serial.print("silla on");
}

void handleSillaOff()
{ 
  sillaOn = false;
  Serial.print("silla off");
}


void handleTrueno()
{
  luzRayos = true;
  tiempoRayos = totalRayos;
  Serial.print("trueno");
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

       if(incomingData.equals("ON"))
       {
          handleOn();
       }

       if(incomingData.equals("OFF"))
       {
          handleOff();
       }

       if(incomingData.equals("son"))
       {
          handleSillaOn();
       }

       if(incomingData.equals("soff"))
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


void handleReset()
{ 
   Serial.println("doiing a reset");

///////////////////  VARIABLES TO RESET

  lucesOff();  
  resetState();

  offCounter = 0;
  delay(10);
 /////////////////// END
   
}

void handleOn()
{ 
   Serial.println("machine ON");

///////////////////  VARIABLES TO RESET

  lucesOff();
  resetState();

  offCounter = 0;
  enchufada = true;
   
  //attractMode(); // After setup is complete, say hello to the world
 /////////////////// END
   
}

void handleOff()
{ 
   Serial.println("turning on");

///////////////////  VARIABLES TO RESET

  lucesOff();
  resetState();
  delay(10);
 /////////////////// END
   
}

void lucesOff()
{

  digitalWrite(LED_RED,LOW);
  digitalWrite(LED_GREEN,LOW);
  digitalWrite(LED_BLUE,LOW);
  digitalWrite(LED_YELLOW,LOW);
  delay(10);

  digitalWrite(plasma1,HIGH);     // x algun motivo funca alrevez el relay module
  digitalWrite(plasma2,HIGH);
  digitalWrite(plasma3,HIGH);
  digitalWrite(plasma4,HIGH);
  delay(10);

  for(int i=0; i< total_strip1; i++) {
      strip1[i] = CRGB(0,0,0);
  }

  for(int i=0; i< PIXEL_COUNT5; i++) {
      ledsTurbina[i] = CRGB(0,0,0);
  }
  FastLED.show();
  
  Serial.print("luces off");  
  
}

void resetState()
{
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

  //offCounter = 0;
  Serial.print("reset state");
}

