#ifndef GPIO_MANAGER_H
#define GPIO_MANAGER_H

#include "driver/gpio.h"
#include "keys.h"

/* Initialize all MCB input pins */
void gpio_manager_init(void);

/* Read individual MCB states */
int gpio_manager_get_mcb1(void);
int gpio_manager_get_mcb2(void);
int gpio_manager_get_mcb3(void);
int gpio_manager_get_mcb4(void);

#endif /* GPIO_MANAGER_H */