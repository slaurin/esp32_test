#include <Arduino.h>
#include <NimBLEDevice.h>

static const char* TAG = "ESP32_BLE_GATT";

// UUIDs for our GATT service and characteristics
#define SERVICE_UUID        "12345678-1234-1234-1234-123456789abc"
#define CHARACTERISTIC_UUID "87654321-4321-4321-4321-cba987654321"
#define DEVICE_NAME         "ESP32-S3-BLE-Device"

static NimBLEServer* pServer = nullptr;
static NimBLEService* pService = nullptr;
static NimBLECharacteristic* pCharacteristic = nullptr;
static bool deviceConnected = false;
static bool oldDeviceConnected = false;
static uint32_t value = 0;

class MyServerCallbacks: public NimBLEServerCallbacks {
    void onConnect(NimBLEServer* pServer) {
        deviceConnected = true;
        Serial.println("Client connected");
        
        // Start advertising again to allow multiple connections
        NimBLEDevice::startAdvertising();
    }

    void onDisconnect(NimBLEServer* pServer) {
        deviceConnected = false;
        Serial.println("Client disconnected - start advertising");
    }
};

class MyCallbacks: public NimBLECharacteristicCallbacks {
    void onRead(NimBLECharacteristic* pCharacteristic) {
        Serial.print("Read request received. Current value: ");
        Serial.println(pCharacteristic->getValue().c_str());
    }

    void onWrite(NimBLECharacteristic* pCharacteristic) {
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
};

void setup() {
    Serial.begin(115200);
    Serial.println("Starting ESP32-S3 BLE GATT Server...");

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

    pCharacteristic->setCallbacks(new MyCallbacks());
    pCharacteristic->setValue("Hello ESP32-S3");

    // Start the service
    pService->start();

    // Start advertising
    NimBLEAdvertising *pAdvertising = NimBLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->setScanResponse(false);
    pAdvertising->setMinPreferred(0x0);  // set value to 0x00 to not advertise this parameter
    NimBLEDevice::startAdvertising();

    Serial.println("BLE GATT Server started!");
    Serial.println("Device name: " + String(DEVICE_NAME));
    Serial.println("Service UUID: " + String(SERVICE_UUID));
    Serial.println("Characteristic UUID: " + String(CHARACTERISTIC_UUID));
    Serial.println("Waiting for a client connection to notify...");
}

void loop() {
    // Check if device is connected
    if (deviceConnected) {
        // Update characteristic value periodically
        value++;
        String newValue = "Count: " + String(value);
        pCharacteristic->setValue(newValue.c_str());
        pCharacteristic->notify();
        
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
    
    delay(100);
}