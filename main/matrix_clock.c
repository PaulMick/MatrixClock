#include <stdio.h>

#include "display_manager.h"

#include "modes_assets.h"
#include "clock.h"

// Runs the entire program, kinda important
void matrix_clock_run() {
    display_init(0);

    // Main application loop
    while (1) {
        display_update();
    }
}