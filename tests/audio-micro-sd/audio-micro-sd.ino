#include <pcmConfig.h>
#include <pcmRF.h>

#include "SD.h"
#define SD_ChipSelectPin D8
#include "TMRpcm.h"
#include "SPI.h"

TMRpcm tmrpcm;


String audioFile = "numeros-test_converted.wav";



void setup(){
  tmrpcm.speakerPin = D4;
  Serial.begin(9600);
  if (!SD.begin(SD_ChipSelectPin)) {
    Serial.println("SD fail");
  return;
}

tmrpcm.setVolume(9);
tmrpcm.play("audiotest2.wav");
}

void loop(){  }
