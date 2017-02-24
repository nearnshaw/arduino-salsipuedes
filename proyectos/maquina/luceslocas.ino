void rayoAvanza()
{
   Serial.print("rayo avanza. gamesWon:");
   Serial.print(gamesWon);
     
   if (gamesWon == 1)    // ENCHUFE
   {
      for(int i=0; i< PIXELS1; i++) 
      {
          strip1[i] = CRGB(128,128,128);
          FastLED.show();
          delay(velocidad_rayos);
      }
   }
   else if (gamesWon == 2)    // ENCHUFE + RADIO o KNOBS
   {  
       digitalWrite(plasma1, LOW);  
       for(int i=PIXELS1; i< PIXELS2; i++) 
       {
          strip1[i] = CRGB(128,128,128);
          FastLED.show();
          delay(velocidad_rayos);
       }
   }
   else if (gamesWon == 3)    // ENCHUFE + RADIO + KNOBS
   {
       digitalWrite(plasma2, LOW);
       for(int i= PIXELS2; i< PIXELS3; i++) 
       {
          strip1[i] = CRGB(128,128,128);
          FastLED.show();
          delay(velocidad_rayos);
       }  
   }
   else if (gamesWon == 4)   // ENCHUFE + RADIO + KNOBS + SIMON
   {
       digitalWrite(plasma3, LOW); 
       for(int i= PIXELS3; i< PIXELS4; i++) 
       {
          strip1[i] = CRGB(128,128,128);
          FastLED.show();
          delay(velocidad_rayos);
       } 
   }  // se queda esperando la silla
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
      intensidadTurbina +=  0.05;
      }
    }
    else   // sillaOn = false
    {
      intensidadTurbina = baseBrightness;
    }
    
    FastLED.setBrightness(intensidadTurbina);


    for(int i=0; i< total_turbina; i++) {
      int finalPos = (i + turbinaPos) % total_turbina; 
      if (finalPos > total_turbina/3 ) 
      {
         ledsTurbina[i] = CRGB(20,20,20);
      }
      else
      {
          ledsTurbina[i] = CRGB(255,255,255);      }
      
    }
    turbinaCounter =+1;

    if (turbinaCounter == turbinaSpeed)   // actualizar posicion cada x ciclos
    {     
          turbinaCounter = 0;
          turbinaPos += 1;
          turbinaPos = turbinaPos % PIXEL_COUNT5;        
    }


  
}


void rayoPega()
{

  if (sillaOn == false) // no esta el buildup x la turbina
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
    intensidadRayos %= 128;
  
    FastLED.setBrightness(intensidadRayos);
    
    tiempoRayos -=1;
    if (tiempoRayos < 1)
    {
      luzRayos = false;
      intensidadRayos = 30; 
      FastLED.setBrightness(baseBrightness);
      
    }
  }
}

