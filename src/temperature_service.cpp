#include "temperature_service.h"

// Static member definitions
float TemperatureService::currentTemp = -20.0f;
float TemperatureService::maxTemp = -100.0f;
float TemperatureService::minTemp = 100.0f;
TemperatureUnit TemperatureService::unit = CELSIUS;
unsigned long TemperatureService::lastUpdateTime = 0;

void TemperatureService::init() {
    Serial.println("Initializing Temperature Service...");
    currentTemp = generateFakeTemperature();
    maxTemp = currentTemp;
    minTemp = currentTemp;
    lastUpdateTime = millis();
    Serial.println("Temperature Service initialized");
}

void TemperatureService::update() {
    if (shouldUpdate()) {
        currentTemp = generateFakeTemperature();
        
        // Update max and min temperatures
        if (currentTemp > maxTemp) {
            maxTemp = currentTemp;
        }
        if (currentTemp < minTemp) {
            minTemp = currentTemp;
        }
        
        lastUpdateTime = millis();
        
        Serial.print("Temperature updated: Current=");
        Serial.print(currentTemp);
        Serial.print(unit == CELSIUS ? "°C" : "°F");
        Serial.print(", Max=");
        Serial.print(maxTemp);
        Serial.print(unit == CELSIUS ? "°C" : "°F");
        Serial.print(", Min=");
        Serial.print(minTemp);
        Serial.println(unit == CELSIUS ? "°C" : "°F");
    }
}

float TemperatureService::getCurrentTemperature() {
    return currentTemp;
}

float TemperatureService::getMaxTemperature() {
    return maxTemp;
}

float TemperatureService::getMinTemperature() {
    return minTemp;
}

TemperatureUnit TemperatureService::getUnit() {
    return unit;
}

void TemperatureService::setUnit(TemperatureUnit newUnit) {
    if (unit != newUnit) {
        if (newUnit == FAHRENHEIT && unit == CELSIUS) {
            // Convert all temperatures to Fahrenheit
            currentTemp = celsiusToFahrenheit(currentTemp);
            maxTemp = celsiusToFahrenheit(maxTemp);
            minTemp = celsiusToFahrenheit(minTemp);
        } else if (newUnit == CELSIUS && unit == FAHRENHEIT) {
            // Convert all temperatures to Celsius
            currentTemp = fahrenheitToCelsius(currentTemp);
            maxTemp = fahrenheitToCelsius(maxTemp);
            minTemp = fahrenheitToCelsius(minTemp);
        }
        unit = newUnit;
        Serial.print("Temperature unit changed to: ");
        Serial.println(unit == CELSIUS ? "Celsius" : "Fahrenheit");
    }
}

bool TemperatureService::shouldUpdate() {
    return (millis() - lastUpdateTime) >= UPDATE_INTERVAL;
}

float TemperatureService::generateFakeTemperature() {
    // Generate a fake temperature between 15 and 30 degrees Celsius
    // Using a simple pseudo-random variation based on time
    float baseTemp = -10.5f; // Base temperature
    float variation = (float)(millis() % 1000) / 1000.0f * 10.0f - 5.0f; // -5 to +5 variation
    float temp = baseTemp + variation;
    
    // If we're in Fahrenheit mode, convert the generated Celsius value
    if (unit == FAHRENHEIT) {
        temp = celsiusToFahrenheit(temp);
    }
    
    return temp;
}

float TemperatureService::celsiusToFahrenheit(float celsius) {
    return (celsius * 9.0f / 5.0f) + 32.0f;
}

float TemperatureService::fahrenheitToCelsius(float fahrenheit) {
    return (fahrenheit - 32.0f) * 5.0f / 9.0f;
}
