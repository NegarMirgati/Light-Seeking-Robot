#include "Wire.h"
#include "I2Cdev.h"
#include "MPU6050.h"
#include  <Servo.h>


MPU6050 mpu;

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
void setup()
{
  Serial.begin(9600);
  
  Serial.println("Initialize MPU");
  mpu.initialize();
  Serial.println(mpu.testConnection() ? "Connected" : "Connection failed");
 
  Wire.begin();

}

void loop(){

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

  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  
  x=x+gx-bx;
  y=y+gy-by;
  z=z+gz-bz;
  
  cx=map(x, -750000, 750000, -90, 90);
  cy=map(y, -750000, 750000, -90, 90);
  cz=map(z, -750000, 750000, -90, 90);
  
  Serial.println(cz);

}
