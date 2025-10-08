# WiFi Manager

This document describes the WiFi functionality integrated into the ESP32-S3 BLE GATT Server project.

## Overview

The WiFi Manager provides basic WiFi connectivity for the ESP32-S3, allowing it to connect to a wireless network. It includes automatic connection handling, reconnection on disconnect, and status reporting.

## Features

- **Automatic Connection**: Connects to configured WiFi network on startup
- **Auto Reconnection**: Automatically reconnects if connection is lost
- **Connection Timeout**: Configurable timeout to prevent hanging
- **Retry Logic**: Attempts to reconnect with exponential backoff
- **Status Monitoring**: Reports connection status, IP address, and signal strength
- **Low Overhead**: Minimal impact on BLE and other services

## Configuration

### Secure Configuration (Recommended)

For security, WiFi credentials should **not** be hardcoded in source files. Instead, use build flags in `platformio.ini`:

```ini
[env:esp32-s3-devkitc-1-n16r8]
build_flags = 
    -D WIFI_SSID='"YourNetworkName"'
    -D WIFI_PASSWORD='"YourSecurePassword"'
```

**Note:** The double quotes inside single quotes are required for proper string handling.

**Security best practices:**
- Add `platformio.ini` to `.gitignore` if it contains credentials
- Or create a separate `wifi_credentials.ini` file (also gitignored) and include it:
  ```ini
  extra_configs = wifi_credentials.ini
  ```
- Never commit real WiFi credentials to version control

### Development/Testing Configuration

For quick testing only, you can set default values in `include/wifi_manager.h`:

```cpp
#define WIFI_SSID        "YourSSID"        // Development only
#define WIFI_PASSWORD    "YourPassword"    // Development only
#define WIFI_TIMEOUT_MS  20000             // Connection timeout (20 seconds)
```

The code will use build flags if defined, otherwise fall back to these defaults.

### Configuration Parameters

- **WIFI_SSID**: Your WiFi network name (SSID)
- **WIFI_PASSWORD**: Your WiFi network password
- **WIFI_TIMEOUT_MS**: Maximum time to wait for connection (default: 20 seconds)
- **MAX_CONNECTION_ATTEMPTS**: Number of retry attempts before waiting (default: 3)
- **RECONNECT_INTERVAL**: Time to wait before retrying after max attempts (default: 30 seconds)

## WiFi Manager API

### Initialization

```cpp
void WiFiManager::init();
```

Initializes the WiFi manager. Sets WiFi mode to station (STA) and prepares for connection.

### Connection

```cpp
void WiFiManager::connect();
```

Attempts to connect to the configured WiFi network. Includes timeout and retry logic.

### Disconnection

```cpp
void WiFiManager::disconnect();
```

Disconnects from the WiFi network.

### Loop Function

```cpp
void WiFiManager::loop();
```

Must be called regularly from the main loop. Monitors connection state and handles automatic reconnection.

### Status Functions

```cpp
bool WiFiManager::isConnected();
String WiFiManager::getIPAddress();
String WiFiManager::getMACAddress();
int WiFiManager::getRSSI();
String WiFiManager::getSSID();
```

- **isConnected()**: Returns true if connected to WiFi
- **getIPAddress()**: Returns the device's IP address as a string
- **getMACAddress()**: Returns the device's MAC address
- **getRSSI()**: Returns signal strength in dBm (higher is better)
- **getSSID()**: Returns the connected network's SSID

## Usage Example

### In main.cpp

```cpp
#include "wifi_manager.h"

void setup() {
    Serial.begin(115200);
    
    // Initialize WiFi Manager
    WiFiManager::init();
    
    // Connect to WiFi
    WiFiManager::connect();
}

void loop() {
    // Maintain WiFi connection
    WiFiManager::loop();
    
    // Check status periodically
    if (WiFiManager::isConnected()) {
        Serial.println("WiFi IP: " + WiFiManager::getIPAddress());
        Serial.println("WiFi RSSI: " + String(WiFiManager::getRSSI()) + " dBm");
    }
    
    delay(100);
}
```

