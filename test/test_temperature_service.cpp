#include <unity.h>
#include <Arduino.h>
#include "../include/temperature_service.h"
#include "../include/ble_server.h"

// Test temperature service initialization
void test_temperature_init() {
    TemperatureService::init();
    float temp = TemperatureService::getCurrentTemperature();
    // Temperature should be within reasonable range (15-30°C or equivalent in F)
    TEST_ASSERT_TRUE(temp >= 10.0f && temp <= 120.0f);
}

// Test temperature unit conversion
void test_temperature_unit_conversion() {
    TemperatureService::init();
    TemperatureService::setUnit(CELSIUS);
    float tempC = TemperatureService::getCurrentTemperature();
    
    TemperatureService::setUnit(FAHRENHEIT);
    float tempF = TemperatureService::getCurrentTemperature();
    
    // After conversion, Fahrenheit should be higher than Celsius
    TEST_ASSERT_TRUE(tempF > tempC);
}

// Test temperature tracking (max/min)
void test_temperature_tracking() {
    TemperatureService::init();
    float current = TemperatureService::getCurrentTemperature();
    float max = TemperatureService::getMaxTemperature();
    float min = TemperatureService::getMinTemperature();
    
    // Initially, all should be the same
    TEST_ASSERT_EQUAL_FLOAT(current, max);
    TEST_ASSERT_EQUAL_FLOAT(current, min);
}

// Test temperature UUIDs are valid
void test_temperature_uuids() {
    String envServiceUUID = ENV_SENSING_SERVICE_UUID;
    String tempCharUUID = TEMPERATURE_CHAR_UUID;
    String tempMaxCharUUID = TEMP_MAX_CHAR_UUID;
    String tempMinCharUUID = TEMP_MIN_CHAR_UUID;
    String tempConfigCharUUID = TEMP_CONFIG_CHAR_UUID;
    
    // All UUIDs should be 36 characters (standard UUID format)
    TEST_ASSERT_EQUAL(36, envServiceUUID.length());
    TEST_ASSERT_EQUAL(36, tempCharUUID.length());
    TEST_ASSERT_EQUAL(36, tempMaxCharUUID.length());
    TEST_ASSERT_EQUAL(36, tempMinCharUUID.length());
    TEST_ASSERT_EQUAL(36, tempConfigCharUUID.length());
}

// Test temperature unit getter/setter
void test_temperature_unit() {
    TemperatureService::init();
    TemperatureService::setUnit(CELSIUS);
    TEST_ASSERT_EQUAL(CELSIUS, TemperatureService::getUnit());
    
    TemperatureService::setUnit(FAHRENHEIT);
    TEST_ASSERT_EQUAL(FAHRENHEIT, TemperatureService::getUnit());
}

void setUp(void) {
    // Set up test environment
}

void tearDown(void) {
    // Clean up after tests
}

void setup() {
    delay(2000); // Wait for serial
    
    UNITY_BEGIN();
    RUN_TEST(test_temperature_init);
    RUN_TEST(test_temperature_unit_conversion);
    RUN_TEST(test_temperature_tracking);
    RUN_TEST(test_temperature_uuids);
    RUN_TEST(test_temperature_unit);
    UNITY_END();
}

void loop() {
    // Nothing to do in loop for tests
}
