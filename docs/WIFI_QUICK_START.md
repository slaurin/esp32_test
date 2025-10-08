# WiFi Quick Start Guide

This is a quick reference for getting WiFi up and running on your ESP32-S3.

## 1. Configure WiFi Credentials (Secure Method)

### Option A: Using platformio.ini (Recommended for Security)

Edit `platformio.ini` and add your credentials to the `build_flags`:

```ini
[env:esp32-s3-devkitc-1-n16r8]
platform = espressif32
board = esp32-s3-devkitc-1
framework = arduino
build_flags = 
    -D CONFIG_BT_ENABLED=1
    -D CONFIG_BT_NIMBLE_ENABLED=1
    -D CONFIG_BT_BLUEDROID_ENABLED=0
    -D CONFIG_BT_CONTROLLER_ENABLED=1
    -D CORE_DEBUG_LEVEL=3
    -D WIFI_SSID='"MyHomeNetwork"'
    -D WIFI_PASSWORD='"MySecurePassword123"'
```

**Important:** The double quotes inside single quotes are required!

**Security tip:** Add `platformio.ini` to `.gitignore` or use a separate credentials file that is gitignored.

### Option B: Direct in Code (Development Only)

Edit `include/wifi_manager.h` and change these lines:

```cpp
#define WIFI_SSID        "MyHomeNetwork"
#define WIFI_PASSWORD    "MySecurePassword123"
```

**Warning:** Never commit real credentials to version control!

## 2. Build and Upload

```bash
pio run --target upload
```

## 3. Monitor Serial Output

```bash
pio device monitor
```

You should see:
```
Initializing WiFi Manager...
WiFi Manager initialized
Connecting to WiFi...
SSID: MyHomeNetwork
.....
WiFi connected successfully!
IP Address: 192.168.1.100
```

## 4. Check Status

Every 30 seconds, you'll see status updates:
```
Status: BLE Server running, Connected: Yes
WiFi Status: Connected
WiFi IP: 192.168.1.100
WiFi RSSI: -45 dBm
```

## Common Issues

### "WiFi connection failed!"

**Causes:**
- Wrong SSID or password
- WiFi network not in range
- Using 5GHz network (ESP32-S3 only supports 2.4GHz)

**Solutions:**
1. Double-check credentials in `wifi_manager.h`
2. Make sure your router's 2.4GHz band is enabled
3. Move closer to the router
4. Check router settings (ensure it's not hidden or MAC-filtered)

### Connection drops frequently

**Solutions:**
- Check signal strength (RSSI should be > -70 dBm)
- Reduce interference from other 2.4GHz devices
- Update router firmware
- Use a better power supply for ESP32-S3

### Very slow to connect

**Solutions:**
- Increase timeout in `wifi_manager.h`:
  ```cpp
  #define WIFI_TIMEOUT_MS  30000  // 30 seconds
  ```
- Check for DHCP issues on your router
- Ensure router isn't overloaded with devices

## API Reference

### In Your Code

```cpp
#include "wifi_manager.h"

// Initialize WiFi
WiFiManager::init();

// Connect to network
WiFiManager::connect();

// In loop - maintain connection
WiFiManager::loop();

// Check if connected
if (WiFiManager::isConnected()) {
    // Get IP address
    String ip = WiFiManager::getIPAddress();
    
    // Get signal strength
    int rssi = WiFiManager::getRSSI();
    
    // Get MAC address
    String mac = WiFiManager::getMACAddress();
}
```

## Default Settings

- **Connection timeout**: 20 seconds
- **Max retry attempts**: 3
- **Retry interval**: 30 seconds
- **WiFi mode**: Station (STA)
- **Security**: WPA/WPA2/WPA3-PSK

## What WiFi Enables

With WiFi connected, you can:
- Add OTA (Over-The-Air) updates
- Create a web server for configuration
- Send data to cloud services
- Sync time with NTP servers
- Implement MQTT for IoT integration
- Access REST APIs

## Security Best Practices

1. **Never commit credentials** to version control
   ```bash
   # Add to .gitignore
   echo "include/wifi_manager.h" >> .gitignore
   ```

2. **Use strong WiFi passwords** (WPA2/WPA3)

3. **For production**: Store credentials in NVS (non-volatile storage) instead of source code

4. **Keep ESP-IDF and libraries updated** for security patches

## Next Steps

See full documentation:
- `docs/WIFI_MANAGER.md` - Complete WiFi Manager guide
- `docs/WIFI_IMPLEMENTATION_SUMMARY.md` - Implementation details
- `README.md` - Project overview

## Need Help?

Check the troubleshooting section in `docs/WIFI_MANAGER.md` or open an issue on GitHub.