## Serial Output

The WiFi Manager provides detailed logging to the serial monitor:

### Initialization

```
Initializing WiFi Manager...
WiFi Manager initialized
```

### Connection Success

```
Connecting to WiFi...
SSID: YourNetworkName
.....
WiFi connected successfully!
IP Address: 192.168.1.100
MAC Address: AA:BB:CC:DD:EE:FF
Signal Strength (RSSI): -45 dBm
```

### Connection Failure

```
Connecting to WiFi...
SSID: YourNetworkName
.......................
WiFi connection failed!
Connection attempt 1 of 3
```

### Connection Lost

```
WiFi connection lost!
Connecting to WiFi...
```

## Troubleshooting

### WiFi doesn't connect

1. **Check credentials**: Verify SSID and password in `wifi_manager.h`
2. **Check network**: Ensure the WiFi network is available and in range
3. **Check frequency**: ESP32-S3 only supports 2.4GHz WiFi (not 5GHz)
4. **Monitor serial output**: Look for connection error messages

### Connection keeps dropping

1. **Check signal strength**: RSSI should be > -70 dBm for stable connection
2. **Reduce interference**: Move away from other 2.4GHz devices
3. **Check router**: Verify router settings and firmware
4. **Power supply**: Ensure stable power supply to ESP32-S3

### Slow connection

1. **Increase timeout**: Adjust `WIFI_TIMEOUT_MS` if needed
2. **Router settings**: Check for DHCP delays or authentication issues
3. **Network traffic**: High network traffic can slow connection

## Integration with BLE

The WiFi Manager is designed to work alongside BLE without conflicts:

- **Coexistence**: ESP32-S3 supports simultaneous WiFi and BLE operation
- **Independent**: WiFi and BLE operate independently
- **Low overhead**: WiFi manager uses minimal CPU time in loop()
- **Non-blocking**: Connection attempts don't block BLE operations

## Performance Notes

- **Connection time**: Typically 3-10 seconds on first connection
- **Reconnection time**: Typically 2-5 seconds when reconnecting
- **CPU usage**: Minimal when connected, higher during connection attempts
- **Memory usage**: ~4KB for WiFi stack overhead

## Future Enhancements

Possible improvements for production use:

- Web server for configuration
- OTA (Over-The-Air) firmware updates
- Static IP configuration option
- Multiple WiFi network support with fallback
- WiFi AP mode for initial setup
- Network time synchronization (NTP)
- MQTT or HTTP client integration
- WiFi event callbacks for application use

## Security Considerations

1. **Credentials storage**: Multiple secure options available
   - **Recommended**: Use build flags in `platformio.ini` (keep file out of version control)
   - **Alternative**: Store in NVS (Non-Volatile Storage) for production deployments
   - **Advanced**: Use WiFi Protected Setup (WPS) or SmartConfig for initial setup
   - The implementation supports all methods via conditional compilation
   
2. **Network security**: Ensure your WiFi network uses WPA2 or WPA3
   
3. **Version control**: Never commit real credentials to repositories
   - Add `platformio.ini` to `.gitignore` if it contains credentials
   - Or use a separate `wifi_credentials.ini` file that is gitignored
   - The header file now uses `#ifndef` to allow build-time overrides

## Standards Compliance

- **WiFi Protocol**: 802.11 b/g/n (2.4 GHz)
- **Security**: WPA/WPA2/WPA3-PSK
- **IPv4**: Full support
- **IPv6**: Supported by ESP-IDF (not explicitly configured in this implementation)

## Example Use Cases

### 1. Remote Monitoring

Use WiFi to send sensor data to a cloud service or local server while BLE provides local configuration access.

### 2. Time Synchronization

Connect to WiFi to sync time with an NTP server, useful for timestamped sensor readings.

### 3. OTA Updates

Enable firmware updates over WiFi without physical access to the device.

### 4. Web Interface

Serve a web page for configuration and monitoring accessible from any browser on the network.

### 5. MQTT Integration

Publish sensor data to an MQTT broker for integration with home automation systems.
