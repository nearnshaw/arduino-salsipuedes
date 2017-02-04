/*
 * IRremoteESP8266: IRsendDemo - demonstrates sending IR codes with IRsend
 * An IR LED must be connected to ESP8266 pin 0.
 * Version 0.1 June, 2015
 * Based on Ken Shirriff's IrsendDemo Version 0.1 July, 2009, Copyright 2009 Ken Shirriff, http://arcfn.com
 */

#include <IRremoteESP8266.h>

IRsend irsend(D3); //an IR led is connected to GPIO pin 0

void setup()
{
  irsend.begin();
  Serial.begin(115200);
}



void loop() {
  int khz = 38; // 38kHz carrier frequency for the NEC protocol
  // proyector on off
  unsigned int irSignal[] = {19486, 8950, -4350, 600, -550, 550, -550, 600, -1650, 550, -550, 550, -550, 550, -550, 550, -550, 550, -550 ,600, -1650, 550, -1650, 550, -1650, 550, -1650, 600, -550, 550, -550, 550, -550, 550, -550, 550, -1650, 600, -500, 600, -1650 ,550, -1650, 550, -550, 550 ,-1650, 600, -550, 550, -1650, 550 ,-550 ,550, -1650, 600 ,-500, 600, -550, 550, -1650, 550, -550, 550, -1650, 600, -550, 550};
    
  //original:  
//    9000, 4500, 560, 560, 560, 560, 560, 1690, 560, 560, 560, 560, 560, 560, 560, 560, 560, 560, 560, 1690, 560, 1690, 560, 560, 560, 1690, 560, 1690, 560, 1690, 560, 1690, 560, 1690, 560, 560, 560, 560, 560, 560, 560, 1690, 560, 560, 560, 560, 560, 560, 560, 560, 560, 1690, 560, 1690, 560, 1690, 560, 560, 560, 1690, 560, 1690, 560, 1690, 560, 1690, 560, 39416, 9000, 2210, 560}; //AnalysIR Batch Export (IRremote) - RAW

  Serial.println("NEC"); 
  irsend.sendRaw(irSignal, sizeof(irSignal) / sizeof(irSignal[0]), khz); //Note the approach used to automatically calculate the size of the array.

  delay(10000); //In this example, the signal will be repeated every 5 seconds, approximately.
}

