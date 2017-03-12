void rayoAvanza()
{
   Serial.println("rayo avanza. gamesWon:");
   Serial.println(gamesWon);
     
   if (gamesWon == 1)    // ENCHUFE
   {
      for(int i=0; i< PIXELS1; i++) 
      {
          strip1[i] = CRGB(128,128,128);
          FastLED.show();
          delay(velocidad_rayos);
      }
      delay(200);
      turbinaCounter = 1500;
      newShapeTurbina();   // solo primera vez pq sino no tiene shape        
      ApplyShape();
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
       delay(200);
       turbinaCounter = 2500;
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
       delay(200);
       turbinaCounter = 1000;
       simonReady = true;
   }
   else if (gamesWon == 4)   // ENCHUFE + RADIO + KNOBS + SIMON
   {
       digitalWrite(LED_RED,LOW);
       digitalWrite(LED_GREEN,LOW);
       digitalWrite(LED_BLUE,LOW);
       digitalWrite(LED_YELLOW,LOW);
       
       digitalWrite(plasma3, LOW); 
       for(int i= PIXELS3; i< PIXELS4; i++) 
       {
          strip1[i] = CRGB(128,128,128);
          FastLED.show();
          delay(velocidad_rayos);
       }
       delay(200);
       turbinaCounter = 20000;
   }  // se queda esperando la silla
   else
   {
    Serial.println("got weird value for gamesWon:");
    Serial.println(gamesWon);
    }


}

void updateTurbina()
{    

    if (turbinaCounter > 1)
    { 
      turbinaFlash();
    }
    else if (turbinaCounter == 1)
    {
         turbinaOff();
         FastLED.setBrightness(baseBrightness);
         turbinaCounter = 0;       
    }
    else if (efectoTurbina == true)
    {

  
      if (sillaOn == true)
      {
        if (intensidadTurbina < 255)
        {
          intensidadTurbina +=  0.04;
          for(int i=0; i< total_turbina; i++)
          {
            ledsTurbina[i] = CRGB(255,255,255);       
          }
        }
        else
        {
          int flicker = random(0,2000);
          if (flicker > 150)
          {
            for(int i=0; i< total_turbina; i++)
            {
                  ledsTurbina[i] = CRGB(255,255,255);       
            }
          }
          else
          {
            for(int i=0; i< total_turbina; i++)
            {
                  newShapeTurbina();
                  ApplyShape();   
            }      
          }
        }
      }
      else   // sillaOn = false
      {
        intensidadTurbina = baseBrightness;
        turbinaOff();      
      }
      
      FastLED.setBrightness(intensidadTurbina);  
   }


}





void turbinaFlash()
{
   int flicker = pow(random(0,sqrt(turbinaCounter)),2); //pow(random(0,45),2);   // hasta 2025 exponencial
   if (flicker > 1900)
   {
       newShapeTurbina();        
       ApplyShape();
   }
   else if (flicker > turbinaCounter)
   {
      FastLED.setBrightness(0);
   }
   else
   {   
      flicker = flicker % 255;
      FastLED.setBrightness(flicker);                    
   }
   turbinaCounter -= 1;
}

void newShapeTurbina()
{
    for(int i=0; i<  total_turbina; i++) {

      // random y de fuerza exponencial
      
      float lum = random(0,  sqrt(255)); 
      turbinaArray[i] =  lum * lum;   //exponencial hasta 225 
    }
}

void ApplyShape()
{
     for(int i=0; i<  total_turbina; i++) 
     {
        ledsTurbina[i] = CRGB(turbinaArray[i], turbinaArray[i], turbinaArray[i]);
     }
}

void turbinaOff()
{
    for(int i=0; i<  total_turbina; i++) {

      ledsTurbina[i] = CRGB(0,0,0);  
    }
    FastLED.show();
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

void exitRoom()
{
    digitalWrite(botonSalida,LOW); // habilitar boton
    digitalWrite(luzSalida,HIGH);
}

