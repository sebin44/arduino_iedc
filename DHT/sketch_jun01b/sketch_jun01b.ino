#include "dht.h"
#define dht_apin A0 // Analog Pin sensor is connected to
const int relayPin = 8;
dht DHT;
 
void setup(){
 
  Serial.begin(9600);
  delay(500);//Delay to let system boot
  pinMode(relayPin, OUTPUT);
  Serial.println("DHT11 Humidity & temperature Sensor\n\n");
  delay(1000);//Wait before accessing Sensor
 
}//end "setup()"
 
void loop(){
  //Start of Program 
 
    DHT.read11(dht_apin);
    
    Serial.print("Current humidity = ");
    Serial.print(DHT.humidity);
    Serial.print("%  ");
    Serial.print("temperature = ");
    Serial.print(DHT.temperature); 
    Serial.println("C  ");
    delay(1000);//Wait 5 seconds before accessing sensor again.
    if (DHT.temperature <= 38) {// you can change temperature value here - t>= 30 to your preferred number or change from Celsius to Fahrenheit readings
    digitalWrite(relayPin, HIGH);
  }
  else {
    digitalWrite(relayPin, LOW);
  }
  delay(1000);
 
  //Fastest should be once every two seconds.
 
}// end loop() 
