#include <Wire.h>
#include <Servo.h> 
#include <SoftwareSerial.h>
#include "I2Cdev.h"
#include "MPU6050.h"

// Create  servo objects
Servo Servo1;
Servo Servo2; 

// Declare the Servo pins
int servoPin1 = 9; 
int ServoPin2 = 10;

//mpu interrupt pin = 2 
MPU6050 mpu;

// Declare Sr04 pins
int echoPin1 = 12;
int trigPin1 = A2;

int echoPin2 = 11;
int trigPin2 = 13;

// Declare Motor Pins
const int RightMotorIN3 = 5;    // This pin is used to enable or disable the Right motor. Connected to the base of an NPN transistor.
const int RightMotorIN4 = 3;    // This pin is used to enable or disable the Right motor. Connected to the base of an NPN transistor.
const int LeftMotorIN1 = 7;     // This pin is used to enable or disable the Left motor. Connected to the base of an NPN transistor.
const int LeftMotorIN2 = 6;     // This pin is used to enable or disable the Left motor. Connected to the base of an NPN transistor.

// Declare photoresistor pins
const int FrontSensor = A0;
const int BackSensor = A3 ;

// Variable definitions
int degreeMax = -1;
bool max_in_front;
int maxLX;
int distance;
long duration;
int front_distance;
int back_distance;


// Variable for mpu 
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

//Variable use for rotation of robot 
int robot_init_degree = 0;
int robot_current_degree = 0;

// Water pomp 
int Pomp = 8 ;

// Moisture sensor
 int Moisture_ain=A1;

int ad_value;
uint16_t measureMoistureTime = 0 ; 

// variables for bluetooth and moisture value 
SoftwareSerial BTSerial(10, 11); // RX | TX
int min_Moisture;
char bt_val;

 void move_right()  {

    digitalWrite(RightMotorIN3, LOW);            
    digitalWrite(RightMotorIN4, HIGH); 
  
    digitalWrite(LeftMotorIN1, HIGH);                           
    digitalWrite(LeftMotorIN2, LOW); 
 }

 void move_left()  {

   digitalWrite(RightMotorIN3, HIGH);                         
   digitalWrite(RightMotorIN4, LOW);    
  
   digitalWrite(LeftMotorIN1, LOW);                             
   digitalWrite(LeftMotorIN2, HIGH);   
 }

 void move_forward()  {
  
    digitalWrite(RightMotorIN3, HIGH);                         
    digitalWrite(RightMotorIN4, LOW); 
    
    digitalWrite(LeftMotorIN1, HIGH);                            
    digitalWrite(LeftMotorIN2, LOW); 
 }

 void move_backward()  {
  
    digitalWrite(RightMotorIN3, LOW);                            
    digitalWrite(RightMotorIN4, HIGH); 

    digitalWrite(LeftMotorIN1, LOW);                               
    digitalWrite(LeftMotorIN2, HIGH); 
                                      
 }
 void stop_robot()  {
  
    digitalWrite(RightMotorIN3, HIGH);                             
    digitalWrite(RightMotorIN4, HIGH); 

    digitalWrite(LeftMotorIN1, HIGH);                               
    digitalWrite(LeftMotorIN2, HIGH);                                    
 }

 void get_min_moisture_from_user(){
  while(true){
     // Keep reading from HC-05 and send to Arduino Serial Monitor
  if (BTSerial.available()){
     
     bt_val = BTSerial.read();
     Serial.println(bt_val);
     if(bt_val == '1' || bt_val == '2' || bt_val == '3'){
     if(bt_val == '1')  
      min_Moisture = 550;
     else if(bt_val == '2') 
      min_Moisture = 600;
      else
        min_Moisture = 650;
         
      Serial.println(min_Moisture);
      BTSerial.end();
      break;
     }
     
     else
     BTSerial.write("Invalid Arguement, please try again");  
  }

  // Keep reading from Arduino Serial Monitor and send to HC-05
  if (Serial.available())
    BTSerial.write(Serial.read());
     }
     return;
  }


