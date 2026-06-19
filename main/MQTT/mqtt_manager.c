#include "mqtt_manager.h"
#include "keys.h"
#include "mqtt_client.h"
#include "esp_log.h"

static const char *TAG = "MQTT";
static esp_mqtt_client_handle_t client;

void mqtt_start(void)
{
    ESP_LOGI(TAG, "Initializing MQTT");

    esp_mqtt_client_config_t mqtt_cfg = {
        .broker.address.uri = MQTT_BROKER,

        .credentials.username = MQTT_USERNAME,
        .credentials.authentication.password = MQTT_PASSWORD,

        .session.protocol_ver = MQTT_PROTOCOL_V_3_1_1,
    };

    client = esp_mqtt_client_init(&mqtt_cfg);

    if (client == NULL)
    {
        ESP_LOGE(TAG, "Failed to create MQTT client");
        return;
    }

    esp_mqtt_client_start(client);

    ESP_LOGI(TAG, "MQTT Started");
}
void mqtt_manager_stop(void)
{
    if (client != NULL)
    {
        ESP_LOGI(TAG, "Stopping MQTT");

        esp_mqtt_client_stop(client);
        esp_mqtt_client_destroy(client);

        client = NULL;

        ESP_LOGI(TAG, "MQTT stopped");
    }
}
bool mqtt_is_running(void)
{
    return (client != NULL);
}
int mqtt_publish(const char *msg)
{
    return esp_mqtt_client_publish(
        client,
        MQTT_TOPIC,
        msg,
        0,
        1,
        0
    );
}