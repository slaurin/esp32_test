#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#ifdef ARDUINO
#include <Arduino.h>
#include <WiFi.h>
#else
#include <cstdint>
#include <string>

using String = std::string;

inline void delay(uint32_t) {}
inline unsigned long millis() { return 0; }
#endif

// WiFi Configuration
// Note: For security, WiFi credentials should be defined in platformio.ini using build_flags
// Example in platformio.ini:
//   build_flags = 
//     -D WIFI_SSID='"YourSSID"'
//     -D WIFI_PASSWORD='"YourPassword"'
//
// If not defined via build flags, default values are used (for development only)
#ifndef WIFI_SSID
#define WIFI_SSID        "YourSSID"        // Change this to your WiFi SSID
#endif

#ifndef WIFI_PASSWORD
#define WIFI_PASSWORD    "YourPassword"    // Change this to your WiFi password
#endif

#define WIFI_TIMEOUT_MS  20000             // WiFi connection timeout in milliseconds

// WiFi Manager class
class WiFiManager {
private:
    static bool connected;
    static unsigned long lastConnectionAttempt;
    static int connectionAttempts;
    static const int MAX_CONNECTION_ATTEMPTS = 3;
    static const unsigned long RECONNECT_INTERVAL = 30000; // 30 seconds

public:
    static void init();
    static void connect();
    static void disconnect();
    static void loop();
    static bool isConnected();
    static String getIPAddress();
    static String getMACAddress();
    static int getRSSI();
    static String getSSID();
};

#endif // WIFI_MANAGER_H
