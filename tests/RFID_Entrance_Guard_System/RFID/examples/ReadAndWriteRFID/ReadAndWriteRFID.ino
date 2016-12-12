#include <SPI.h>
#include <RFID.h>
const int Rled = 3;
const int Gled = 4;
const int key = 2;
boolean keysw=false; 
RFID rfid(10,5); //D10-Card Reader pin MOSI ,D5 Card Reader pin RST
//The fourth byte is card's serial number and the five byte is card's Check byte
//现象写完卡后，读卡
unsigned char serNum[5];
// Write data
unsigned char writeDate[16] ={'W', 'e', 'l', 'c', 'o', 'm', 'e', 'T', 'o', 'S', 'm', 'r', 'a', 'z', 'a', 0};
//A password original sector, 16 sectors, each sector password 6Byte
unsigned char sectorKeyA[16][16] = {
        {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
        {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
        {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},};
//A password new sector, 16 sectors, each sector password 6Byte
unsigned char sectorNewKeyA[16][16] = {
        {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
        {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xff,0x07,0x80,0x69, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
        {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xff,0x07,0x80,0x69, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},};

void setup()
{
  Serial.begin(9600);
  SPI.begin();
  rfid.init();
  pinMode(Rled,OUTPUT);
  pinMode(Gled,OUTPUT);
  attachInterrupt(0, swichkey, RISING);
  analogWrite(Rled,0);
  analogWrite(Gled,0);
}
void swichkey()
{
    keysw=true;
    delay(10);
    Serial.println("key is true ");
 }
void loop()
{ 
  unsigned char i,tmp;
  unsigned char status;
  unsigned char str[MAX_LEN];
  unsigned char RC_size;
  unsigned char blockAddr;        //Selecting operation block`s address 0～63
  //Detecting card
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
   //Select card and return memory size(note:Card is locked to prevent multiple read and write
   rfid.selectTag(rfid.serNum);
 
    // write data to card 
        blockAddr = 7;                //data block 7
        if(keysw==true)
        {
        if (rfid.auth(PICC_AUTHENT1A, blockAddr, sectorKeyA[blockAddr/4], rfid.serNum) == MI_OK)  //authenticate
        {
          //Write data
          status = rfid.write(blockAddr, sectorNewKeyA[blockAddr/4]);
          Serial.print("set the new card password, and can modify the data of the Sector: ");
          Serial.println(blockAddr/4,DEC);
          //Write data
          blockAddr = blockAddr - 3 ; //data block 4
          status = rfid.write(blockAddr, writeDate);
          if(status == MI_OK)
          {
            Serial.println("Write card OK!");
            for(int j=0;j<5;j++)
            {         
              digitalWrite(Gled,HIGH);
              digitalWrite(Rled,LOW);
              delay(500);
              digitalWrite(Gled,LOW);
              delay(500);
              }
            keysw=false;
           Serial.println("key is false ");
          }
        }
        }
    //Read card
    blockAddr = 7;                //data block 7
    status = rfid.auth(PICC_AUTHENT1A, blockAddr, sectorNewKeyA[blockAddr/4], rfid.serNum);
    if (status == MI_OK)  //authenticate
    {
      //Read Data
      blockAddr = blockAddr - 3 ; //data block 4
      if( rfid.read(blockAddr, str) == MI_OK)
      {
        Serial.print("Read from the card ,the data is : ");
        Serial.println((char *)str);
        for(int j=0;j<5;j++)
        {
        digitalWrite(Gled,HIGH);
        digitalWrite(Rled,HIGH);
        delay(500);
        digitalWrite(Gled,LOW);
        digitalWrite(Rled,LOW);
        delay(500);
        }
      }
    }
  flashled();
 // swichkey();
  rfid.halt();
}

void flashled()
{
  digitalWrite(Gled,LOW);
  digitalWrite(Rled,HIGH);
  delay(200);
  digitalWrite(Rled,LOW);
  digitalWrite(Gled,HIGH);
  delay(200);
  }
