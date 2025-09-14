#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#include "display_driver.h"
#include "display_utils.h"
#include "display_assets.h"
#include "modes_assets.h"

DisplayHandle display_handle;

void display_init(int loading_screen) {
    printf("Initializing Display\n");

    display_handle = get_display_handle();

    run_refresh();
    printf("Display Initialization Complete\n");
}

void display_update(modestates_t mode_states, current_mode_t mode) {
    fill_display(display_handle.frame_buf_ptr, 0, 0, 0);
    // Main Display

    switch (mode) {
        case MODE_MENU:
            draw_menu(display_handle.frame_buf_ptr, mode_states.menu);
            break;
        case MODE_CLOCK:
            draw_clock(display_handle.frame_buf_ptr, mode_states.clock);
            break;
        case MODE_CLOCKWEATHER:
            draw_clockweather(display_handle.frame_buf_ptr, mode_states.clockweather);
            break;
        case MODE_WEATHER:
            draw_weather(display_handle.frame_buf_ptr, mode_states.weather);
            break;
    }

    // End Main Display
    prep_bitplanes();
    *(display_handle.in_done_ptr) = 1;
    vTaskDelay(30 / portTICK_PERIOD_MS); // Do not remove
}