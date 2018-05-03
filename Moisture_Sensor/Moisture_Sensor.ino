int Moisture_ain=A0;
int ad_value;
void setup()
{
  pinMode(Moisture_ain,INPUT);
  Serial.begin(9600);
}
void loop()
{
  ad_value=analogRead(Moisture_ain);
  Serial.println(ad_value);
  if(ad_value>200)
  {
    Serial.println("Eat drink");
  }
  else
  {
    Serial.println("Thirsty");
  }
  delay(500);
}
