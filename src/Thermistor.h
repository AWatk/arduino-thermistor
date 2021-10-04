#ifndef __THERMISTOR_SENSOR_H__
#define __THERMISTOR_SENSOR_H__

struct THSettings
{
    public:

    //thermistor properties
    double nominal_temperature_c; //celsius
    double nominal_resistance;
    double b_coefficient;
    //sensor properties
    int thermistor_pin;
    double series_resistor;
    int num_samples;
    int read_delay;
    int adc_resolution;

};

class Thermistor
{
    public:
    //settings
    THSettings settings;

    //Constructor generates default thermistor settings
    Thermistor(void);
    double readKelvin();
    double readKelvinAvg();
    double readCelsius();
    double readCelsiusAvg();
    double readResistance();
    double readResistanceAvg();

    private:
    inline double readThermistorResistance();
    inline double readThermistorResistanceAvg();
    inline double calculateSteinhart(double resistance);    

};


#endif // End of THERMISTOR_SENSOR_H__ check