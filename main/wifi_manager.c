#include "wifi_manager.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "esp_log.h"
#include "keys.h"
#include "nvs.h"
#include "nvs_flash.h"
#include "mqtt_manager.h"
static const char *TAG = "WIFI";
static bool mqtt_started = false;
static wifi_ap_record_t ap_info;
static void wifi_event_handler(
    void *arg,
    esp_event_base_t event_base,
    int32_t event_id,
    void *event_data);
void wifi_manager_connect(const char *ssid,
                          const char *password)
{
    wifi_config_t wifi_config = {0};

    strncpy((char *)wifi_config.sta.ssid,
            ssid,
            sizeof(wifi_config.sta.ssid) - 1);

    strncpy((char *)wifi_config.sta.password,
            password,
            sizeof(wifi_config.sta.password) - 1);

    esp_wifi_set_mode(WIFI_MODE_STA);

    esp_wifi_set_config(
        WIFI_IF_STA,
        &wifi_config);

    esp_wifi_connect();

    ESP_LOGI("WIFI",
             "Connecting to %s",
             ssid);
}
void wifi_manager_init(void)
{
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();

    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    ESP_ERROR_CHECK(
        esp_event_handler_register(
            WIFI_EVENT,
            ESP_EVENT_ANY_ID,
            &wifi_event_handler,
            NULL));

    ESP_ERROR_CHECK(
        esp_event_handler_register(
            IP_EVENT,
            IP_EVENT_STA_GOT_IP,
            &wifi_event_handler,
            NULL));

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));

    ESP_ERROR_CHECK(esp_wifi_start());
}
static void wifi_event_handler(
    void *arg,
    esp_event_base_t event_base,
    int32_t event_id,
    void *event_data)
{
    if (event_base == WIFI_EVENT)
    {
        switch (event_id)
        {
        case WIFI_EVENT_STA_START:
            ESP_LOGI(TAG, "WiFi Started");
            break;

        case WIFI_EVENT_STA_DISCONNECTED:
            ESP_LOGW(TAG, "WiFi Disconnected");

            if (mqtt_started)
            {
                mqtt_manager_stop();
                mqtt_started = false;
            }

            esp_wifi_connect();
            break;
        }
    }

    if (event_base == IP_EVENT &&
        event_id == IP_EVENT_STA_GOT_IP)
    {
        ip_event_got_ip_t *event =
            (ip_event_got_ip_t *)event_data;

        ESP_LOGI(TAG,
                 "Got IP: " IPSTR,
                 IP2STR(&event->ip_info.ip));

        if (!mqtt_started)
        {
            mqtt_start();
            mqtt_started = true;
        }
    }
}


bool wifi_is_connected(void)
{
    return (esp_wifi_sta_get_ap_info(&ap_info) == ESP_OK);
}
void save_wifi_credentials(const char *ssid,
                           const char *password)
{
    nvs_handle_t nvs_handle;

    if (nvs_open("storage",
                 NVS_READWRITE,
                 &nvs_handle) == ESP_OK)
    {
        nvs_set_str(nvs_handle, "ssid", ssid);
        nvs_set_str(nvs_handle, "pass", password);

        nvs_commit(nvs_handle);
        nvs_close(nvs_handle);
    }
}
bool load_wifi_credentials(char *ssid,
                           size_t ssid_len,
                           char *pass,
                           size_t pass_len)
{
    nvs_handle_t nvs_handle;

    if (nvs_open("storage",
                 NVS_READONLY,
                 &nvs_handle) != ESP_OK)
    {
        return false;
    }

    if (nvs_get_str(nvs_handle,
                    "ssid",
                    ssid,
                    &ssid_len) != ESP_OK)
    {
        nvs_close(nvs_handle);
        return false;
    }

    if (nvs_get_str(nvs_handle,
                    "pass",
                    pass,
                    &pass_len) != ESP_OK)
    {
        nvs_close(nvs_handle);
        return false;
    }

    nvs_close(nvs_handle);
    return true;
}