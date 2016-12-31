/**
 * BasicHTTPClient.ino
 *
 *  Created on: 24.05.2015
 *
 */

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>

#define USE_SERIAL Serial



const char* hostPC = "192.168.0.15";  //IP de PC
const int portPC = 7003;          // puerto PC

const char* ssid = "NICTOPIA";
const char* password = "queganasdejoder";
int httpCode = 0;

const int magnet = D1;
int mag = 0;
boolean magnetState = false;

ESP8266WiFiMulti WiFiMulti;

void setup() {

    USE_SERIAL.begin(115200);
   // USE_SERIAL.setDebugOutput(true);

    USE_SERIAL.println();
    USE_SERIAL.println();
    USE_SERIAL.println();

    for(uint8_t t = 4; t > 0; t--) {
        USE_SERIAL.printf("[SETUP] WAIT %d...\n", t);
        USE_SERIAL.flush();
        delay(1000);
    }

    WiFiMulti.addAP(ssid, password);

}

void loop() {
    // wait for WiFi connection
    if((WiFiMulti.run() == WL_CONNECTED)) {
        
        HTTPClient http;
    
        USE_SERIAL.print("[HTTP] begin...\n");
        // configure traged server and url
        //http.begin("https://192.168.1.12/test.html", "7a 9c f4 db 40 d3 62 5a 6e 21 bc 5c cc 66 c8 3e a1 45 59 38"); //HTTPS
        http.begin("http://192.168.0.15:7004"); //HTTP

          mag = digitalRead(magnet);
          Serial.println(mag);
          delay(1);
          if(mag == 1)
          {
            if (magnetState == false)
            {   
              USE_SERIAL.print("[HTTP] GET...\n");
              // start connection and send HTTP header
              int httpCode = http.POST("1");
             
              Serial.print("request sent, response:");          
              Serial.print( httpCode);
              delay(1000);
              magnetState = true;
              
            }
          }
          else
          {
             magnetState = false;
          }
           
        // httpCode will be negative on error
        if(httpCode > 0) {
            // HTTP header has been send and Server response header has been handled
            USE_SERIAL.printf("[HTTP] GET... code: %d\n", httpCode);

            // file found at server
            if(httpCode == HTTP_CODE_OK) {
                String payload = http.getString();
                USE_SERIAL.println(payload);
            }
        } else {
            USE_SERIAL.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }

        http.end();
    }

    delay(10000);
}

