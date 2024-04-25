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

double Thermistor::readCelsiusFiltered()
{
    return filter(calculateSteinhart(readThermistorResistance()) - 273.15);
}

double Thermistor::filter(double input)
{
    unsigned long timestamp = micros();
  float dt = (timestamp - timestamp_prev)*1e-6f;
  // quick fix for strange cases (micros overflow)
  if (dt < 0.0f || dt > 0.5f) dt = 1e-3f;

  // calculate the filtering 
  float alpha = Tf/(Tf + dt);
  float y = alpha*y_prev + (1.0f - alpha)*input;

  // save the variables
  y_prev = y;
  timestamp_prev = timestamp;
  return y;
}

ThermistorArray::ThermistorArray(){
    THARRAYSettings _set;
    _set.num_thermistors = 16;
    _set.sig = A0;
    _set.en = 7;
    _set.s0 = 8;
    _set.s1 = 9;
    _set.s2 = 10;
    _set.s3 = 11;
    updateArraySettings(_set);
    initThermistors();
}

ThermistorArray::ThermistorArray(uint8_t num, uint8_t sig, uint8_t en, uint8_t s0, uint8_t s1, uint8_t s2, uint8_t s3){
    THARRAYSettings _set;
    _set.num_thermistors = num;
    _set.sig = sig;
    _set.en = en;
    _set.s0 = s0;
    _set.s1 = s1;
    _set.s2 = s2;
    _set.s3 = s3;
    updateArraySettings(_set);
    initThermistors();
}

ThermistorArray::ThermistorArray(THARRAYSettings settings){
    updateArraySettings(settings);
    initThermistors();
}

void ThermistorArray::initThermistors()
{
    for(int i = 0; i < _settings.num_thermistors; i++)
    {
        _array[i] = Thermistor();
        _array[i].settings.thermistor_pin = _settings.sig;
    }
}

void ThermistorArray::switchChannels(uint8_t num)
{
    digitalWrite(_settings.en, LOW);
    digitalWrite(_settings.s0, num & 0x0001);
    num >>=1;
    digitalWrite(_settings.s1, num & 0x0001);
    num >>=1;
    digitalWrite(_settings.s2, num & 0x0001);
    num >>=1;
    digitalWrite(_settings.s3, num & 0x0001);
}

THARRAYSettings ThermistorArray::getArraySettings(){
    return _settings;
}

void ThermistorArray::updateArraySettings(THARRAYSettings settings)
{
    _settings = settings;

    pinMode(_settings.sig, INPUT);
    pinMode(_settings.en, OUTPUT);
    pinMode(_settings.s0, OUTPUT);
    pinMode(_settings.s1, OUTPUT);
    pinMode(_settings.s2, OUTPUT);
    pinMode(_settings.s3, OUTPUT);

    digitalWrite(_settings.en, LOW);
}

THSettings ThermistorArray::getThermistorSettings(uint8_t channel)
{
    channel = channel >= _settings.num_thermistors ? 0 : channel;
    return _array[channel].settings;

}

void ThermistorArray::updateThermistorSettings(uint8_t channel, THSettings settings)
{
    channel = channel >= _settings.num_thermistors ? 0 : channel;
    _array[channel].settings = settings;

}

double ThermistorArray::readKelvin(uint8_t channel)
{
    channel = channel >= _settings.num_thermistors ? 0 : channel;
    switchChannels(channel);
    return _array[channel].readKelvin();
}

double ThermistorArray::readKelvinAvg(uint8_t channel)
{
    channel = channel >= _settings.num_thermistors ? 0 : channel;
    switchChannels(channel);
    return _array[channel].readKelvinAvg();
}

double ThermistorArray::readCelsius(uint8_t channel)
{
    channel = channel >= _settings.num_thermistors ? 0 : channel;
    switchChannels(channel);
    return _array[channel].readCelsius();
}

double ThermistorArray::readCelsiusAvg(uint8_t channel)
{
    channel = channel >= _settings.num_thermistors ? 0 : channel;
    switchChannels(channel);
    return _array[channel].readCelsiusAvg();
}

double ThermistorArray::readResistance(uint8_t channel)
{
    channel = channel >= _settings.num_thermistors ? 0 : channel;
    switchChannels(channel);
    return _array[channel].readResistance();
}

double ThermistorArray::readResistanceAvg(uint8_t channel)
{
    channel = channel >= _settings.num_thermistors ? 0 : channel;
    switchChannels(channel);
    return _array[channel].readResistanceAvg();
}

double ThermistorArray::readCelsiusFiltered(uint8_t channel)
{
    channel = channel >= _settings.num_thermistors ? 0 : channel;
    switchChannels(channel);
    return _array[channel].readCelsiusFiltered();
}
