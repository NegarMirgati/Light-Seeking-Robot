
#include <Wire.h>
#include <Servo.h> 
#include "I2Cdev.h"
#include "MPU6050.h"



// Create  servo objects
Servo Servo1;
Servo Servo2; 


MPU6050 mpu;


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



int16_t ax, ay, az;
int16_t gx, gy, gz;
float x=0;
int cx=0;
float bx=0;
float rx=0;
float y=0;
int cy=0;
float by=0;
float ry=0;
float z=0;
int cz=0;
float bz=0;
float rz=0;

int robot_init_degree = 0;
int robot_current_degree = 0;

 void move_right()  {
  
    digitalWrite(LeftMotorIN1, HIGH);                              // This is used to turn Left. Notice the 
    digitalWrite(LeftMotorIN2, LOW); 
    
    digitalWrite(RightMotorIN3, LOW);                                // opposite motor runs to turn Left.
    digitalWrite(RightMotorIN4, HIGH); 
    
   // Serial.println("Left");                                      // This prints Left when the robot would actually turn Left.
 }

 void move_left()  {

   digitalWrite(RightMotorIN3, HIGH);                              // This is used to turn Left. Notice the 
   digitalWrite(RightMotorIN4, LOW);    
   digitalWrite(LeftMotorIN1, LOW);                                // opposite motor runs to turn Left.
   digitalWrite(LeftMotorIN2, HIGH);   
    
 //   Serial.println("Right");                                      // This prints Left when the robot would actually turn Left.
 }

 void move_forward()  {
  
    digitalWrite(RightMotorIN3, HIGH);                              // This is used to turn Left. Notice the 
    digitalWrite(RightMotorIN4, LOW); 
    
    digitalWrite(LeftMotorIN1, HIGH);                                // opposite motor runs to turn Left.
    digitalWrite(LeftMotorIN2, LOW); 
    
   // Serial.println("Forward");                                      // This prints Left when the robot would actually turn Left.
 }

 void move_backward()  {
  
    digitalWrite(RightMotorIN3, LOW);                            
    digitalWrite(RightMotorIN4, HIGH); 

    digitalWrite(LeftMotorIN1, LOW);                               
    digitalWrite(LeftMotorIN2, HIGH); 
    
 //   Serial.println("Backward");                                   
 }
 void stop_robot()  {
  
    digitalWrite(RightMotorIN3, HIGH);                             
    digitalWrite(RightMotorIN4, HIGH); 

    digitalWrite(LeftMotorIN1, HIGH);                               
    digitalWrite(LeftMotorIN2, HIGH); 
    
  //  Serial.println("Stop");                                     
 }

int calc_light_max_degree(){
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
   return  degreeMax;
 }


void init_mpu()
{
  if(millis()<2000){
    delay(2000);
    for(int i=0; i<1000; i++) {
      mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
      bx=bx+gx;
      by=by+gy;
      bz=bz+gz;
    }
    bx=bx/10000;
    by=by/10000;
    bz=bz/10000;
  }
}


int get_wheels_degree()
{
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  
  x=x+gx-bx;
  y=y+gy-by;
  z=z+gz-bz;
  
  cz=map(z, -750000, 750000, -90, 90);
  return cz;
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

  mpu.initialize();
   
  Serial.begin(9600);              // Enables a serial connection through the Arduino to either USB or UART (pins 0&1). Note that the baud rate is set to 9600
  Serial.println(" \nBeginning Light Seeking Behavior");   // Placed at the very end of void Setup() so that it is runs once, right before the void Loop()

  Serial.println("Initialize MPU");

  Serial.println(mpu.testConnection() ? "Connected" : "Connection failed");
 
  Wire.begin();

 
}
void loop()    {

   init_mpu();

   
   
   degreeMax = calc_light_max_degree();
   delay(1000);
   Serial.println(degreeMax);
   delay(1000);
   
   if (degreeMax == 0 || degreeMax == 30 || degreeMax == 60  ) 
  {
   
   robot_init_degree = get_wheels_degree();
   while(  abs(robot_current_degree - robot_init_degree)  < (90 - degreeMax) )
   {
  
   move_right();
   
   robot_current_degree = get_wheels_degree(); 
   
   } 

   stop_robot(); 
   
    delay(500);


if(max_in_front) {
      move_forward();
       Serial.println("degreeMax == 0 || degreeMax == 30 || degreeMax == 60   front")   ; 
    }
    else{
       move_backward();
       Serial.println("degreeMax == 0 || degreeMax == 30 || degreeMax == 60   back")   ;
    }
    delay(2000);
    stop_robot();
   
  }
  else if (degreeMax == 90  ) 
  {
    if(max_in_front)
    {
      Serial.println("degreeMax 90   front")   ;
      move_forward();
      delay(2000);
      stop_robot();
    }
    else
    {
      Serial.println("degreeMax 90   back")  ; 
      move_backward();
      delay(2000);
      stop_robot();
    }
  }
    
  
  else if (degreeMax == 120 ||   degreeMax == 150  || degreeMax == 180 )
  {
   robot_init_degree = get_wheels_degree();
   while(  abs(robot_current_degree - robot_init_degree)  < ( degreeMax - 90 ) )
   {
  
   move_left();
  
   robot_current_degree = get_wheels_degree(); 
   
   
   } 

    stop_robot();
    
    delay(500);
   if(max_in_front)
   {
    Serial.println("degreeMax == 120 ||   degreeMax == 150  || degreeMax == 180   front ");
      move_forward();
   }
    else {
        Serial.println("degreeMax == 120 ||   degreeMax == 150  || degreeMax == 180  back");
       move_backward();
    }
    delay(1000);
    stop_robot();
  } 
delay ( 2000);
}
