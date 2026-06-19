#ifndef MQTT_MANAGER_H
#define MQTT_MANAGER_H
#include <stdbool.h>
bool mqtt_is_running(void);
void mqtt_start(void);
int mqtt_publish(const char *msg);
void mqtt_manager_stop(void);
#endif