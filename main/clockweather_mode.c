#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include "esp_netif_sntp.h"
#include "esp_sntp.h"

#include "modes_assets.h"

// esp_sntp_config_t config = ESP_NETIF_SNTP_DEFAULT_CONFIG("pool.ntp.org");
// esp_netif_sntp_init(&config);

time_t now;
struct tm timeinfo;

modestate_clockweather_t get_clockweather_state() {
    time(&now);
    setenv("TZ", "EST", 1);
    tzset();
    localtime_r(&now, &timeinfo);
    printf("epoch %lld\n", now);
    modestate_clockweather_t state = {
        .month = timeinfo.tm_mon,
        .month_day = timeinfo.tm_mday,
        .week_day = timeinfo.tm_wday,
        .hour = timeinfo.tm_hour,
        .minute = timeinfo.tm_min,
        .temp_f = 100,
        .temp_high_f = 255,
        .temp_low_f = -10,
        .weather_code = 0,
    };
    return state;
}