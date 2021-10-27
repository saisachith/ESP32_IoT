#include <DHT.h>  // Including library for dht
#include<HTTPClient.h> 
#include <WiFi.h>
 
String apiKey = "EO7DPFK4UURXEX62";     //  Enter your Write API key from ThingSpeak
 
const char *ssid =  "Sai Sachith";     // replace with your wifi ssid and wpa2 key
const char *pass =  "123saisachith";
const char* server = "http://api.thingspeak.com/update";
 
#define DHTPIN 4          //pin where the dht11 is connected
 
DHT dht(DHTPIN, DHT11);
 
WiFiClient client;
 
void setup() 
{
       Serial.begin(115200);
       delay(10);
       dht.begin();
 
       Serial.println("Connecting to ");
       Serial.println(ssid);
 
 
       WiFi.begin(ssid, pass);
 
      while (WiFi.status() != WL_CONNECTED) 
     {
            delay(500);
            Serial.print(".");
     }
      Serial.println("");
      Serial.println("WiFi connected");
 
}
 
void loop() 
{
  
      float h = dht.readHumidity();
      float t = dht.readTemperature();
      
              if (isnan(h) || isnan(t)) 
                 {
                     Serial.println("Failed to read from DHT sensor!");
                     return;
                 }
 
              if (WiFi.status() == WL_CONNECTED)   //   "184.106.153.149" or api.thingspeak.com
                 {  
                     HTTPClient http;
                     http.begin(server);
                     String DataSent = "api_key=" + apiKey + "&field1=" + String(t) + "&field2=" + String(h);
                     int Response = http.POST(DataSent);
                     Serial.print("Temperature: ");
                     Serial.print(t);
                     Serial.print(" degrees Celcius, Humidity: ");
                     Serial.print(h);
                     Serial.println("%. Send to Thingspeak.");
                     http.end();
                        }
          client.stop();
 
          Serial.println("Waiting...");
  
  // thingspeak needs minimum 15 sec delay between updates
  delay(1000);
}
