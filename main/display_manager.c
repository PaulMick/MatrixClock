#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#include "display_driver.h"
#include "display_utils.h"
#include "display_assets.h"
#include "modes_assets.h"

DisplayHandle display_handle;

int y = -5;

void display_init(int loading_screen) {
    printf("Initializing Display\n");

    display_handle = get_display_handle();

    run_refresh();
    printf("Display Initialization Complete\n");
}

void display_update(modestates_t mode_states, current_mode_t mode) {
    fill_display(display_handle.frame_buf_ptr, 0, 0, 0);
    // Main Display

    draw_rect(display_handle.frame_buf_ptr, 0, 0, 40, 20, -1, 0, 255, 0);

    draw_str(display_handle.frame_buf_ptr, "HELLO", FONT_5x5_FLEX, 0, y, 1, 255, 0, 255);
    y ++;
    if (y > 32) {
        y = -5;
    }

    draw_img(display_handle.frame_buf_ptr, IMG_WIFI_CONNECTED, 32, 0);

    

    // End Main Display
    prep_bitplanes();
    *(display_handle.in_done_ptr) = 1;
    vTaskDelay(30 / portTICK_PERIOD_MS); // Do not remove
}