#include "gpio_manager.h"

void gpio_manager_init(void)
{
    gpio_config_t io_conf = {
        .pin_bit_mask =
            (1ULL << MCB1_GPIO) |
            (1ULL << MCB2_GPIO) |
            (1ULL << MCB3_GPIO) |
            (1ULL << MCB4_GPIO),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };

    gpio_config(&io_conf);
}

int gpio_manager_get_mcb1(void)
{
    return gpio_get_level(MCB1_GPIO);
}

int gpio_manager_get_mcb2(void)
{
    return gpio_get_level(MCB2_GPIO);
}

int gpio_manager_get_mcb3(void)
{
    return gpio_get_level(MCB3_GPIO);
}

int gpio_manager_get_mcb4(void)
{
    return gpio_get_level(MCB4_GPIO);
}