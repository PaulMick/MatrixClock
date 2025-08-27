#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_wifi.h>

#include "secrets.h"

void app_main(void) {
    extern char* wifi_ssid;

    while (1) {
        printf("%s\n", wifi_ssid);
    }
}