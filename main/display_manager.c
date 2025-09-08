#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#include "display_driver.h"
#include "display_utils.h"
#include "assets.h"

DisplayHandle display_handle;

int x = 0;
int y = 0;

void display_init(int loading_screen) {
    printf("Initializing Display\n");

    display_handle = get_display_handle();

    run_refresh();
    printf("Display Initialization Complete\n");
}

void display_update() {
    fill_display(display_handle.frame_buf_ptr, 0, 0, 0);
    // Main Display

    // End Main Display
    prep_bitplanes();
    *(display_handle.in_done_ptr) = 1;
    vTaskDelay(10 / portTICK_PERIOD_MS); // Do not remove
}