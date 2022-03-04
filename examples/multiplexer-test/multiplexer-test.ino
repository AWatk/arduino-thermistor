#include "Thermistor.h"

uint8_t sig = A0;
int s0 = 8;
int s1 = 9;
int s2 = 10;
int s3 = 11;
int numTh = 4;
int en = 7; 

ThermistorArray therms(numTh,sig,en,s0,s1,s2,s3);

void setup()
{
  Serial.begin(115200);
}

void loop()
{
  
  for(uint8_t i = 0; i<numTh; i++)
  {
    
    double val = therms.readCelsius(i);
    Serial.print("Thermistor ");
    Serial.print(i);
    Serial.print(":");
    Serial.println(val);
  }
  

  delay(500);

}