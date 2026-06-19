#include <stdio.h>

#include "nvs_flash.h"
#include "esp_netif.h"
#include "esp_event.h"
#include "esp_log.h"
#include "wifi_manager.h"
#include "ble_manager.h"
#include "mqtt_manager.h"

char ssid[32];
char password[64];
char json_payload[100];

void app_main(void)
{
    sprintf(json_payload,
            "{\"mcb1\":\"ON\",\"mcb2\":\"OFF\",\"mcb3\":\"ON\",\"mcb4\":\"OFF\"}");

    ESP_ERROR_CHECK(nvs_flash_init());

    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    wifi_manager_init();
    ble_manager_init();

    if (load_wifi_credentials(
            ssid,
            sizeof(ssid),
            password,
            sizeof(password)))
    {
        ESP_LOGI("MAIN", "Saved credentials found");
        ESP_LOGI("MAIN", "SSID: %s", ssid);
        wifi_manager_connect(ssid, password);
    }
    else
    {
        ESP_LOGI("MAIN", "No credentials found");

    }

        while (1)
        {
            if (wifi_is_connected() &&
                mqtt_is_running())
            {
                int msg_id = mqtt_publish(json_payload);

                if (msg_id >= 0)
                {
                    printf("Publish ID = %d\n", msg_id);
                }
                }

            vTaskDelay(pdMS_TO_TICKS(1000));
        }
}