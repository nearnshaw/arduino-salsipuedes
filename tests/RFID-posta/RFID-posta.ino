#include <LiquidCrystal.h>


#include <SPI.h>

#include <MFRC522.h>


/*
 * --------------------------------------------------------------------------------------------------------------------
 * Example sketch/program showing how to read data from a PICC to serial.
 * --------------------------------------------------------------------------------------------------------------------

 * -----------------------------------------------------------------------------------------
 *             MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino
 *             Reader/PCD   Uno/101       Mega      Nano v3    Leonardo/Micro   Pro Micro
 * Signal      Pin          Pin           Pin       Pin        Pin              Pin
 * -----------------------------------------------------------------------------------------
 * RST/Reset   RST          9           
 * SPI SS      SDA(SS)      10            
 * SPI MOSI    MOSI         11  
 * SPI MISO    MISO         12   
 * SPI SCK     SCK          13 
 * IRQ                      null
 * 
 *
 *Screen:
 *
 *VSS   G
 *VDD   5v
 *V0    5v (w/ fader)
 *RS    7
 *RW    G
 *E     6
 *D0
 *D1
 *D2
 *D3
 *E4    5
 *D5    4
 *D6    3
 *D7    2
 *A     5v (with 1k resistance) 
 *K     G
 *
 *      almost like:  https://www.arduino.cc/en/uploads/Tutorial/LCD_Base_bb_Fritz.png
 */


#define RST_PIN         9          // Configurable, see typical pin layout above
#define SS_PIN          10         // Configurable, see typical pin layout above
const int buzzPin = A0;
bool validado = false;

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

// UID  95 6D D0 65
int byte0 = 149;
int byte1 = 109;
int byte2 = 208;
int byte3 = 101;

String currentCode = "";


// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);
String screenMsg1 ="";
String screenMsg2 ="";

void setup() {
  Serial.begin(9600);   // Initialize serial communications with the PC
  while (!Serial);    // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
  SPI.begin();      // Init SPI bus
  pinMode(SS_PIN, OUTPUT);
  mfrc522.PCD_Init();   // Init MFRC522
  mfrc522.PCD_DumpVersionToSerial();  // Show details of PCD - MFRC522 Card Reader details
  Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("No se detecta ninguna iris.");

  pinMode(buzzPin,OUTPUT);
}

void loop() {
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
      if (validado == true)
      {
        screenMsg1 = "Traba abierta ->";
        screenMsg2 = "Deslizar panel.";
      }
      else
      {
        screenMsg1 = "Por favor acerca";
        screenMsg2 = "iris al lector.";
      }

      lcd.setCursor(0, 0);
      lcd.print(screenMsg1);
      lcd.setCursor(0, 1);
      lcd.print(screenMsg2);
      return;
  }


  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }


  currentCode = mfrc522.PICC_ReadCardSerial();
  Serial.println(currentCode);


  // Dump debug info about the card; PICC_HaltA() is automatically called
  mfrc522.PICC_DumpToSerial(&(mfrc522.uid));

  Serial.println(mfrc522.uid.uidByte[0]);
  Serial.println(mfrc522.uid.uidByte[1]);
  Serial.println(mfrc522.uid.uidByte[2]);
  Serial.println(mfrc522.uid.uidByte[3]);




  if (mfrc522.uid.uidByte[0] == byte0 && 
       mfrc522.uid.uidByte[1] == byte1 &&
       mfrc522.uid.uidByte[2] == byte2 &&
       mfrc522.uid.uidByte[3] == byte3 )
     {  

    lcd.clear();
    lcd.setCursor(0, 1);  
    lcd.print("Bienvenido!");
    tone(buzzPin,1500,600);
    
    // avisar evento wifi
    
    delay(2000);
    validado = true;
    return;
  }
  else
  {
   //   
  }





  
  
  
}
