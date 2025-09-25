#include "led_control.h"
#include "esp_log.h"

static const char *TAG = "LED_CONTROL";

esp_err_t led_control_init(gpio_num_t gpio_num)
{
    ESP_LOGI(TAG, "Initializing LED control on GPIO %d", gpio_num);
    
    // Reset GPIO to default state
    gpio_reset_pin(gpio_num);
    
    // Set GPIO direction to output
    esp_err_t ret = gpio_set_direction(gpio_num, GPIO_MODE_OUTPUT);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to set GPIO %d direction: %s", gpio_num, esp_err_to_name(ret));
        return ret;
    }
    
    // Initialize LED to OFF state
    ret = gpio_set_level(gpio_num, 0);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to set initial LED state: %s", esp_err_to_name(ret));
        return ret;
    }
    
    ESP_LOGI(TAG, "LED control initialized successfully");
    return ESP_OK;
}

esp_err_t led_control_on(gpio_num_t gpio_num)
{
    return gpio_set_level(gpio_num, 1);
}

esp_err_t led_control_off(gpio_num_t gpio_num)
{
    return gpio_set_level(gpio_num, 0);
}

esp_err_t led_control_toggle(gpio_num_t gpio_num)
{
    int current_level = gpio_get_level(gpio_num);
    return gpio_set_level(gpio_num, !current_level);
}

esp_err_t led_control_set_state(gpio_num_t gpio_num, bool state)
{
    return gpio_set_level(gpio_num, state ? 1 : 0);
}