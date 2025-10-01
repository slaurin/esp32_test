#ifndef TEMPERATURE_SERVICE_H
#define TEMPERATURE_SERVICE_H

#include <Arduino.h>

// Temperature unit configuration
enum TemperatureUnit {
    CELSIUS = 0,
    FAHRENHEIT = 1
};

// Temperature service class
class TemperatureService {
private:
    static float currentTemp;
    static float maxTemp;
    static float minTemp;
    static TemperatureUnit unit;
    static unsigned long lastUpdateTime;
    static const unsigned long UPDATE_INTERVAL = 30000; // 30 seconds in milliseconds
    
    static float generateFakeTemperature();
    static float celsiusToFahrenheit(float celsius);
    static float fahrenheitToCelsius(float fahrenheit);

public:
    static void init();
    static void update();
    static float getCurrentTemperature();
    static float getMaxTemperature();
    static float getMinTemperature();
    static TemperatureUnit getUnit();
    static void setUnit(TemperatureUnit newUnit);
    static bool shouldUpdate();
};

#endif // TEMPERATURE_SERVICE_H
