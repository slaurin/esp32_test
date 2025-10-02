#include "ble_server.h"

#ifdef ARDUINO
#include "temperature_service.h"

// Static member definitions
NimBLEServer* BLEServerManager::pServer = nullptr;
NimBLEService* BLEServerManager::pService = nullptr;
NimBLECharacteristic* BLEServerManager::pCharacteristic = nullptr;
NimBLEService* BLEServerManager::pTempService = nullptr;
NimBLECharacteristic* BLEServerManager::pTempCharacteristic = nullptr;
NimBLECharacteristic* BLEServerManager::pTempMaxCharacteristic = nullptr;
NimBLECharacteristic* BLEServerManager::pTempMinCharacteristic = nullptr;
NimBLECharacteristic* BLEServerManager::pTempConfigCharacteristic = nullptr;
bool BLEServerManager::deviceConnected = false;
bool BLEServerManager::oldDeviceConnected = false;
uint32_t BLEServerManager::value = 0;

// Server callback implementations
void MyServerCallbacks::onConnect(NimBLEServer* pServer) {
    BLEServerManager::setDeviceConnectionState(true);
    Serial.println("Client connected");

    // Start advertising again to allow multiple connections
    NimBLEDevice::startAdvertising();
}

void MyServerCallbacks::onDisconnect(NimBLEServer* pServer) {
    BLEServerManager::setDeviceConnectionState(false);
    Serial.println("Client disconnected - start advertising");
}

// Characteristic callback implementations
void MyCharacteristicCallbacks::onRead(NimBLECharacteristic* pCharacteristic) {
    Serial.print("Read request received. Current value: ");
    Serial.println(pCharacteristic->getValue().c_str());
}

void MyCharacteristicCallbacks::onWrite(NimBLECharacteristic* pCharacteristic) {
    std::string value = pCharacteristic->getValue();
    Serial.print("Write request received. New value: ");
    Serial.println(value.c_str());

    if (value.length() > 0) {
        Serial.print("Data written: ");
        for (int i = 0; i < value.length(); i++) {
            Serial.print(value[i]);
        }
        Serial.println();
    }
}

// Temperature config callback implementation
void TempConfigCallbacks::onWrite(NimBLECharacteristic* pCharacteristic) {
    std::string value = pCharacteristic->getValue();
    
    if (value.length() > 0) {
        uint8_t unitValue = (uint8_t)value[0];
        Serial.print("Temperature unit config write received: ");
        Serial.println(unitValue);
        
        if (unitValue == 0 || unitValue == 1) {
            TemperatureService::setUnit((TemperatureUnit)unitValue);
            Serial.println("Temperature unit updated successfully");
        } else {
            Serial.println("Invalid temperature unit value. Use 0 for Celsius, 1 for Fahrenheit");
        }
    }
}

// BLE Server Manager implementations
void BLEServerManager::init() {
    Serial.println("Initializing BLE Server...");

    // Initialize NimBLE
    NimBLEDevice::init(DEVICE_NAME);

    // Set security
    NimBLEDevice::setSecurityAuth(true, true, true);
    NimBLEDevice::setSecurityPasskey(123456);
    NimBLEDevice::setSecurityIOCap(BLE_HS_IO_DISPLAY_ONLY);

    // Create BLE Server
    pServer = NimBLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());

    // Create BLE Service
    pService = pServer->createService(SERVICE_UUID);

    // Create BLE Characteristic
    pCharacteristic = pService->createCharacteristic(
                         CHARACTERISTIC_UUID,
                         NIMBLE_PROPERTY::READ   |
                         NIMBLE_PROPERTY::WRITE  |
                         NIMBLE_PROPERTY::NOTIFY |
                         NIMBLE_PROPERTY::INDICATE
                       );

    pCharacteristic->setCallbacks(new MyCharacteristicCallbacks());
    pCharacteristic->setValue("Hello ESP32-S3");

    // Start the service
    pService->start();

    // Create Environmental Sensing Service for temperature
    pTempService = pServer->createService(ENV_SENSING_SERVICE_UUID);
    
    // Create Temperature Characteristics (all are READ and NOTIFY)
    pTempCharacteristic = pTempService->createCharacteristic(
                             TEMPERATURE_CHAR_UUID,
                             NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::NOTIFY
                           );
    
    pTempMaxCharacteristic = pTempService->createCharacteristic(
                                TEMP_MAX_CHAR_UUID,
                                NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::NOTIFY
                              );
    
    pTempMinCharacteristic = pTempService->createCharacteristic(
                                TEMP_MIN_CHAR_UUID,
                                NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::NOTIFY
                              );
    
    // Temperature config characteristic (READ and WRITE for unit configuration)
    pTempConfigCharacteristic = pTempService->createCharacteristic(
                                   TEMP_CONFIG_CHAR_UUID,
                                   NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::WRITE
                                 );
    
    pTempConfigCharacteristic->setCallbacks(new TempConfigCallbacks());
    
    // Set initial values
    uint8_t initialUnit = (uint8_t)TemperatureService::getUnit();
    pTempConfigCharacteristic->setValue(&initialUnit, 1);
    
    // Start the temperature service
    pTempService->start();

    // Start advertising
    NimBLEAdvertising *pAdvertising = NimBLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->addServiceUUID(ENV_SENSING_SERVICE_UUID);
    pAdvertising->setScanResponse(false);
    pAdvertising->setMinPreferred(0x0);  // set value to 0x00 to not advertise this parameter
    NimBLEDevice::startAdvertising();

    Serial.println("BLE GATT Server started!");
    Serial.println("Device name: " + String(DEVICE_NAME));
    Serial.println("Service UUID: " + String(SERVICE_UUID));
    Serial.println("Characteristic UUID: " + String(CHARACTERISTIC_UUID));
    Serial.println("Temperature Service UUID: " + String(ENV_SENSING_SERVICE_UUID));
    Serial.println("Waiting for a client connection to notify...");
}

