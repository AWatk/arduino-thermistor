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

struct THARRAYSettings
{
    uint8_t num_thermistors;
    uint8_t s0;
    uint8_t s1;
    uint8_t s2;
    uint8_t s3;
    uint8_t sig;
    uint8_t en;
};

class ThermistorArray
{
    public:
    //default Constructor generates default thermistor settings
    ThermistorArray(void);
    //overloaded constructor initializes specific number of thermistors
    ThermistorArray(uint8_t num, uint8_t sig, uint8_t en, uint8_t s0, uint8_t s1, uint8_t s2, uint8_t s3);
    ThermistorArray(THARRAYSettings settings);

    THARRAYSettings getArraySettings();
    void updateArraySettings(THARRAYSettings _settings);
    THSettings getThermistorSettings(uint8_t channel);
    void updateThermistorSettings(uint8_t channel,THSettings _settings);

    double readKelvin(uint8_t channel);
    double readKelvinAvg(uint8_t channel);
    double readCelsius(uint8_t channel);
    double readCelsiusAvg(uint8_t channel);
    double readResistance(uint8_t channel);
    double readResistanceAvg(uint8_t channel);
    
    private:
    Thermistor _array[16];

    //settings
    THARRAYSettings _settings;

    void initThermistors();
    void switchChannels(uint8_t num);
    
};


#endif // End of THERMISTOR_SENSOR_H__ check