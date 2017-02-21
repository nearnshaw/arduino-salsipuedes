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

       if(incomingData.equals("ON"))
       {
          handleOn();
       }

       if(incomingData.equals("OFF"))
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


void handleReset()
{ 
   Serial.println("doiing a reset");

///////////////////  VARIABLES TO RESET

  lucesOff();  
  resetState();

  offCounter = 0;
 
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
  
 /////////////////// END
   
}

void lucesOff()
{

  digitalWrite(LED_RED,LOW);
  digitalWrite(LED_GREEN,LOW);
  digitalWrite(LED_BLUE,LOW);
  digitalWrite(LED_YELLOW,LOW);


  digitalWrite(plasma1,LOW);
  digitalWrite(plasma2,LOW);
  digitalWrite(plasma3,LOW);


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

