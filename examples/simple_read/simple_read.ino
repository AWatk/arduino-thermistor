#include "Thermistor.h"

Thermistor therm;

void setup(void){
    Serial.begin(115200);
}

void loop(void){

    Serial.println(therm.readCelsiusAvg());
    delay(1000);

}