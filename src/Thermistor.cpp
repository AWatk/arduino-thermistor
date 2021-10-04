#include <Arduino.h>
#include "Thermistor.h"

Thermistor::Thermistor(void)
{
    //construct with default settings

    settings.nominal_temperature_c = 25;
    settings.nominal_resistance = 10000;
    settings.b_coefficient = 3950;
    settings.thermistor_pin = A0;
    settings.series_resistor = 10000;
    settings.num_samples = 5;
    settings.read_delay = 1;
    settings.adc_resolution = 1023;
}

inline double Thermistor::readThermistorResistance()
{
    return settings.series_resistor/(double(settings.adc_resolution) / analogRead(settings.thermistor_pin) - 1);
}

inline double Thermistor::readThermistorResistanceAvg()
{
    double sum = 0;
    for (int i = 0; i < settings.num_samples; ++i)
    {
        sum += readThermistorResistance();
        delay(settings.read_delay);
    }
    return sum / settings.num_samples;
    
}

inline double Thermistor::calculateSteinhart(double resistance)
{
    float steinhart;
    steinhart = resistance / settings.nominal_resistance;  // (R/R0)
    steinhart = log(steinhart);                                  // ln(R/R0)
    steinhart /= settings.b_coefficient;                         // 1/B * ln(R/R0)
    steinhart += 1.0 / (settings.nominal_temperature_c + 273.15);// 1/B * ln(R/R0) + (1/T0)
    return 1.0 / steinhart; // invert
}

double Thermistor::readResistance()
{
    return readThermistorResistance();
}
double Thermistor::readResistanceAvg()
{
    return readThermistorResistanceAvg();
}

double Thermistor::readKelvin()
{
    return calculateSteinhart(readThermistorResistance());
}

double Thermistor::readKelvinAvg()
{
    return calculateSteinhart(readThermistorResistanceAvg());
}

double Thermistor::readCelsius()
{
    return calculateSteinhart(readThermistorResistance()) - 273.15;
}

double Thermistor::readCelsiusAvg()
{
    return calculateSteinhart(readThermistorResistanceAvg()) - 273.15;
}