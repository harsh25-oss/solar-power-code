// new
#include <DHT.h>  // Including library for dht
 
// new ends

#define BLYNK_TEMPLATE_ID "TMPL3sxwyNmZR"
#define BLYNK_TEMPLATE_NAME "Control Pump"
#define BLYNK_AUTH_TOKEN "X6WJ5GXqAAY-l8DIAoxbmK-RUtaRTEaL"

//#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h> 
 
#include <BlynkSimpleEsp8266.h>
 

char auth[] = BLYNK_AUTH_TOKEN;

char ssid[] = "Test";  // Enter your wifi name
char pass[] = "12345678";  // Enter your wifi password

int relaypin = D4;
int motorpin = D5;

//second pro start
String apiKey = "F8XIKKECIX8HMH9Z";     //  Enter your Write API key from ThingSpeak
 

const char* server = "api.thingspeak.com";
 
#define DHTPIN 0         
#define sensorPin A0          
 
DHT dht(DHTPIN, DHT11);
 
WiFiClient client;
int ledpin = D1;

void setup()
{     
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);    
  pinMode(relaypin,OUTPUT);
  pinMode(ledpin,OUTPUT);
  pinMode(motorpin,OUTPUT);

  dht.begin();
  digitalWrite(ledpin, HIGH);
  digitalWrite(relaypin, HIGH);
  digitalWrite(motorpin, HIGH);

}

void loop()
{
  Blynk.run();

    float h = dht.readHumidity();
      float t = dht.readTemperature();
      int m = analogRead(sensorPin);
      float moisture_percentage = ( 100 - ( (m/1024.00) * 100 ) );

            
      // if(moisture_percentage < 30)
      // {
      //       Serial.println("PUMP ON...");
      //       digitalWrite(motorpin, LOW);
        
      // }
      // else if(moisture_percentage >60)
      // {
      //       Serial.println("PUMP OFF...");
      //       digitalWrite(motorpin, HIGH);
        
      // }
      // else{
      //       Serial.println("PUMP OFF...");
      //       digitalWrite(motorpin, HIGH);

      // }

              if (isnan(h) || isnan(t)) 
                 {
                     Serial.println("Failed to read from DHT sensor!");
                      return;
                 }
 
                         if (client.connect(server,80))   //   "184.106.153.149" or api.thingspeak.com
                      {  
                            
                             String postStr = apiKey;
                             postStr +="&field1=";
                             postStr += String(t);
                             postStr +="&field2=";
                             postStr += String(h);
                             postStr +="&field3=";
                             postStr += String(moisture_percentage);
                             postStr += "\r\n\r\n";
 
                             client.print("POST /update HTTP/1.1\n");
                             client.print("Host: api.thingspeak.com\n");
                             client.print("Connection: close\n");
                             client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
                             client.print("Content-Type: application/x-www-form-urlencoded\n");
                             client.print("Content-Length: ");
                             client.print(postStr.length());
                             client.print("\n\n");
                             client.print(postStr);
 
                             Serial.print("Temperature: ");
                             Serial.print(t);
                             Serial.print(" degrees Celcius, Humidity: ");
                             Serial.print(h);
                            Serial.print(" Moisture: ");
                             Serial.print(moisture_percentage);
                             Serial.println("data. Send to Thingspeak.");
                        }
          client.stop();
 
          Serial.println("Waiting...");
  
  // thingspeak needs minimum 15 sec delay between updates
  delay(500); 
 }