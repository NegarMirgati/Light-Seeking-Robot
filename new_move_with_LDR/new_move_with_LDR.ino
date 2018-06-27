#include <Wire.h>
#include <Servo.h> 

// Create  servo objects
Servo Servo1;
Servo Servo2; 

// Declare the Servo pins
int servoPin1 = 9; 
int ServoPin2 = 10;

// Declare Sr04 pins
int echoPin1 = 15;
int trigPin1 = 16;

int echoPin2 = 11;
int trigPin2 = 13;

// Declare Motor Pins
const int RightMotorIN3 = 5;    // This pin is used to enable or disable the Right motor. Connected to the base of an NPN transistor.
const int RightMotorIN4 = 3;    // This pin is used to enable or disable the Right motor. Connected to the base of an NPN transistor.
const int LeftMotorIN1 = 7;     // This pin is used to enable or disable the Left motor. Connected to the base of an NPN transistor.
const int LeftMotorIN2 = 6;     // This pin is used to enable or disable the Left motor. Connected to the base of an NPN transistor.

// Declare photoresistor pins
const int FrontSensor = 14;
const int BackSensor = A3 ;

// Variable definitions
int degreeMax;
bool max_in_front;
int maxLX;
int SensorDifference;   // This value is used to determine the difference between the Left and Right

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

   for(int j = 0; j <= 6; j++){
      
      Servo1.write(j*30); 
      Servo2.write(j*30);    
      delay(100);
      
      
       uint16_t luxb = analogRead(BackSensor);
      
      delay(200); 
       
      uint16_t luxf = analogRead(FrontSensor);
      
      Serial.print("Light F: ");
      Serial.print(luxf);
      Serial.println(" lx");
      Serial.print("Light B: ");
      Serial.print(luxb);
      Serial.println(" lx");
      Serial.println("the other sensor");
    if(luxb > maxLX) 
      {
        Serial.print(" Current max in Back :  ");
        Serial.println(luxb);
        maxLX = luxb;
        degreeMax = j*30;
        max_in_front = false;
        Serial.print("maxLX : ");
        Serial.println(maxLX);
        
      }
      if(luxf > maxLX) 
      {
        Serial.print(" Current max in Front :  ");
        Serial.print(luxf);
        maxLX = luxf;
        degreeMax = j*30;
        max_in_front = true;
        Serial.print("maxLX : ");
        Serial.println(maxLX);
        
      }
      delay(1000);
   }
 }

void setup()   { 
  /* We connected ENA and ENB to VCC*/
  //analogWrite(ENA, 255);   
  //analogWrite(ENB, 255); 

  pinMode(LeftMotorIN1, OUTPUT);      // Defines this pin as an output. The Arduino will write values to this pin.
  pinMode(LeftMotorIN2, OUTPUT);      // Defines this pin as an output. The Arduino will write values to this pin.
  pinMode(RightMotorIN3, OUTPUT);     // Defines this pin as an output. The Arduino will write values to this pin.
  pinMode(RightMotorIN4, OUTPUT);     // Defines this pin as an output. The Arduino will write values to this pin.

  // SR05 setup
  pinMode(trigPin1, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin1, INPUT); // Sets the echoPin as an Input
  pinMode(trigPin2, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin2, INPUT); // Sets the echoPin as an Input
 
  // We need to attach the servo to the used pin number 
  Servo1.attach(servoPin1);
  Servo2.attach(ServoPin2);
   
  Serial.begin(9600);              // Enables a serial connection through the Arduino to either USB or UART (pins 0&1). Note that the baud rate is set to 9600
  Serial.println(" \nBeginning Light Seeking Behavior");   // Placed at the very end of void Setup() so that it is runs once, right before the void Loop()
 
}
void loop()    {

   calc_degree();
   if (degreeMax >= 0 &&  degreeMax < 90 ) {    
    move_left();
    delay(2000);
   
  }
  else if (degreeMax > 90 &&  degreeMax < 180) {
    if(max_in_front)
      move_right();
    delay(2000);
  }
  else if (degreeMax == 90  ) {         
    if(max_in_front)                      
      move_forward();
     else
      move_backward();
      delay(2000);
  } 
}
