#define BLUETOOTH_SPEED 9600 
#include <SoftwareSerial.h>

SoftwareSerial mySerial(0, 1); 
char data;  
void setup() {
 
  Serial.begin(9600);
  mySerial.begin(BLUETOOTH_SPEED);
  delay(1000);

}

void loop() {
     if(mySerial.available() > 0){
      data = mySerial.read();        
      Serial.print(data);  
      mySerial.write(data);        
  }
}
