#include <IRremote.h>
#include <IRremoteInt.h>

#include <Dhcp.h>
#include <Dns.h>
#include <Ethernet.h>
#include <EthernetClient.h>
#include <EthernetServer.h>
#include <EthernetUdp.h>

/*
 * Author: SMRAZA KEEN
 * Date:2016/6/29
 * IDE V1.6.9
 * Email:TechnicSmraza@outlook.com
 * Function:
 */
#include <IRremote.h> 
const int irReceiverPin = 3;   
IRrecv irrecv(irReceiverPin); 
decode_results results;   
 
void setup()
{
  Serial.begin(9600);   
  irrecv.enableIRIn();   
}
void loop() 
{
  if (irrecv.decode(&results)) 
  {  
    Serial.print("IR_Code: ");            
    Serial.print(results.value, HEX); 
    Serial.print(",  Bits: ");           
    Serial.println(results.bits); 
    irrecv.resume();    
  }  
  delay(600); 
}

