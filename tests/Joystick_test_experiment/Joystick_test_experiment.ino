/*
 * Author: SMRAZA KEEN
 * Date:2016/6/29
 * IDE V1.6.9
 * Email:TechnicSmraza@outlook.com
 * Function:
 */
int xpotPin = A0;
int ypotPin = A1;            
int bpotPin = 7; 
int xval=0;       //Initialize a variable
int yval=0;
int bval=0;
void setup()                                                                                     
{
  pinMode(xpotPin,INPUT);
  pinMode(ypotPin,INPUT);
  pinMode(bpotPin,INPUT); 
  Serial.begin(115200); // Inicializamos el puerto serie a una velocidad de 115200 baudios.
}
void loop()
{ 
  xval = analogRead(xpotPin);   //Read Values from the xpotPin
  yval = analogRead(ypotPin);   
  bval = digitalRead(bpotPin);   
  
  
  Serial.println("X=");            //Display "X="
  Serial.println(xval);
  Serial.println("Y=");       
  Serial.println(yval);
  Serial.println("switch");  
  Serial.println(bval);
  delay(500); //After 150ms the screen will be refreshed
}
