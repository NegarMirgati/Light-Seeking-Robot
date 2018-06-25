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

int i = 0;
int flag; // 0 -> front , 1 -> left

// Create  servo objects
Servo Servo1;
Servo Servo2; 

// Declare the Servo pins
int servoPin1 = 9; 
int ServoPin2 = 10;

const int RightMotorIN3 = 5;    // This pin is used to enable or disable the Right motor. Connected to the base of an NPN transistor.
const int RightMotorIN4 = 4;    // This pin is used to enable or disable the Right motor. Connected to the base of an NPN transistor.
const int LeftMotorIN1 = 7;     // This pin is used to enable or disable the Left motor. Connected to the base of an NPN transistor.
const int LeftMotorIN2 = 6;     // This pin is used to enable or disable the Left motor. Connected to the base of an NPN transistor.

const int BackSensor = 11;
const int FrontSensor = 8 ;


//const int ENA = 10;
//const int ENB = 3;

// Variable definitions
int degreeMax;
bool max_in_front;
int maxLX;
int SensorDifference;   // This value is used to determine the difference between the Left and Right

// the setup() method runs once when the program is run. When the
// Arduino is reset, the setup() will be executed once again.


 void move_left()  {
  
    digitalWrite(LeftMotorIN1, HIGH);                              // This is used to turn Left. Notice the 
    digitalWrite(LeftMotorIN2, LOW); 
    
    digitalWrite(RightMotorIN3, LOW);                                // opposite motor runs to turn Left.
    digitalWrite(RightMotorIN4, HIGH); 
    
    Serial.println("Left");                                      // This prints Left when the robot would actually turn Left.
 }

 void move_right()  {

    digitalWrite(RightMotorIN3, HIGH);                              // This is used to turn Left. Notice the 
    digitalWrite(RightMotorIN4, LOW); 
    
    digitalWrite(LeftMotorIN1, LOW);                                // opposite motor runs to turn Left.
    digitalWrite(LeftMotorIN2, HIGH); 
    
    
    Serial.println("Right");                                      // This prints Left when the robot would actually turn Left.
 }

 void move_forward()  {
  
    digitalWrite(RightMotorIN3, HIGH);                              // This is used to turn Left. Notice the 
    digitalWrite(RightMotorIN4, LOW); 
    
    digitalWrite(LeftMotorIN1, HIGH);                                // opposite motor runs to turn Left.
    digitalWrite(LeftMotorIN2, LOW); 
    
    Serial.println("Forward");                                      // This prints Left when the robot would actually turn Left.
 }

 void move_backward()  {
  
    digitalWrite(RightMotorIN3, LOW);                              // This is used to turn Left. Notice the 
    digitalWrite(RightMotorIN4, HIGH); 
    
    digitalWrite(LeftMotorIN1, LOW);                                // opposite motor runs to turn Left.
    digitalWrite(LeftMotorIN2, HIGH); 
    
    Serial.println("Backward");                                      // This prints Left when the robot would actually turn Left.
 }

void calc_degree(){
   Serial.println(" Calc_degree called");
   Servo1.write(0); 
   Servo2.write(0);
   maxLX = 0 ; 
   degreeMax = 0;
   //for(int cnt = 0 ; cnt <= 1; cnt++){
   for(int j = 0; j <= 6; j++){
      
      /*(cnt == 0) ? */
      Servo1.write(j*30); 
      Servo2.write(j*30);    
      delay(100);
      
      digitalWrite(FrontSensor, LOW);       // sets the digital_out_11 on -> front
      digitalWrite(BackSensor, HIGH);       // sets the digital_out_8 off -> back
      uint16_t luxb = lightMeter.readLightLevel();
      
      delay(200); 
         
      digitalWrite(FrontSensor, HIGH);       // sets the digital_out_11 on -> front
      digitalWrite(BackSensor, LOW);       // sets the digital_out_8 off -> back
      uint16_t luxf = lightMeter.readLightLevel();
      
      /*Serial.print("Light F: ");
      Serial.print(luxf);
      Serial.println(" lx");
      Serial.print("Light B: ");
      Serial.print(luxb);
      Serial.println(" lx");
      Serial.println("the other sensor");*/
      if(luxb > maxLX /*&& (cnt == 1)*/) 
      {
        Serial.print(" Current max in Back :  ");
        Serial.println(luxb);
        maxLX = luxb;
        degreeMax = j*30;
        flag = 1;
        max_in_front = false;
        Serial.print("maxLX : ");
        Serial.println(maxLX);
        
      }
      if(luxf > maxLX /*&& (cnt == 0)*/) 
      {
        Serial.print(" Current max in Front :  ");
        Serial.print(luxf);
        maxLX = luxf;
        degreeMax = j*30;
        flag = 0;
        max_in_front = true;
        Serial.print("maxLX : ");
        Serial.println(maxLX);
        
      }
      delay(1000);
   }
   if ( flag == 1 ){
    degreeMax = degreeMax + 180;
   }
 // }
 }

void setup()   { 
  /* We connected ENA and ENB to VCC*/
  //analogWrite(ENA, 255);   
  //analogWrite(ENB, 255); 

  pinMode(LeftMotorIN1, OUTPUT);      // Defines this pin as an output. The Arduino will write values to this pin.
  pinMode(LeftMotorIN2, OUTPUT);      // Defines this pin as an output. The Arduino will write values to this pin.
  pinMode(RightMotorIN3, OUTPUT);     // Defines this pin as an output. The Arduino will write values to this pin.
  pinMode(RightMotorIN4, OUTPUT);     // Defines this pin as an output. The Arduino will write values to this pin.
        
  //BH1750 setup
  pinMode(FrontSensor, OUTPUT);   // gy-front
  pinMode(BackSensor, OUTPUT);   // gy-back
 
  // We need to attach the servo to the used pin number 
  Servo1.attach(servoPin1);
  Servo2.attach(ServoPin2);
   
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
void loop()    {

   calc_degree();

////// cm to test servo
/*
 // if (degreeMax >= 0 &&  degreeMax < 90 ) {    

    move_left();
    delay(2000);
   
 // }

 // else if (degreeMax > 90 &&  degreeMax < 180 ) {    
 
    move_right();
    delay(2000);
 // }

 // else if (degreeMax == 90 ) {                               

    move_forward();
     delay(2000);
 // }
  
 //  else if (degreeMax == 270 ) {                               

    move_backward();
     delay(2000);
 // }
 */ 
}

