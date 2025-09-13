#include <stdio.h>
#include <time.h>
#include <sys/time.h>

#include "modes_assets.h"

modestate_clock_t get_clock_state() {
    modestate_clock_t state = {
        .year = 2025,
        .month = 10,
        .month_day = 11,
        .week_day = 2,
        .hour = 12,
        .minute = 59,
        .second = 59,
    };
    return state;
}