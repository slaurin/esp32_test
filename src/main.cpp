//#include <Arduino.h>
#include "ble_server.h"
#include "temperature_service.h"

static const char* TAG = "ESP32_BLE_MAIN";

void setup() {
    Serial.begin(115200);
    Serial.println("\n=== ESP32-S3 BLE GATT Server Starting ===");
    
    // Add a small delay for serial to stabilize
    delay(1000);
    
    // Initialize Temperature Service
    TemperatureService::init();
    
    // Initialize BLE Server
    BLEServerManager::init();
    
    Serial.println("Setup complete. Entering main loop...");
}

void loop() {
    // Update temperature readings (checks internally if 30 seconds have passed)
    TemperatureService::update();
    
    // Update BLE temperature characteristics with current values
    BLEServerManager::updateTemperature(
        TemperatureService::getCurrentTemperature(),
        TemperatureService::getMaxTemperature(),
        TemperatureService::getMinTemperature()
    );
    
    // Notify connected clients about temperature updates every 30 seconds
    static unsigned long lastTempNotify = 0;
    if (BLEServerManager::isConnected() && (millis() - lastTempNotify >= 30000)) {
        BLEServerManager::notifyTemperature();
        lastTempNotify = millis();
    }
    
    // Run BLE server loop
    BLEServerManager::loop();
    
    // Add any additional application logic here
    delay(100);
    
    // Example: Print status every 30 seconds
    static unsigned long lastStatusPrint = 0;
    if (millis() - lastStatusPrint > 30000) {
        Serial.println("Status: BLE Server running, Connected: " + 
                      String(BLEServerManager::isConnected() ? "Yes" : "No"));
        lastStatusPrint = millis();
    }
}