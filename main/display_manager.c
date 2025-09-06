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
    draw_pixel(display_handle.frame_buf_ptr, x, y, 255, 255, 255);
    x ++;
    if (x >= DISPLAY_WIDTH) {
        x = 0;
        y ++;
    }
    if (y >= DISPLAY_HEIGHT) {
        y = 0;
    }
    draw_str(display_handle.frame_buf_ptr, "Hello World!", FONT_5x5_FLEX, 2, 2, 1, 255, 255, 255);
    draw_rect(display_handle.frame_buf_ptr, 0, 0, 45, 9, 1, 255, 255, 255);

    *(display_handle.in_done_ptr) = 1;
    vTaskDelay(50 / portTICK_PERIOD_MS);
}