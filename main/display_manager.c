#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#include "display_driver.h"
#include "display_utils.h"

DisplayHandle display_handle;

void display_init() {
    printf("Initializing Display\n");

    display_handle = get_display_handle();
    int load_font_result = load_font();

    run_refresh();
    printf("Display Initialization Complete\n");
}

void display_update() {
    (*(display_handle.frame_buf_ptr))[31][63][0] = 255;
    *(display_handle.in_done_ptr) = 1;
    
    vTaskDelay(1000 / portTICK_PERIOD_MS);
}