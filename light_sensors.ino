/*
  Example of BH1750 library usage.
  This example initialises the BH1750 object using the default high resolution
  continuous mode and then makes a light level reading every second.
  Connection:
    VCC -> 3V3 or 5V
    GND -> GND
    SCL -> SCL (A5 on Arduino Uno, Leonardo, etc or 21 on Mega and Due, on esp8266 free selectable)
    SDA -> SDA (A4 on Arduino Uno, Leonardo, etc or 20 on Mega and Due, on esp8266 free selectable)
    ADD -> (not connected) or GND
  ADD pin is used to set sensor I2C address. If it has voltage greater or equal to
  0.7VCC voltage (e.g. you've connected it to VCC) the sensor address will be
  0x5C. In other case (if ADD voltage less than 0.7 * VCC) the sensor address will
  be 0x23 (by default).
*/

#include <Wire.h>
#include <BH1750.h>

BH1750 lightMeter;
//int digital_out_3 , digital_out_5 , temp;
int i = 0;


void setup(){

  Serial.begin(9600);

  
  //digital_out_3 = 3;
 // digital_out_5 = 5;
  pinMode(3, OUTPUT);   // gy-front
  pinMode(5, OUTPUT);   // gy-back
 
 

  // Initialize the I2C bus (BH1750 library doesn't do this automatically)
   Wire.begin();
  // On esp8266 you can select SCL and SDA pins using Wire.begin(D4, D3);
  //Wire.begin(D4, D3);

  lightMeter.begin();
 

  Serial.println(F("BH1750 Test begin"));

}


void loop() {

  uint16_t lux = lightMeter.readLightLevel();
  if ( i % 2 == 0 )
  {
    digitalWrite(3, LOW);       // sets the digital_out_3 on -> front
    digitalWrite(5, HIGH);       // sets the digital_out_5 off -> back
  }
  else 
  {
    digitalWrite(3, HIGH);       // sets the digital_out_3 on -> front
    digitalWrite(5, LOW);       // sets the digital_out_5 off -> back
  }
  
  Serial.print("Light: ");
  Serial.print(lux);
  Serial.println(" lx");
  Serial.println("now change...............");
  i = i+1;
  delay(1000);

}
