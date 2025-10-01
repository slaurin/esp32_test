# Temperature Service

This document describes the temperature monitoring feature added to the ESP32-S3 BLE server.

## Overview

The temperature service implements a fake temperature sensor that reports temperature readings over BLE using the standard Environmental Sensing Service (UUID: 0x181A).

## Features

- **Fake Temperature Generation**: Generates pseudo-random temperature values between 17.5°C and 27.5°C (63.5°F to 81.5°F)
- **30-Second Updates**: Temperature readings are updated every 30 seconds
- **Min/Max Tracking**: Automatically tracks minimum and maximum temperatures since startup
- **Unit Configuration**: Supports both Celsius and Fahrenheit units
- **BLE Notifications**: Sends temperature updates to connected BLE clients

## BLE Characteristics

### Environmental Sensing Service
**UUID**: `0000181A-0000-1000-8000-00805f9b34fb`

#### Current Temperature Characteristic
- **UUID**: `00002A6E-0000-1000-8000-00805f9b34fb`
- **Properties**: Read, Notify
- **Format**: Signed 16-bit integer (value * 100)
- **Example**: Value `2250` = 22.50°C or 72.50°F

#### Max Temperature Characteristic
- **UUID**: `00002A6F-0000-1000-8000-00805f9b34fb`
- **Properties**: Read, Notify
- **Format**: Signed 16-bit integer (value * 100)
- **Example**: Value `2750` = 27.50°C or 81.50°F

#### Min Temperature Characteristic
- **UUID**: `00002A70-0000-1000-8000-00805f9b34fb`
- **Properties**: Read, Notify
- **Format**: Signed 16-bit integer (value * 100)
- **Example**: Value `1750` = 17.50°C or 63.50°F

#### Temperature Config Characteristic
- **UUID**: `00002A71-0000-1000-8000-00805f9b34fb`
- **Properties**: Read, Write
- **Format**: Single byte
- **Values**:
  - `0` = Celsius
  - `1` = Fahrenheit

## Using the Temperature Service

### Reading Temperature

1. Connect to the ESP32-S3 BLE device
2. Discover services and find the Environmental Sensing Service (0x181A)
3. Read the Current Temperature characteristic
4. Parse the value as a signed 16-bit integer
5. Divide by 100 to get the actual temperature

### Enabling Notifications

1. Subscribe to notifications on the temperature characteristics
2. Receive automatic updates every 30 seconds
3. Each notification contains the latest temperature reading

### Changing Temperature Units

1. Write to the Temperature Config characteristic:
   - Write `0x00` for Celsius
   - Write `0x01` for Fahrenheit
2. All temperature values will be converted to the selected unit
3. Historical min/max values are also converted

## Example BLE Client Code (Python with bleak)

```python
import asyncio
from bleak import BleakClient, BleakScanner

DEVICE_NAME = "ESP32-S3-BLE-Device"
TEMP_CURRENT_UUID = "00002A6E-0000-1000-8000-00805f9b34fb"
TEMP_CONFIG_UUID = "00002A71-0000-1000-8000-00805f9b34fb"

def notification_handler(sender, data):
    # Parse temperature (signed 16-bit integer, little-endian)
    temp_raw = int.from_bytes(data, byteorder='little', signed=True)
    temp = temp_raw / 100.0
    print(f"Temperature: {temp}°C")

async def main():
    # Find device
    device = await BleakScanner.find_device_by_name(DEVICE_NAME)
    
    async with BleakClient(device) as client:
        # Read current temperature
        temp_data = await client.read_gatt_char(TEMP_CURRENT_UUID)
        temp_raw = int.from_bytes(temp_data, byteorder='little', signed=True)
        print(f"Current temperature: {temp_raw / 100.0}°C")
        
        # Set to Fahrenheit
        await client.write_gatt_char(TEMP_CONFIG_UUID, bytes([1]))
        
        # Subscribe to notifications
        await client.start_notify(TEMP_CURRENT_UUID, notification_handler)
        
        # Wait for updates
        await asyncio.sleep(60)
        
        await client.stop_notify(TEMP_CURRENT_UUID)

asyncio.run(main())
```

## Example BLE Client (nRF Connect Mobile App)

1. Open nRF Connect app on your smartphone
2. Scan for devices and connect to "ESP32-S3-BLE-Device"
3. Expand the "Environmental Sensing" service (0x181A)
4. Tap the characteristic you want to read:
   - Current Temperature (0x2A6E)
   - Max Temperature (0x2A6F)
   - Min Temperature (0x2A70)
5. To enable notifications:
   - Tap the notification icon (three arrows pointing down)
   - You'll receive updates every 30 seconds
6. To change units:
   - Select Temperature Config (0x2A71)
   - Write `00` for Celsius or `01` for Fahrenheit

## Testing

Unit tests are provided in `test/test_temperature_service.cpp`:

```bash
# Run tests with PlatformIO
pio test -e native
```

Tests cover:
- Temperature service initialization
- Unit conversion (Celsius ↔ Fahrenheit)
- Min/Max temperature tracking
- UUID format validation
- Unit getter/setter functionality

## Implementation Details

### Temperature Generation

The fake temperature is generated using a simple algorithm:
```cpp
float baseTemp = 22.5f;  // Base temperature (22.5°C)
float variation = (millis() % 1000) / 1000.0f * 10.0f - 5.0f;  // -5 to +5
float temp = baseTemp + variation;  // Result: 17.5 to 27.5°C
```

### Data Format

Temperature values are transmitted as signed 16-bit integers in little-endian format. To convert:
- **To send**: `int16_t value = (int16_t)(temperature * 100)`
- **To receive**: `float temperature = (float)value / 100.0f`

This format allows:
- Range: -327.68°C to 327.67°C
- Precision: 0.01°C (two decimal places)

## Future Enhancements

Possible improvements for production use:
- Replace fake temperature with real sensor (DHT22, BME280, etc.)
- Add humidity and pressure sensing
- Implement data logging
- Add configurable update intervals
- Store min/max history in non-volatile memory
- Add temperature alarms/thresholds
