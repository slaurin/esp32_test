# WiFi Examples

This document provides practical examples of using WiFi functionality in your ESP32-S3 project.

## Example 1: Basic WiFi Connection

The main project already includes basic WiFi connectivity. This is what's happening in `main.cpp`:

```cpp
#include "wifi_manager.h"

void setup() {
    Serial.begin(115200);
    
    // Initialize and connect to WiFi
    WiFiManager::init();
    WiFiManager::connect();
}

void loop() {
    // Maintain WiFi connection
    WiFiManager::loop();
    
    // Your other code here
    delay(100);
}
```

## Example 2: Check WiFi Status

```cpp
void loop() {
    WiFiManager::loop();
    
    // Print status every 10 seconds
    static unsigned long lastCheck = 0;
    if (millis() - lastCheck > 10000) {
        if (WiFiManager::isConnected()) {
            Serial.println("WiFi is connected");
            Serial.print("IP: ");
            Serial.println(WiFiManager::getIPAddress());
            Serial.print("Signal: ");
            Serial.print(WiFiManager::getRSSI());
            Serial.println(" dBm");
        } else {
            Serial.println("WiFi is disconnected");
        }
        lastCheck = millis();
    }
}
```

## Example 3: Web Server (Future Enhancement)

Here's how you could add a simple web server to the project:

```cpp
// Add to platformio.ini:
// lib_deps = 
//     h2zero/NimBLE-Arduino @ ^1.4.1
//     ESP Async WebServer

#include <ESPAsyncWebServer.h>
#include "wifi_manager.h"

AsyncWebServer server(80);

void setup() {
    Serial.begin(115200);
    
    WiFiManager::init();
    WiFiManager::connect();
    
    // Wait for WiFi connection
    while (!WiFiManager::isConnected()) {
        WiFiManager::loop();
        delay(100);
    }
    
    // Setup web server routes
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        String html = "<html><body>";
        html += "<h1>ESP32-S3 Web Server</h1>";
        html += "<p>Temperature: " + String(TemperatureService::getCurrentTemperature()) + "°C</p>";
        html += "<p>BLE Connected: " + String(BLEServerManager::isConnected() ? "Yes" : "No") + "</p>";
        html += "<p>WiFi IP: " + WiFiManager::getIPAddress() + "</p>";
        html += "<p>WiFi RSSI: " + String(WiFiManager::getRSSI()) + " dBm</p>";
        html += "</body></html>";
        request->send(200, "text/html", html);
    });
    
    server.begin();
    Serial.println("Web server started");
    Serial.print("Visit: http://");
    Serial.println(WiFiManager::getIPAddress());
}
```

## Example 4: HTTP Client (Future Enhancement)

Send temperature data to a cloud service:

```cpp
#include <HTTPClient.h>
#include "wifi_manager.h"

void sendDataToCloud() {
    if (!WiFiManager::isConnected()) {
        Serial.println("WiFi not connected, skipping upload");
        return;
    }
    
    HTTPClient http;
    
    // Your API endpoint
    http.begin("http://api.example.com/temperature");
    http.addHeader("Content-Type", "application/json");
    
    // Create JSON payload
    String payload = "{";
    payload += "\"temperature\":" + String(TemperatureService::getCurrentTemperature());
    payload += ",\"unit\":\"" + String(TemperatureService::getUnit() == CELSIUS ? "C" : "F") + "\"";
    payload += ",\"rssi\":" + String(WiFiManager::getRSSI());
    payload += "}";
    
    // Send POST request
    int httpCode = http.POST(payload);
    
    if (httpCode > 0) {
        Serial.printf("HTTP Response code: %d\n", httpCode);
        String response = http.getString();
        Serial.println(response);
    } else {
        Serial.printf("HTTP Request failed: %s\n", http.errorToString(httpCode).c_str());
    }
    
    http.end();
}

void loop() {
    WiFiManager::loop();
    
    // Send data every 5 minutes
    static unsigned long lastUpload = 0;
    if (WiFiManager::isConnected() && millis() - lastUpload > 300000) {
        sendDataToCloud();
        lastUpload = millis();
    }
}
```

## Example 5: NTP Time Synchronization (Future Enhancement)

Get current time from internet:

