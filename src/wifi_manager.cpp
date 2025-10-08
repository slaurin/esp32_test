#include "wifi_manager.h"

#ifdef ARDUINO

// Static member definitions
bool WiFiManager::connected = false;
unsigned long WiFiManager::lastConnectionAttempt = 0;
int WiFiManager::connectionAttempts = 0;

void WiFiManager::init() {
    Serial.println("Initializing WiFi Manager...");
    
    // Set WiFi mode to station (client)
    WiFi.mode(WIFI_STA);
    
    // Disconnect from any previous connections
    WiFi.disconnect();
    delay(100);
    
    Serial.println("WiFi Manager initialized");
}

void WiFiManager::connect() {
    if (connected) {
        return; // Already connected
    }
    
    if (connectionAttempts >= MAX_CONNECTION_ATTEMPTS && 
        (millis() - lastConnectionAttempt) < RECONNECT_INTERVAL) {
        return; // Wait before retrying
    }
    
    // Reset connection attempts if enough time has passed
    if ((millis() - lastConnectionAttempt) >= RECONNECT_INTERVAL) {
        connectionAttempts = 0;
    }
    
    Serial.println("Connecting to WiFi...");
    Serial.print("SSID: ");
    Serial.println(WIFI_SSID);
    
    lastConnectionAttempt = millis();
    connectionAttempts++;
    
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    
    unsigned long startAttemptTime = millis();
    
    // Wait for connection with timeout
    while (WiFi.status() != WL_CONNECTED && 
           millis() - startAttemptTime < WIFI_TIMEOUT_MS) {
        delay(500);
        Serial.print(".");
    }
    Serial.println();
    
    if (WiFi.status() == WL_CONNECTED) {
        connected = true;
        connectionAttempts = 0; // Reset attempts on success
        Serial.println("WiFi connected successfully!");
        Serial.print("IP Address: ");
        Serial.println(WiFi.localIP());
        Serial.print("MAC Address: ");
        Serial.println(WiFi.macAddress());
        Serial.print("Signal Strength (RSSI): ");
        Serial.print(WiFi.RSSI());
        Serial.println(" dBm");
    } else {
        connected = false;
        Serial.println("WiFi connection failed!");
        Serial.print("Connection attempt ");
        Serial.print(connectionAttempts);
        Serial.print(" of ");
        Serial.println(MAX_CONNECTION_ATTEMPTS);
        
        if (connectionAttempts >= MAX_CONNECTION_ATTEMPTS) {
            Serial.println("Maximum connection attempts reached. Will retry in 30 seconds.");
        }
    }
}

void WiFiManager::disconnect() {
    if (connected) {
        Serial.println("Disconnecting from WiFi...");
        WiFi.disconnect();
        connected = false;
        Serial.println("WiFi disconnected");
    }
}

void WiFiManager::loop() {
    // Check if connection was lost
    if (connected && WiFi.status() != WL_CONNECTED) {
        connected = false;
        Serial.println("WiFi connection lost!");
    }
    
    // Try to reconnect if not connected
    if (!connected && WiFi.status() != WL_CONNECTED) {
        connect();
    }
}

bool WiFiManager::isConnected() {
    return connected && WiFi.status() == WL_CONNECTED;
}

String WiFiManager::getIPAddress() {
    if (isConnected()) {
        return WiFi.localIP().toString();
    }
    return "Not connected";
}

String WiFiManager::getMACAddress() {
    return WiFi.macAddress();
}

int WiFiManager::getRSSI() {
    if (isConnected()) {
        return WiFi.RSSI();
    }
    return 0;
}

String WiFiManager::getSSID() {
    if (isConnected()) {
        return WiFi.SSID();
    }
    return "Not connected";
}

#else

// Static member definitions for native/unit-test builds
bool WiFiManager::connected = false;
unsigned long WiFiManager::lastConnectionAttempt = 0;
int WiFiManager::connectionAttempts = 0;

void WiFiManager::init() {}

void WiFiManager::connect() {}

void WiFiManager::disconnect() {}

void WiFiManager::loop() {}

bool WiFiManager::isConnected() {
    return false;
}

String WiFiManager::getIPAddress() {
    return "Not connected";
}

String WiFiManager::getMACAddress() {
    return "00:00:00:00:00:00";
}

int WiFiManager::getRSSI() {
    return 0;
}

String WiFiManager::getSSID() {
    return "Not connected";
}

#endif
