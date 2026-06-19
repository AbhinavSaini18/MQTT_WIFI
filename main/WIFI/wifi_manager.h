#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <stdbool.h>
#include <stddef.h>

void wifi_manager_init(void);

void wifi_manager_connect(const char *ssid,
                          const char *password);

bool wifi_is_connected(void);

void save_wifi_credentials(const char *ssid,
                           const char *password);

bool load_wifi_credentials(char *ssid,
                           size_t ssid_len,
                           char *pass,
                           size_t pass_len);

#endif