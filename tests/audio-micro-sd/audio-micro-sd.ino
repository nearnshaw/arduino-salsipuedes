

#include "SD.h"
#define SD_ChipSelectPin 10
#include "TMRpcm.h"
#include "SPI.h"

TMRpcm tmrpcm;


String audioFile = "audiotest2.wav";



void setup(){
  tmrpcm.speakerPin = 7;
  Serial.begin(9600);
  if (!SD.begin(SD_ChipSelectPin)) {
    Serial.println("SD fail");
  return;
}

tmrpcm.setVolume(6  );

}

void loop(){ 
  tmrpcm.play("audiotest2.wav");
  delay(3000);
  
  
  }
