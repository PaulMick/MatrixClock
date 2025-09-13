#include <stdio.h>

#include "display_manager.h"

#include "modes_assets.h"

#include "menu_mode.h"
#include "clock_mode.h"
#include "clockweather_mode.h"
#include "weather_mode.h"

// Runs the entire program, kinda important
void matrix_clock_run() {
    display_init(0);

    current_mode_t mode = MODE_CLOCK;

    modestates_t mode_states = {
        .menu = {
            .selection = 0,
        },
        .clock = {
            .year = 2025,
            .month = 12,
            .month_day = 25,
            .week_day = 0,
            .hour = 22,
            .minute = 0,
            .second = 0
        },
        .clockweather = {
            .month = 12,
            .month_day = 25,
            .week_day = 0,
            .hour = 22,
            .minute = 0,
            .temp_f = 103,
            .temp_high_f = 104,
            .temp_low_f = 102,
            .weather_code = 0
        },
        .weather = {
            .temp_f = 103,
            .temp_high_f = 104,
            .temp_low_f = 102,
            .weather_code = 0,
            .sunset_hour = 20,
            .sunset_minute = 23,
        }
    };

    // Main application loop
    while (1) {
        switch (mode) {
            case MODE_MENU:
                mode_states.menu = get_menu_state();
                break;
            case MODE_CLOCK:
                mode_states.clock = get_clock_state();
                break;
            case MODE_CLOCKWEATHER:
                mode_states.clockweather = get_clockweather_state();
                break;
            case MODE_WEATHER:
                mode_states.weather = get_weather_state();
                break;
        }

        // Update display
        display_update(mode_states, mode);
    }
}