int calc_light_max_degree(int luxes_and_degrees[14][3]){
   Serial.println(" Calc_degree called");
   Servo1.write(0); 
   Servo2.write(0);
   maxLX = 0 ; 
   degreeMax = -1;



for(int j = 0; j <= 6; j++){
      
      Servo1.write(j*30); 
      Servo2.write(j*30);   
       
      delay(100);
      uint16_t luxb = analogRead(BackSensor);
      delay(100); 
      uint16_t luxf = analogRead(FrontSensor);
      
      back_distance = 2001 ;
      int  count = 0 ;
      while  ( back_distance > 2000 && count < 11  ) {
         back_distance  = calc_distance(2);
         count++;
      }
      
        if ( back_distance > 60){
          luxes_and_degrees[j+7][0] = luxb;
          if (luxes_and_degrees[j+7][1] == 2) {
            (luxes_and_degrees[j+7][1] = 2) ; 
          }
          else if ( luxes_and_degrees[j+7][1] != 2 )
          {
            (luxes_and_degrees[j+7][1] = 1);
          }
          luxes_and_degrees[j+7][2] = back_distance;
        }
        
        else if(back_distance <= 60 && back_distance >=30){
          luxes_and_degrees[j+7][0] = luxb;
          luxes_and_degrees[j+7][1] = 0;
          luxes_and_degrees[j+7][2] = back_distance;
        }
        
        else if(back_distance < 30){
          if(j+7-1 >= 7){
            luxes_and_degrees[j+7][1] = 0;
            luxes_and_degrees[j+7-1][1] = 0;
            luxes_and_degrees[j+7][2] = back_distance;
          }
       if(j+7+1 <= 13){
            luxes_and_degrees[j+7][1] = 0;
            luxes_and_degrees[j+7][2] = back_distance;
            luxes_and_degrees[j+7+1][1] = 2;
          }
        }
        front_distance = 2001 ;
        count = 0;
        while  (  front_distance > 2000 && count < 11 ) 
        {
          front_distance  = calc_distance(1);
          count++;
        }
       if ( front_distance > 60 ){
          luxes_and_degrees[j][0] = luxf;
          if (luxes_and_degrees[j][1] == 2) {
            (luxes_and_degrees[j][1] = 2);
          }
          else if (luxes_and_degrees[j][1] != 2) {
            (luxes_and_degrees[j][1] = 1);
          }
           luxes_and_degrees[j][2] = front_distance;
        }
        else if(front_distance <= 60 && front_distance >= 25){
          luxes_and_degrees[j][0] = luxf;
          luxes_and_degrees[j][1] = 0;
          luxes_and_degrees[j][2] = front_distance;
        }
        else if(front_distance < 25){
          if(j-1 >= 0){
            luxes_and_degrees[j][1] = 0;
            luxes_and_degrees[j-1][1] = 0;
            luxes_and_degrees[j][2] = front_distance;
          }
        if(j+1 <= 6){
             luxes_and_degrees[j][1] = 0;
              luxes_and_degrees[j][2] = front_distance;
            luxes_and_degrees[j+1][1] = 2;
            Serial.print(" Changing ");  Serial.print(j+1);  Serial.print("to 2");
          }
        } 
      delay(1000);
   }
   maxLX = 0;
   for(int cnt = 0; cnt <= 13; cnt++){
    if ( cnt <= 6 )
    {
      Serial.println(cnt*30);
    }
    
     Serial.print("luxes_and_degrees[cnt][0] " );
     Serial.println(luxes_and_degrees[cnt][0]);
     Serial.print("luxes_and_degrees[cnt][1] " );
     Serial.println(luxes_and_degrees[cnt][1]);
     Serial.print("luxes_and_degrees[cnt][2] " );
     Serial.println(luxes_and_degrees[cnt][2]);
      if( (luxes_and_degrees[cnt][1] == 1) && (luxes_and_degrees[cnt][0] > maxLX)){
        maxLX = luxes_and_degrees[cnt][0];
        if ( cnt <= 6 ){
           degreeMax = cnt * 30;
           max_in_front = true;
        }
        else if ( cnt >= 7 ){
          degreeMax = (cnt - 7) * 30;
           max_in_front = false;
        }
      }
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
  
  cz=map(z, -750000, 750000, -90, 90)*2;
  return cz;
}


int calc_distance(int mode)
{
  // Clears the trigPin 
  if ( mode == 1)
    digitalWrite(trigPin1, LOW); 
  else if ( mode == 2 )
     digitalWrite(trigPin2, LOW); 
  delayMicroseconds(20); 
  // Sets the trigPin on HIGH state for 10 micro seconds 
  if ( mode == 1)
    digitalWrite(trigPin1, HIGH); 
  else if ( mode == 2 ) 
    digitalWrite(trigPin2, HIGH); 
  delayMicroseconds(100); 
  if ( mode == 1)
    digitalWrite(trigPin1, LOW); 
  else if ( mode == 2 )  
    digitalWrite(trigPin2, LOW); 
  // Reads the echoPin, returns the sound wave travel time in microseconds 
  if ( mode == 1)
    duration = pulseIn(echoPin1, HIGH);
  else if ( mode == 2 )  
    duration = pulseIn(echoPin2, HIGH);
  // Calculating the distance 
  distance= duration*0.034/2;
  // Prints the distance on the Serial Monitor 
  Serial.print("Distance: "); 
  Serial.println(distance);
  return distance;
}



void setup()   { 
  
  pinMode(Moisture_ain,INPUT);
  pinMode(Pomp,OUTPUT);


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
  
  Serial.begin(9600); 
  
  Serial.println("Start Bluetooth");
  BTSerial.begin(9600);
  BTSerial.write("Enter minimum moisture value : ");
  BTSerial.write("**Enter 1 for low moisture \n");
  BTSerial.write("**Enter 2 for medium moisture \n");
  BTSerial.write("**Enter 3 for high moisture \n");
  Serial.println("Initialize MPU");

  get_min_moisture_from_user();
  Serial.println("value successfully received");
  Serial.println(min_Moisture);
  
  mpu.initialize();
  Serial.println(mpu.testConnection() ? "Connected" : "Connection failed"); 
 
  Serial.println(" \nBeginning Light Seeking Behavior");   // Placed at the very end of void Setup() so that it is runs once, right before the void Loop()
  
  Wire.begin();
}


void loop()    {
    int  luxes_and_degrees[14][3]; 
    init_mpu();
   
   if(measureMoistureTime % 150 == 0 ) {
        ad_value=analogRead(Moisture_ain);
        Serial.print(ad_value);
        Serial.print ("          sldkjflksjlkfjkljsfkljslfjsljflksjfl " );
      if(ad_value < min_Moisture){
        digitalWrite(Pomp,HIGH);
        Serial.println("Thirsty");
        while(ad_value < min_Moisture){
           ad_value=analogRead(Moisture_ain);
           delay(100);
        }
        digitalWrite(Pomp,LOW);
      }
  delay(100);
  }


  
   degreeMax = calc_light_max_degree(luxes_and_degrees);
   
   if (degreeMax == 0 || degreeMax == 30 || degreeMax == 60  ) {
   robot_init_degree = get_wheels_degree();
   Serial.println(robot_init_degree);
   while(  abs(robot_current_degree - robot_init_degree)  < (100 - degreeMax) ){
    Serial.print("init : ");
    Serial.print(robot_init_degree);
    Serial.println("current : " );
    Serial.println(robot_current_degree);
    move_left();
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
  else if (degreeMax == 90  ) {
    if(max_in_front){
      Serial.println("degreeMax 90   front")   ;
      move_forward();
      delay(2000);
      stop_robot();
    }
    else{
      Serial.println("degreeMax 90   back")  ; 
      move_backward();
      delay(2000);
      stop_robot();
    }
  }
    
  
  else if (degreeMax == 120 ||   degreeMax == 150  || degreeMax == 180 ){
   robot_init_degree = get_wheels_degree();
   while(  abs(robot_current_degree - robot_init_degree)  < ( degreeMax - 85 ) ){
      Serial.print("init : ");
      Serial.print(robot_init_degree);
      Serial.println("current : " );
      Serial.println(robot_current_degree);
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
delay ( 1000);
measureMoistureTime++;
}
