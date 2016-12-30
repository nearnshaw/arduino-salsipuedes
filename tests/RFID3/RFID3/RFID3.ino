#include <RFID.h>


/* FILE:    RC522_RFID_Module_Example
   DATE:    23/03/14
   VERSION: 0.2.1 Spark

REVISIONS:

29/08/16    Version 0.2.1, modified by ScruffR: corrected Software SPI sample

23/03/14    Version 0.2, modified by Paul Kourany to run on Spark Core
            with added support for Software SPI
24/07/13    Created version 0.1


This is an example of how to use the RC522 RFID module. The module allows reading
and writing to various types of RFID devices and can be found in our MFRC-522 
(HCMODU0016) and Ultimate RFID (HCARDU0068) kits. This example Arduino sketch uses
the RFID library written by Miguel Balboa to read the pre-programmed serial number 
from RFID cards and tags supplied with our RFID kits. Snapshots and links to the 
library are available on our support forum.


PINOUT:

RC522 MODULE    SPARK HARD SPI  SPARK SOFT SPI
SS                  A2              ANY
SCK                 A3              ANY
MOSI                A5              ANY
MISO                A4              ANY
IRQ                 N/A             N/A
GND                 GND             GND
RST                 D9              ANY
3.3V                3.3V            3.3V


You may copy, alter and reuse this code in any way you like, but please leave
reference to HobbyComponents.com in your comments if you redistribute this code.
This software may not be used directly for the purpose of selling products that
directly compete with Hobby Components Ltd's own range of products.

THIS SOFTWARE IS PROVIDED "AS IS". HOBBY COMPONENTS MAKES NO WARRANTIES, WHETHER
EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE, ACCURACY OR LACK OF NEGLIGENCE.
HOBBY COMPONENTS SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR ANY DAMAGES,
INCLUDING, BUT NOT LIMITED TO, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY
REASON WHATSOEVER.
*/


/* Include the RFID library */
/* SEE RFID.h for selecting Hardware or Software SPI modes */


/* Define the pins used for the SS (SDA) and RST (reset) pins for BOTH hardware and software SPI */
/* Change as required */
const int RST_PIN = 9;
const int SS_PIN  =   10;      // Same pin used as hardware SPI (SS)


/* Define the pins used for the DATA OUT (MOSI), DATA IN (MISO) and CLOCK (SCK) pins for SOFTWARE SPI ONLY */
/* Change as required and may be same as hardware SPI as listed in comments */
const int MOSI_PIN  =  11;      // hardware SPI: A4
const int MISO_PIN  = 12  ;    //     "     " : A5
const int SCK_PIN   =  13  ;    //     "     " : A3


boolean flag = false;    //Write card flag
RFID rfid(MISO_PIN,RST_PIN);         //D10-Card Reader pin MOSI ,D9 Card Reader pin RST


/* Create an instance of the RFID library */
//#if defined(_USE_SOFT_SPI_)

//RFID RC522(SS_PIN, RST_PIN, MOSI_PIN, MISO_PIN, SCK_PIN);    // Software SPI

//#else
//    RFID RC522(SS_PIN, RST_PIN);                                 // Hardware SPI
//#endif


void setup()
{ 
  Serial.begin(9600);

#if !defined(_USE_SOFT_SPI_)
  /* Enable the HW SPI interface */
  SPI.setDataMode(SPI_MODE0);
  SPI.setBitOrder(MSBFIRST);
  SPI.setClockDivider(SPI_CLOCK_DIV8);
  SPI.begin();
#endif

  /* Initialise the RFID reader */
  rfid.init();
}

void loop()
{
  /* Temporary loop counter */
  uint8_t i;

  /* Has a card been detected? */
  rfid.isCard();
  //Reading the card serial number
  if (rfid.readCardSerial())
  {
    Serial.print("The card's number is  : ");
    Serial.print(rfid.serNum[0],HEX);
    Serial.print(rfid.serNum[1],HEX);
    Serial.print(rfid.serNum[2],HEX);
    Serial.print(rfid.serNum[3],HEX);
    Serial.print(rfid.serNum[4],HEX);
    Serial.println(" ");
  }
  else
      Serial.println("Card NOT detected:");
      
  delay(1000);
}
