void rayoAvanza()
{
   if (gamesWon == 1)    // ENCHUFE
   {
      for(int i=0; i< PIXELS1; i++) 
      {
          strip1.setPixelColor(i, 0, 0, intensidadRayos);
          delay(velocidad_rayos);
      }
   }
   else if (gamesWon == 2)    // ENCHUFE + RADIO o KNOBS
   {  
       digitalWrite(plasma1, HIGH);  
       for(int i=PIXELS1; i< PIXELS2; i++) 
       {
          strip1.setPixelColor(i, 0, 0, intensidadRayos);
          delay(velocidad_rayos);
       }
   }
   else if (gamesWon == 3)    // ENCHUFE + RADIO + KNOBS
   {
       digitalWrite(plasma2, HIGH);
       for(int i= PIXELS2; i< PIXELS3; i++) 
       {
          strip1.setPixelColor(i, 0, 0, intensidadRayos);
          delay(velocidad_rayos);
       }  
   }
   else if (gamesWon == 4)   // ENCHUFE + RADIO + KNOBS + SIMON
   {
       digitalWrite(plasma3, HIGH); 
       for(int i= PIXELS3; i< PIXELS4; i++) 
       {
          strip1.setPixelColor(i, 0, 0, intensidadRayos);
          delay(velocidad_rayos);
       } 
   }
   else if (gamesWon == 5)   // ENCHUFE + RADIO + KNOBS + SIMON + SILLA 
   {
      efectoTurbina = true;    //PRENDE LAVARRPOAS
      Serial1.print("WIN");     
   }
   else
   {
    Serial.println("got weird value for gamesWon:");
    Serial.println(gamesWon);
    }


}

void updateTurbina()
{


    
    if (sillaOn == true)
    {
      if (intensidadTurbina < 255)
      {
      intensidadTurbina +=  1;
      }
    }
    else   // sillaOn = false
    {
      intensidadTurbina = 30;
    }


    for(int i=0; i< PIXEL_COUNT5; i++) {
      int finalPos = (i + turbinaPos) % PIXEL_COUNT5; 
      if (finalPos == PIXEL_COUNT5 || finalPos == PIXEL_COUNT5/2) 
      {
         strip5.setPixelColor(i, 0,0,0);
         strip6.setPixelColor(i, 0,0,0);
      }
      else
      {
         strip5.setPixelColor(i, 0,0,intensidadTurbina);
         strip6.setPixelColor(i, 0,0,intensidadTurbina);
      }
      
    }
    turbinaPos += 1;
    turbinaPos = turbinaPos % PIXEL_COUNT5;  

  
}


void rayoPega()
{

  if (tiempoRayos >   totalRayos / 4 * 3)
  {
    intensidadRayos += 1;
  }
  else if (tiempoRayos >   totalRayos / 2)
  {
    intensidadRayos -= 1;
  }
  else if (tiempoRayos >   totalRayos / 4 )
  {
    intensidadRayos += 1;
  }
  else
  {
    intensidadRayos -= 1;
  }



  
   if (gamesWon > 0)    // ENCHUFE
   {
      for(int i=0; i< PIXELS1; i++) 
      {
          strip1.setPixelColor(i, 0, 0, intensidadRayos);
      }
   }
   else if (gamesWon  > 1)
   {  
       for(int i= PIXELS1; i< PIXELS2; i++) 
       {
          strip1.setPixelColor(i, 0, 0, intensidadRayos);
       }
   }
   else if (gamesWon > 2)
   {
       for(int i= PIXELS2; i< PIXELS3; i++) 
       {
          strip1.setPixelColor(i, 0, 0, intensidadRayos);
       }  
   }
   else if (gamesWon > 3)
   {
       for(int i= PIXELS3; i< PIXELS4; i++) 
       {
          strip1.setPixelColor(i, 0, 0, intensidadRayos);
       } 
   }
  
  tiempoRayos -=1;
  if (tiempoRayos == 0)
  {
    luzRayos == false;
    
  }

}

