
/****************************************************************************************************************
 *    Program for a simple light Seeking Robot.
 *    created by: Anuj
 *    (c)copyright
 *
 *    Robotic base should include differential drive.
 *
 *    Physical setup:
 *    Two LDRs connected to ground and Analog pin 1(Right) and Analog pin 2(left)
 *    base of NPN Transistors on pins 11(right) and 12(left).
 *    
 *
 ******************************************************************************************************************/
#include <Wire.h>
#include <BH1750.h>
#include <Servo.h> 


BH1750 lightMeter;
//int digital_out_3 , digital_out_5 , temp;
int i = 0;

// Create a servo object 
Servo Servo1; 
// Declare the Servo pin 
int servoPin = 5; 

// Pin definitions - attaches a variable to a pin.
const int RightMotor = 2;    // This pin is used to enable or disable the Right motor. Connected to the base of an NPN transistor.
const int LeftMotor = 3;     // This pin is used to enable or disable the Left motor. Connected to the base of an NPN transistor.
const int RightSensor = 1;    // This pin is used to read the value of the Right Sensor.
const int LeftSensor = 4;     // This pin is used to read the value of the Left Sensor.

// Variable definitions
int degreeMax;
int maxLX;
int SensorDifference;   // This value is used to determine the difference between the Left and Right

// the setup() method runs once when the program is run. When the
// Arduino is reset, the setup() will be executed once again.

void setup()   {                
  //for bh1750
  pinMode(6, OUTPUT);   // gy-front
  pinMode(5, OUTPUT);   // gy-back
 
  // We need to attach the servo to the used pin number 
  Servo1.attach(servoPin);
   
  pinMode(LeftMotor, OUTPUT);      // Defines this pin as an output. The Arduino will write values to this pin.
  pinMode(RightMotor, OUTPUT);     // Defines this pin as an output. The Arduino will write values to this pin.
  pinMode(LeftSensor, INPUT);      // Defines this pin as an input.  The Arduino will read values from this pin.
  pinMode(RightSensor, INPUT);     // Defines this pin as an input.  The Arduino will read values from this pin.
  digitalWrite(A1, HIGH);          // Enables an internal pullup resistor
  digitalWrite(A2, HIGH);          // Enables an internal pullup resistor
  Serial.begin(9600);              // Enables a serial connection through the Arduino to either USB or UART (pins 0&1). Note that the baud rate is set to 9600
  Serial.println(" \nBeginning Light Seeking Behavior");   // Placed at the very end of void Setup() so that it is runs once, right before the void Loop()
  
  Serial.println(F("BH1750 Test begin"));

  // Initialize the I2C bus (BH1750 library doesn't do this automatically)
   Wire.begin();
  // On esp8266 you can select SCL and SDA pins using Wire.begin(D4, D3);
  //Wire.begin(D4, D3);
  //Uno, Ethernet  A4 (SDA), A5 (SCL)

  lightMeter.begin();
 

}

// the loop() method runs over and over again,
// as long as the Arduino has power

void loop()    {
    maxLX = 0 ; 
   for(int j = 0 ; j < 10 ; j++)
   {
      Servo1.write(j*18);
      delay(100);
       uint16_t lux = lightMeter.readLightLevel();
      Serial.print("Light: ");
      Serial.print(lux);
      Serial.println(" lx");
      Serial.println("now change...............");
      if(lux > maxLX) 
      {
        maxLX = lux;
        degreeMax = j*18;
      }
      delay(1000);
   }

  //if ( i % 2 == 0 )
  //{
    //digitalWrite(3, LOW);       // sets the digital_out_3 on -> front
   // digitalWrite(5, HIGH);       // sets the digital_out_5 off -> back
  //}
  //else 
  //{
 //   digitalWrite(3, HIGH);       // sets the digital_out_3 on -> front
//    digitalWrite(5, LOW);       // sets the digital_out_5 off -> back
//  }


  // This section of the sketch is what actually interperets the data and then runs the motors accordingly.

  if (degreeMax > 90 ) {    // This is interpreted as if the Left sensor reads more light than the Right Sensor, Do this:
    digitalWrite(RightMotor, HIGH);                              // This is used to turn Left. Notice the 
    digitalWrite(LeftMotor, LOW);                                // opposite motor runs to turn Left.
    Serial.println("Left");                                      // This prints Left when the robot would actually turn Left.
  }

  if (degreeMax <90) {    // This is interpreted as if the Left sensor reads less light than the Right Sensor, Do this:
    digitalWrite(RightMotor, LOW);                               // This is used to turn Right. Notice the 
    digitalWrite(LeftMotor, HIGH);                               // opposite motor runs to turn Right.
    Serial.println("Right");                                     // This prints Right when the robot would actually turn Right.
  }

  else if (degreeMax == 90 ) {                                // This is interpreted as if the difference between the two sensors is under 125 (Experiment to suit our sensors), Do this:
    digitalWrite(RightMotor, HIGH);                              // This is used to go straight. Notice
    digitalWrite(LeftMotor, HIGH);                               // both motors are enabled to go forward.
    Serial.println("Forward");                                   // This prints Forward when the robot would actually go forward.
  }
  Serial.print("\n");
}










