
int linetone=0;


int needToPrint = 0;
int count;
int in = A4;
int ledPin = 7;
int lastState = LOW;
int trueState = LOW;
long lastStateChangeTime = 0;
int cleared = 0;
String fullnum = "";

String objectivenum = "157";


// constants

int dialHasFinishedRotatingAfterMs = 100;
int debounceDelay = 10;
int hangPhoneTime = 500;
int resetTime = 2000000;

void setup()
{
  Serial.begin(9600);
  pinMode(in, INPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
}

void loop()
{
  int reading = digitalRead(in);
  
  if ((millis() - lastStateChangeTime) > dialHasFinishedRotatingAfterMs) {
    // the dial isn't being dialed, or has just finished being dialed.
    if (needToPrint) {
      // if it's only just finished being dialed, we need to send the number down the serial
      // line and reset the count. We mod the count by 10 because '0' will send 10 pulses.
      
      //Serial.println(count % 10, DEC);
      fullnum = fullnum + (count % 10);
      Serial.println(fullnum);
      needToPrint = 0;
      count = 0;
      cleared = 0;
    }
  } 



  if (reading != lastState) {
    lastStateChangeTime = millis();
  }
  if ((millis() - lastStateChangeTime) > debounceDelay) {
    // debounce - this happens once it's stablized
      if (reading != trueState) {
      // this means that the switch has either just gone from closed->open or vice versa.
      trueState = reading;
      if (trueState == HIGH) {
        // increment the count of pulses if it's gone high.
        count++; 
        needToPrint = 1; // we'll need to print this number (once the dial has finished rotating)
      } 
    }
  }

//   if (trueState = LOW &&  (millis() - lastStateChangeTime) > hangPhoneTime) {
//    // no dialing for a while - number resets
//      if(fullnum.length() > 0)
//        {
//          fullnum = "";
//      
//          Serial.println("hanging phone");
//        }
//   }  

  
  
  if ((millis() - lastStateChangeTime) > resetTime) {
    // no dialing for a while - number resets
      if(fullnum.length() > 0)
        {
          fullnum = "";
      
          Serial.println("reset number");
        }
   }  

  if (fullnum.equals(objectivenum))
  {
      Serial.println("Got it!");
      digitalWrite(ledPin, HIGH);
      delay (500);
      digitalWrite(ledPin, LOW);
      delay (500);
      digitalWrite(ledPin,  HIGH);
      delay (500);
      digitalWrite(ledPin, LOW);
      delay (500);
      digitalWrite(ledPin,  HIGH);
      delay (500);     
    
    }


  lastState = reading;


//linetone = analogRead(in);
//Serial.println(linetone);


}


















