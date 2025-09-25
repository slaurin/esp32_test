#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_log.h>
#include <sdkconfig.h>
#include "led_control.h"

static const char *TAG = "ESP32S3_TEST";

#define BLINK_GPIO GPIO_NUM_48  // ESP32S3 built-in LED GPIO (adjust if different)
#define BLINK_PERIOD_MS 1000

void app_main(void)
{
    ESP_LOGI(TAG, "Starting ESP32S3 test application");
    ESP_LOGI(TAG, "ESP-IDF version: %s", esp_get_idf_version());
    
    // Initialize LED control using custom component
    esp_err_t ret = led_control_init(BLINK_GPIO);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize LED control: %s", esp_err_to_name(ret));
        return;
    }
    
    ESP_LOGI(TAG, "LED control initialized successfully on GPIO %d", BLINK_GPIO);
    
    // Main application loop - demonstrate LED control component usage
    while (1) {
        // Turn LED on
        led_control_on(BLINK_GPIO);
        ESP_LOGI(TAG, "LED ON");
        vTaskDelay(BLINK_PERIOD_MS / portTICK_PERIOD_MS);
        
        // Turn LED off
        led_control_off(BLINK_GPIO);
        ESP_LOGI(TAG, "LED OFF");
        vTaskDelay(BLINK_PERIOD_MS / portTICK_PERIOD_MS);
    }
}