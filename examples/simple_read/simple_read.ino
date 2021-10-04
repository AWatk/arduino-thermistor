#include "Thermistor.h"

Thermistor thermistor;

void setup(void){
    Serial.begin(115200);
    analogReference(EXTERNAL);
}

void loop(void){
    
    serial.println(Thermistor.readCelsiusAvg());
    delay(1000);

}