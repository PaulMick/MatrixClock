#include <stdio.h>

#include "display_manager.h"

// Runs the entire program, kinda important
void matrix_clock_run() {
    display_init();

    // Main application loop
    while (1) {
        display_update();
    }
}