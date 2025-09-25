# BLE Examples

This directory contains example code demonstrating different BLE use cases with the ESP32-S3.

## Available Examples

### 1. Basic GATT Server (main project)
The main project in `src/main.cpp` implements a basic GATT server with:
- Single service and characteristic
- Read/Write/Notify operations
- Connection management
- Periodic value updates

### 2. Multiple Characteristics Example

```cpp
// Add to your BLE service for multiple characteristics
#define TEMP_CHARACTERISTIC_UUID "11111111-1111-1111-1111-111111111111"
#define HUMID_CHARACTERISTIC_UUID "22222222-2222-2222-2222-222222222222"

// In your setup function:
NimBLECharacteristic* tempCharacteristic = pService->createCharacteristic(
    TEMP_CHARACTERISTIC_UUID,
    NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::NOTIFY
);

NimBLECharacteristic* humidCharacteristic = pService->createCharacteristic(
    HUMID_CHARACTERISTIC_UUID,
    NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::NOTIFY
);
```

### 3. Custom Data Format Example

```cpp
// Send structured data as JSON
void sendSensorData() {
    String jsonData = "{\"temperature\":25.4,\"humidity\":60.2,\"timestamp\":" + String(millis()) + "}";
    pCharacteristic->setValue(jsonData.c_str());
    pCharacteristic->notify();
}
```

### 4. Security Example

```cpp
// Enhanced security setup
NimBLEDevice::setSecurityAuth(BLE_SM_PAIR_AUTHREQ_BOND | BLE_SM_PAIR_AUTHREQ_MITM | BLE_SM_PAIR_AUTHREQ_SC);
NimBLEDevice::setSecurityIOCap(BLE_HS_IO_DISPLAY_YESNO);
NimBLEDevice::setSecurityInitKey(BLE_SM_PAIR_KEY_DIST_ENC | BLE_SM_PAIR_KEY_DIST_ID);
NimBLEDevice::setSecurityRespKey(BLE_SM_PAIR_KEY_DIST_ENC | BLE_SM_PAIR_KEY_DIST_ID);
```

## Testing Your Implementation

1. Use a BLE scanner app to verify your device is advertising
2. Connect and verify services/characteristics are visible
3. Test read/write operations
4. Verify notifications work correctly
5. Test connection/disconnection handling

## Common Issues and Solutions

1. **Device not visible**: Check advertising is started and not in connected state
2. **Write operations fail**: Verify characteristic permissions include WRITE
3. **Notifications not received**: Ensure client has subscribed to notifications
4. **Connection drops**: Check power management and connection parameters