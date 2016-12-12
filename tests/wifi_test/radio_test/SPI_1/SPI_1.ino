#include <SPI.h>

/*Tutorial SPI
www.electroensaimada.com
Autor:Toni Ruiz Sastre
*/

const int slaveSelectPin = 10;

void setup(){
    SPI.begin(); 

  pinMode (slaveSelectPin, OUTPUT);
  

}

void loop(){

  SPI.transfer(0x52);
delay(10);
}
