#include <RestClient.h>       // https://github.com/fabianofranca/ESP8266RestClient
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>


const char* hostPC = "192.168.0.15";  //IP de PC
const int portPC = 7003;          // puerto PC

const char* ssid = "NICTOPIA";
const char* password = "queganasdejoder";


/*
BEST PRACTICES

in handleRoot ALWAYS put description, leave all methods in /something

/test :  has name of device

A0, analoge pin, weird behavior, seems to block wifi working,  avoid

*/




RestClient client = RestClient(hostPC,portPC);
ESP8266WebServer server(80);


const int led = D5;
const int ledStrip = D7;
const int magnet = D1;
int mag = 0;
boolean magnetState = false;

void handleRoot() {
  
  digitalWrite(led, 1);
  digitalWrite(ledStrip, 1);
  delay(500);
  server.send(200, "text/plain", "hello from esp8266!");
  Serial.println("root request");
  digitalWrite(led, 0);
  digitalWrite(ledStrip, 0);
}

void handleTest()
{
   server.send(200, "text/plain", "hab13tel");
   Serial.println("testing request");
}


void handleNotFound(){
  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(led, 0);
}

void setup(void){
  pinMode(led, OUTPUT);
  pinMode(ledStrip, OUTPUT);
  pinMode(magnet,INPUT);
  digitalWrite(led, 0);
  Serial.begin(115200);

  
  WiFi.begin(ssid, password);
  client.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);

  server.on("/test", handleTest);

  server.on("/inline", [](){
    server.send(200, "text/plain", "this works as well");
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");

  //setup client

  //HTTPClient http;
  //char address = "http://" + host + ":" + port;
  //Serial.println(address);
  //http.begin(address);
  //http.send("1");

}

void loop(void){
  server.handleClient();

  mag = digitalRead(magnet);
  Serial.println(mag);
  delay(1);
  if(mag == 1)
  {
    if (magnetState == false)
    {
  
      //client.begin(ssid, password);
      ///int statusCode = client.get("/");
      //client.post("/","1");
      //client.get("/");
      String response = "";
      int statusCode = client.post("/", "foo=bar", &response);
      
      Serial.print("request sent, response:");          
      Serial.print( statusCode);
      delay(1000);
      magnetState = true;
      
    }
  }
  else
  {
     magnetState = false;
  }
  

  
}




