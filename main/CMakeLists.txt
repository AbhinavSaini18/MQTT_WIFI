idf_component_register(
    SRCS "mqtt_manager.c" "wifi_manager.c" "ble_manager.c" "main.c"
    INCLUDE_DIRS "."
    REQUIRES
        bt
        nvs_flash
        esp_wifi
        esp_event
        esp_netif
        mqtt
        
)