#include <unity.h>
#include <Arduino.h>
#include "../include/wifi_manager.h"

// Test WiFi manager initialization
void test_wifi_init() {
    WiFiManager::init();
    // After init, should not be connected yet (no network configured in tests)
    // This just ensures init doesn't crash
    TEST_ASSERT_TRUE(true);
}

// Test WiFi MAC address retrieval
void test_wifi_mac_address() {
    WiFiManager::init();
    String mac = WiFiManager::getMACAddress();
    // MAC address should be 17 characters (XX:XX:XX:XX:XX:XX format)
    TEST_ASSERT_EQUAL(17, mac.length());
}

// Test WiFi connection state (should be disconnected in test environment)
void test_wifi_connection_state() {
    WiFiManager::init();
    // In test environment without valid credentials, should not be connected
    bool connected = WiFiManager::isConnected();
    TEST_ASSERT_FALSE(connected);
}

// Test WiFi IP address when disconnected
void test_wifi_ip_disconnected() {
    WiFiManager::init();
    String ip = WiFiManager::getIPAddress();
    // When disconnected, should return "Not connected"
    TEST_ASSERT_EQUAL_STRING("Not connected", ip.c_str());
}

// Test WiFi SSID when disconnected
void test_wifi_ssid_disconnected() {
    WiFiManager::init();
    String ssid = WiFiManager::getSSID();
    // When disconnected, should return "Not connected"
    TEST_ASSERT_EQUAL_STRING("Not connected", ssid.c_str());
}

// Test WiFi RSSI when disconnected
void test_wifi_rssi_disconnected() {
    WiFiManager::init();
    int rssi = WiFiManager::getRSSI();
    // When disconnected, RSSI should be 0
    TEST_ASSERT_EQUAL(0, rssi);
}

void setUp(void) {
    // Set up test environment
}

void tearDown(void) {
    // Clean up after tests
}

void setup() {
    delay(2000); // Wait for serial
    
    UNITY_BEGIN();
    RUN_TEST(test_wifi_init);
    RUN_TEST(test_wifi_mac_address);
    RUN_TEST(test_wifi_connection_state);
    RUN_TEST(test_wifi_ip_disconnected);
    RUN_TEST(test_wifi_ssid_disconnected);
    RUN_TEST(test_wifi_rssi_disconnected);
    UNITY_END();
}

void loop() {
    // Nothing to do in loop for tests
}
