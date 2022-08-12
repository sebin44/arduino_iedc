#include <ThingSpeak.h>
#include "Adafruit_MLX90614.h"
#include <ESP8266WiFi.h>
Adafruit_MLX90614 mlx = Adafruit_MLX90614();



String apiKey = "5GRWAP501IZ7P6QI";     //  Enter your Write API key from ThingSpeak

const char *ssid =  "D-Link Home";     // replace with your wifi ssid and wpa2 key
const char *pass =  "sunny123";
const char* server = "api.thingspeak.com";


WiFiClient client;
 
void setup() 
{
       Serial.begin(115200);
       delay(10);
       mlx.begin();
 
       Serial.println("Connecting to ");
       Serial.println(ssid);
       Serial.println("Adafruit MLX90614 test w/ Thingspeak");
 
 
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
  Serial.print("Ambient = "); Serial.print(mlx.readAmbientTempC()); 
  Serial.print("*C\tObject = "); Serial.print(mlx.readObjectTempC()); Serial.println("*C");
  Serial.print("Ambient = "); Serial.print(mlx.readAmbientTempF()); 
  Serial.print("*F\tObject = "); Serial.print(mlx.readObjectTempF()); Serial.println("*F");
  Serial.println();
  delay(1000);

  Serial.print("logSensorData() - connecting to ");

                         if (client.connect(server,80))   //   "184.106.153.149" or api.thingspeak.com
                      {  
                            
                             String postStr = apiKey;
                             postStr +="&field1=";
                             postStr += String(mlx.readObjectTempC());
                             postStr +="&field2=";
                             postStr += String(mlx.readAmbientTempC());
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
                             Serial.print(mlx.readObjectTempC());
                             Serial.print(" degrees Celcius, Humidity: ");
                             Serial.print(mlx.readAmbientTempC());
                             Serial.println("%. Send to Thingspeak.");
                        }
          client.stop();
 
          Serial.println("Waiting...");
  
  // thingspeak needs minimum 15 sec delay between updates
  delay(1000);
}
