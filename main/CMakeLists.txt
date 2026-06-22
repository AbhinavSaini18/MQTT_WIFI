idf_component_register(
    SRCS 
        "mqtt/mqtt_manager.c" 
        "wifi/wifi_manager.c"
        "ble/ble_manager.c"
        "gpio/gpio_manager.c"
        "main.c"
    INCLUDE_DIRS "."
                "wifi"
                "mqtt"
                "ble"
                "gpio"
    REQUIRES esp_wifi
            nvs_flash
            esp_event
            esp_netif
            mqtt
            bt
            esp_driver_gpio
)