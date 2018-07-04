#include <SoftwareSerial.h>

SoftwareSerial BTSerial(10, 11); // RX | TX
int min_Moisture;
char bt_val;
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

void setup()
{
  Serial.begin(9600);
  Serial.println("Start Bluetooth");
  BTSerial.begin(9600);
  BTSerial.write("Enter minimum moisture value : ");
  BTSerial.write("**Enter 1 for low moisture \n");
  BTSerial.write("**Enter 2 for medium moisture \n");
  BTSerial.write("**Enter 3 for high moisture \n");
}

void loop(){
  get_min_moisture_from_user();
  Serial.println("value successfully received");
  Serial.println(min_Moisture);
}
