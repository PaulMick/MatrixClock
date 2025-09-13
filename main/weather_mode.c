#include <stdio.h>
#include <time.h>
#include <sys/time.h>

#include "modes_assets.h"

modestate_weather_t get_weather_state() {
    modestate_weather_t state = {
        .temp_f = 100,
        .temp_high_f = 255,
        .temp_low_f = -10,
        .weather_code = 0,
        .sunset_hour = 18,
        .sunset_minute = 7
    };
    return state;
}