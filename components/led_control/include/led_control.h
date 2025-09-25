#pragma once

#include "driver/gpio.h"
#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize LED control
 * 
 * @param gpio_num GPIO number for LED
 * @return esp_err_t ESP_OK on success
 */
esp_err_t led_control_init(gpio_num_t gpio_num);

/**
 * @brief Turn LED on
 * 
 * @param gpio_num GPIO number for LED
 * @return esp_err_t ESP_OK on success
 */
esp_err_t led_control_on(gpio_num_t gpio_num);

/**
 * @brief Turn LED off
 * 
 * @param gpio_num GPIO number for LED  
 * @return esp_err_t ESP_OK on success
 */
esp_err_t led_control_off(gpio_num_t gpio_num);

/**
 * @brief Toggle LED state
 * 
 * @param gpio_num GPIO number for LED
 * @return esp_err_t ESP_OK on success
 */
esp_err_t led_control_toggle(gpio_num_t gpio_num);

/**
 * @brief Set LED state
 * 
 * @param gpio_num GPIO number for LED
 * @param state true for on, false for off
 * @return esp_err_t ESP_OK on success
 */
esp_err_t led_control_set_state(gpio_num_t gpio_num, bool state);

#ifdef __cplusplus
}
#endif