```cpp
#include <time.h>
#include "wifi_manager.h"

void setupTime() {
    if (!WiFiManager::isConnected()) {
        Serial.println("WiFi not connected, cannot sync time");
        return;
    }
    
    // Configure NTP
    configTime(0, 0, "pool.ntp.org", "time.nist.gov");
    
    Serial.println("Waiting for NTP time sync...");
    time_t now = time(nullptr);
    int attempts = 0;
    while (now < 1000000000 && attempts < 20) {
        delay(500);
        Serial.print(".");
        now = time(nullptr);
        attempts++;
    }
    Serial.println();
    
    if (now > 1000000000) {
        Serial.println("Time synchronized!");
        struct tm timeinfo;
        localtime_r(&now, &timeinfo);
        Serial.printf("Current time: %s", asctime(&timeinfo));
    } else {
        Serial.println("Failed to sync time");
    }
}

void setup() {
    Serial.begin(115200);
    
    WiFiManager::init();
    WiFiManager::connect();
    
    // Wait for connection
    while (!WiFiManager::isConnected()) {
        WiFiManager::loop();
        delay(100);
    }
    
    setupTime();
}
```

## Example 6: MQTT Client (Future Enhancement)

Publish sensor data to MQTT broker:

```cpp
// Add to platformio.ini:
// lib_deps = 
//     h2zero/NimBLE-Arduino @ ^1.4.1
//     knolleary/PubSubClient @ ^2.8

#include <PubSubClient.h>
#include "wifi_manager.h"

WiFiClient espClient;
PubSubClient mqtt(espClient);

const char* mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;

void reconnectMQTT() {
    if (!WiFiManager::isConnected()) {
        return;
    }
    
    while (!mqtt.connected()) {
        Serial.print("Connecting to MQTT...");
        String clientId = "ESP32-" + WiFiManager::getMACAddress();
        
        if (mqtt.connect(clientId.c_str())) {
            Serial.println("connected");
        } else {
            Serial.print("failed, rc=");
            Serial.println(mqtt.state());
            delay(5000);
        }
    }
}

void publishTemperature() {
    if (mqtt.connected()) {
        String topic = "esp32/temperature";
        String payload = String(TemperatureService::getCurrentTemperature());
        mqtt.publish(topic.c_str(), payload.c_str());
        Serial.println("Temperature published to MQTT");
    }
}

void setup() {
    Serial.begin(115200);
    
    WiFiManager::init();
    WiFiManager::connect();
    
    mqtt.setServer(mqtt_server, mqtt_port);
}

void loop() {
    WiFiManager::loop();
    
    if (WiFiManager::isConnected()) {
        if (!mqtt.connected()) {
            reconnectMQTT();
        }
        mqtt.loop();
        
        // Publish every 30 seconds
        static unsigned long lastPublish = 0;
        if (millis() - lastPublish > 30000) {
            publishTemperature();
            lastPublish = millis();
        }
    }
}
```

## Example 7: OTA Updates (Future Enhancement)

Enable firmware updates over WiFi:

```cpp
#include <ArduinoOTA.h>
#include "wifi_manager.h"

void setupOTA() {
    ArduinoOTA.setHostname("esp32-ble-device");
    ArduinoOTA.setPassword("admin");  // Change this!
    
    ArduinoOTA.onStart([]() {
        String type = (ArduinoOTA.getCommand() == U_FLASH) ? "sketch" : "filesystem";
        Serial.println("Start updating " + type);
    });
    
    ArduinoOTA.onEnd([]() {
        Serial.println("\nEnd");
    });
    
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    });
    
    ArduinoOTA.onError([](ota_error_t error) {
        Serial.printf("Error[%u]: ", error);
        if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
        else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
        else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
        else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
        else if (error == OTA_END_ERROR) Serial.println("End Failed");
    });
    
    ArduinoOTA.begin();
    Serial.println("OTA ready");
}

void setup() {
    Serial.begin(115200);
    
    WiFiManager::init();
    WiFiManager::connect();
    
    while (!WiFiManager::isConnected()) {
        WiFiManager::loop();
        delay(100);
    }
    
    setupOTA();
}

void loop() {
    WiFiManager::loop();
    
    if (WiFiManager::isConnected()) {
        ArduinoOTA.handle();
    }
}
```

## Notes on Examples

Most of the examples above are **future enhancements** and would require:
1. Adding additional libraries to `platformio.ini`
2. Testing for compatibility with BLE and other services
3. Memory and performance considerations
4. Error handling and recovery

The current implementation provides the **foundation** for all these use cases by providing reliable WiFi connectivity.

## Testing Examples

Before implementing complex examples:
1. Ensure basic WiFi connection works
2. Test connectivity stability
3. Verify BLE still works properly
4. Check memory usage
5. Test under various network conditions

## Performance Considerations

- **Memory**: Each additional service uses RAM
- **CPU**: Balance between WiFi, BLE, and application tasks
- **Power**: WiFi consumes more power than BLE
- **Network**: Consider network latency and timeouts

## See Also

- `WIFI_MANAGER.md` - Full WiFi Manager documentation
- `WIFI_QUICK_START.md` - Quick setup guide
- `WIFI_IMPLEMENTATION_SUMMARY.md` - Implementation details
