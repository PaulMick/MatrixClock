#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#include "display_driver.h"
#include "display_utils.h"

DisplayHandle display_handle;

void display_init(int loading_screen) {
    printf("Initializing Display\n");

    display_handle = get_display_handle();

    run_refresh();
    printf("Display Initialization Complete\n");
}

void display_update() {
    draw_pixel(display_handle.frame_buf_ptr, 63, 31, 255, 255, 255);
    draw_char_5x5_flex(display_handle.frame_buf_ptr, 'P', 15, 31, 255, 255, 255);

    *(display_handle.in_done_ptr) = 1;
    vTaskDelay(1000 / portTICK_PERIOD_MS);
}