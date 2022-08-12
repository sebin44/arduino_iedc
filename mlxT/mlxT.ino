

/*
* Non-contact Thermometer with GY - 906 module
* Support for the MLX90614 sensor on the I2C bus
* SDA line = A4
* SCL line = A5
* Sensor supply with 5V
*/


#include <LiquidCrystal.h>
#include <MLX90614.h>

LiquidCrystal lcd (8, 9, 4, 5, 6, 7);

int address = 0xb4; // Sensor address MLX90614
int erc = 0; // Variable holding the PEC value
int dataH = 0; // The second byte of data
int dataL = 0; // The first byte of data
double tempnalsb = 0.02; // Variable by which the digital value will be multiplied
double temperature = 0; // Variable holding the temperature
    
    
void setup () {
  lcd.begin (16, 2); // Initialize the display
  
  
}




void loop () {
 
  
  temperature = (double) (((dataH & 0x007F) << 8) + dataL); // Create a 16-bit variable consisting of two one-byte variables
  temperature = temperature * tempnalsb; // For one bit 0.02 K, the result of this operation is the temperature in Kelvin
  
  temperature = temperature - 273.15; // Conversion to Celsius degrees
 
  
  
  temperature = (double) (((dataH & 0x007F) << 8) + dataL);
  temperature = temperature * tempnalsb;
  temperature = temperature - 273.15;
   
  lcd.setCursor(0,1); // Display (second LCD line)
  lcd.print ("Ambient =");
  lcd.print (temperature);
  lcd.print ("");
  lcd.write (0xDF);
  lcd.print ("C");
  Serial.print("Current Temperature: ");
  Serial.println(temperature);

delay (200); // Delay 200ms
}
