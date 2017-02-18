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

