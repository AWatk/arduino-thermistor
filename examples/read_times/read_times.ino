#include "Thermistor.h"

Thermistor thermistor;

void setup(void){
    Serial.begin(115200);
    analogReference(EXTERNAL);
    delay(100);
    double singleRead;
    double avgRead;
    double tstart,t;
    tstart = millis();
    for(int i = 0; i < 99; ++i){
        singleRead = thermistor.readCelsius();
    }
    t = (millis() - tstart) / 100;
    Serial.print("Single read execution time (ms): ");
    Serial.println(t);
    tstart = millis();
    avgRead = thermistor.readCelsiusAvg();
    t = millis() - tstart;
    Serial.print("Avg read execution time (ms): ");
    Serial.println(t);
}

void loop(void){
    
}