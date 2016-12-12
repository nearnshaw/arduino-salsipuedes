// - includes laser and CNT1 led

/*
 * Author: SMRAZA KEEN
 * Date:2016/6/29
 * IDE V1.6.9
 * Email:TechnicSmraza@outlook.com
 * Function:
 * Tips:Open the serial port
 */
const int SensorPin1 = A1;
//const int LB = 10;
//const int LG = 9;
//const int LR = 8;

const int SensorPin2 = 8;




int Data1=0;
int Data2=0;

void setup()
{
pinMode (SensorPin1,INPUT);
pinMode (SensorPin2,INPUT);

//pinMode (LB,OUTPUT);
//pinMode (LR,OUTPUT);
//pinMode (LG,OUTPUT);
//pinMode (Laser1,OUTPUT);

Serial.begin(9600);
}
void loop()
{
  Data1=analogRead(SensorPin1);
  Data2=digitalRead(SensorPin2);
  
 //digitalWrite(Laser1,HIGH);
 if (Data1 < 50)
  {
    // digitalWrite(LG,HIGH);
    // digitalWrite(LR,LOW);
    
  }
  else
  {
     // digitalWrite(LG,LOW);
     // digitalWrite(LR,HIGH);;
  }
  
  
  Serial.print("Data1 Data2");
  Serial.println("\n");
  Serial.print(Data1);
  Serial.print("   ");
  Serial.print(Data2);
  Serial.println("\n");
  delay(500);
}
