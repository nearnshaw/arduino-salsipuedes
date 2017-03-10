

/////// nexo con wemos

void checkIncoming()
{
  
  
  
    //   messages from wemos
    while (Serial.available()) 
    {
        // read the incoming byte:
        character = Serial.read();
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

       if(incomingData.equals("alarma"))
       {
          handleAlarma();
       }

 
       incomingData = "";
     }

  
  
  }


void handleReset()
{ 
   Serial.println("doiing a reset");

///////////////////  VARIABLES TO RESET
  digitalWrite(leds[0],LOW);
  digitalWrite(leds[1],LOW);
  digitalWrite(leds[2],LOW);
  digitalWrite(leds[3],LOW);

  digitalWrite(redLeds,LOW);
  alarma = false;
 
 /////////////////// END
   
}

void handleAlarma()
{ 
   Serial.println("alarma");

///////////////////  VARIABLES TO RESET
  alarma = true;
 
 /////////////////// END
   
}

