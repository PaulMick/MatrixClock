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

    mode_t mode = MODE_CLOCK;

    // Main application loop
    while (1) {
        switch (mode) {
            case MODE_MENU:
                break;
            case MODE_CLOCK:
                break;
            case MODE_CLOCKWEATHER:
                break;
            case MODE_WEATHER:
                break;
        }

        // Update display
        display_update();
    }
}