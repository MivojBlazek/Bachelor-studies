#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_bt.h"
#include "esp_bt_main.h"
#include "esp_bt_device.h"
#include "esp_spp_api.h"
#include "nvs_flash.h"
#include "esp_mac.h"

#define SPP_TAG "ESP32_SPP_SERVER"

void app_main(void) {
    // Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NOT_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase()); // This erases the NVS partition
        ret = nvs_flash_init(); // Reinitialize NVS
    }
    ESP_ERROR_CHECK(ret); // Check for errors


    // Initialize Bluetooth controller
    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();

    ret = esp_bt_controller_init(&bt_cfg);
    if (ret) {
        ESP_LOGE(SPP_TAG, "Bluetooth controller initialize failed: %s", esp_err_to_name(ret));
        return;
    }
    ESP_LOGI(SPP_TAG, "Bluetooth controller initialized successfully.");

    // Enable Bluetooth controller
    ret = esp_bt_controller_enable(ESP_BT_MODE_BTDM);
    if (ret) {
        ESP_LOGE(SPP_TAG, "Bluetooth controller enable failed: %s", esp_err_to_name(ret));
        return;
    }
    ESP_LOGI(SPP_TAG, "Bluetooth controller enabled successfully.");

    // Initialize Bluedroid
    ret = esp_bluedroid_init();
    if (ret) {
        ESP_LOGE(SPP_TAG, "Bluedroid initialize failed: %s", esp_err_to_name(ret));
        return;
    }
    ESP_LOGI(SPP_TAG, "Bluedroid initialized successfully.");

    // Enable Bluedroid
    ret = esp_bluedroid_enable();
    if (ret) {
        ESP_LOGE(SPP_TAG, "Bluedroid enable failed: %s", esp_err_to_name(ret));
        return;
    }
    ESP_LOGI(SPP_TAG, "Bluedroid enabled successfully.");

    // Get and print the Bluetooth address
    uint8_t bt_address[6];
    esp_read_mac(bt_address, ESP_MAC_BT);
    ESP_LOGI(SPP_TAG, "Bluetooth device address: %02X:%02X:%02X:%02X:%02X:%02X",
             bt_address[0], bt_address[1], bt_address[2],
             bt_address[3], bt_address[4], bt_address[5]);

    // Keep the task running
    while (1) {
        vTaskDelay(pdMS_TO_TICKS(1000)); // Print every second
    }
}