void BLEServerManager::loop() {
    // Check if device is connected
    if (deviceConnected) {
        // Update characteristic value periodically
        value++;
        String newValue = "Count: " + String(value);
        updateValue(newValue);
        notify();

        Serial.println("Sent notification: " + newValue);
        delay(3000); // Delay between notifications
    }

    // Handle disconnecting
    if (!deviceConnected && oldDeviceConnected) {
        delay(500); // Give the bluetooth stack the chance to get things ready
        pServer->startAdvertising(); // Restart advertising
        Serial.println("Restarted advertising");
        oldDeviceConnected = deviceConnected;
    }

    // Handle connecting
    if (deviceConnected && !oldDeviceConnected) {
        // Do stuff here on connecting
        oldDeviceConnected = deviceConnected;
    }
}

bool BLEServerManager::isConnected() {
    return deviceConnected;
}

void BLEServerManager::updateValue(const String& newValue) {
    if (pCharacteristic) {
        pCharacteristic->setValue(newValue.c_str());
    }
}

void BLEServerManager::notify() {
    if (pCharacteristic && deviceConnected) {
        pCharacteristic->notify();
    }
}

void BLEServerManager::setDeviceConnectionState(bool connected) {
    deviceConnected = connected;
}

#else

// Static member definitions for native/unit-test builds
NimBLEServer* BLEServerManager::pServer = nullptr;
NimBLEService* BLEServerManager::pService = nullptr;
NimBLECharacteristic* BLEServerManager::pCharacteristic = nullptr;
bool BLEServerManager::deviceConnected = false;
bool BLEServerManager::oldDeviceConnected = false;
uint32_t BLEServerManager::value = 0;

void BLEServerManager::init() {}

void BLEServerManager::loop() {}

bool BLEServerManager::isConnected() {
    return deviceConnected;
}

void BLEServerManager::updateValue(const String& /*newValue*/) {}

void BLEServerManager::notify() {}

void BLEServerManager::setDeviceConnectionState(bool connected) {
    deviceConnected = connected;
}

#endif

void BLEServerManager::updateTemperature(float current, float max, float min) {
    if (pTempCharacteristic) {
        // Convert float to int16_t for BLE (multiply by 100 to keep 2 decimal places)
        int16_t currentInt = (int16_t)(current * 100);
        int16_t maxInt = (int16_t)(max * 100);
        int16_t minInt = (int16_t)(min * 100);
        
        pTempCharacteristic->setValue((uint8_t*)&currentInt, 2);
        pTempMaxCharacteristic->setValue((uint8_t*)&maxInt, 2);
        pTempMinCharacteristic->setValue((uint8_t*)&minInt, 2);
    }
}

void BLEServerManager::notifyTemperature() {
    if (deviceConnected) {
        if (pTempCharacteristic) {
            pTempCharacteristic->notify();
        }
        if (pTempMaxCharacteristic) {
            pTempMaxCharacteristic->notify();
        }
        if (pTempMinCharacteristic) {
            pTempMinCharacteristic->notify();
        }
    }
}
