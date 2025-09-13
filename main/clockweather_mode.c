#include <stdio.h>
#include <time.h>
#include <sys/time.h>

#include "modes_assets.h"

modestate_clockweather_t get_clockweather_state() {
    modestate_clockweather_t state = {
        .month = 10,
        .month_day = 11,
        .week_day = 2,
        .hour = 12,
        .minute = 59,
        .temp_f = 100,
        .temp_high_f = 255,
        .temp_low_f = -10,
        .weather_code = 0,
    };
    return state;
}