void handleSillaOn()
{ 

  if (sillaOn == false && gamesWon > 3)
  {
    sillaOn = true;
    efectoTurbina = true;  
    turbinaCounter = 5000;
    Serial.print("silla on");
    turbinaFlash();
  }
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

       if(incomingData.equals("on"))
       {
          handleOn();
       }

       if(incomingData.equals("off"))
       {
          handleOff();
       }

       if(incomingData.equals("win"))
       {
          handleWin();
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

       if(incomingData.equals("manual"))
       {
          handleManual();
       }
       
 
       incomingData = "";
     }

  
  
  }


void handleReset()
{ 
   Serial.println("doiing a reset");
   Serial1.print("OFF");
///////////////////  VARIABLES TO RESET

  lucesOff();  
  resetState();
  shutUp();

  enchufeCounter = 0;
  delay(10);
 /////////////////// END
   
}

void handleOn()
{ 
   Serial.println("machine ON");

///////////////////  VARIABLES TO RESET

  lucesOff();
  resetState();

  enchufeCounter = 0;
  enchufada = true;
   
  //attractMode(); // After setup is complete, say hello to the world
 /////////////////// END
   
}

void handleOff()
{ 
   Serial.println("turning off");

///////////////////  VARIABLES TO RESET

  lucesOff();
  resetState();
  delay(10);
  shutUp();
 /////////////////// END
   
}


void handleWin()
{ 
   Serial.println("winnnnning");

///////////////////  VARIABLES TO RESET

  lucesOff();
  resetState();
  delay(10);
  shutUp();
  exitRoom();   //abrir puerta
  yaGano = true;
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
  digitalWrite(botonSalida,HIGH);

  digitalWrite(luzSalida,LOW);
  delay(10);
  
  for(int i=0; i< total_strip1; i++) {
      strip1[i] = CRGB(0,0,0);
  }

  for(int i=0; i< total_turbina; i++) {
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
  simonReady = false;
  sillaOn = false; 
  efectoTurbina = false; 
  luzRayos = false;
  tiempoRayos = 0;  

  gamesWon = 0;
  gameRound = 0;
  
  yaGano = false;

  //enchufeCounter = 0;
  Serial.print("reset state");
}

void shutUp()
{
    noTone(BUZZER);
}

void handleManual()
{

  if (gamesWon > 3)   // ya esta todo menos silla
  {
    handleWin(); 
  }
  else
  {
    gamesWon +=1;
    rayoAvanza();
  }

}

