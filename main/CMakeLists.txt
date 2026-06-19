idf_component_register(
    SRCS
        "main.c"
        "wifi/wifi_manager.c"
        "ble/ble_manager.c"
        "mqtt/mqtt_manager.c"

    INCLUDE_DIRS
        "."
        "wifi"
        "ble"
        "mqtt"

    REQUIRES
        bt
        nvs_flash
        esp_wifi
        esp_event
        esp_netif
        mqtt
)   