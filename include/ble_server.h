#ifndef BLE_SERVER_H
#define BLE_SERVER_H

#ifdef ARDUINO
#include <Arduino.h>
#include <NimBLEDevice.h>
#else
#include <cstdint>
#include <string>

using String = std::string;

inline void delay(uint32_t) {}
inline unsigned long millis() { return 0; }

class NimBLEServer;
class NimBLEService;
class NimBLECharacteristic;
#endif

// BLE Configuration
#define DEVICE_NAME         "ESP32-S3-BLE-Device"
#define SERVICE_UUID        "12345678-1234-1234-1234-123456789abc"
#define CHARACTERISTIC_UUID "87654321-4321-4321-4321-cba987654321"

// BLE Server class declaration
class BLEServerManager {
private:
    static NimBLEServer* pServer;
    static NimBLEService* pService;
    static NimBLECharacteristic* pCharacteristic;
    static bool deviceConnected;
    static bool oldDeviceConnected;
    static uint32_t value;

public:
    static void init();
    static void loop();
    static bool isConnected();
    static void updateValue(const String& newValue);
    static void notify();
    static void setDeviceConnectionState(bool connected);
};

#ifdef ARDUINO
// Callback classes
class MyServerCallbacks: public NimBLEServerCallbacks {
public:
    void onConnect(NimBLEServer* pServer);
    void onDisconnect(NimBLEServer* pServer);
};

class MyCharacteristicCallbacks: public NimBLECharacteristicCallbacks {
public:
    void onRead(NimBLECharacteristic* pCharacteristic);
    void onWrite(NimBLECharacteristic* pCharacteristic);
};
#endif

#endif // BLE_SERVER_H