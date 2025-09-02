#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#include "display_driver.h"
#include "display_utils.h"

DisplayHandle displayHandle;

void display_init() {
    printf("Initializing Display\n");

    displayHandle = get_display_handle();
    int load_font_result = load_font();

    run_refresh();
    printf("Display Initialization Complete\n");
}

void display_update() {
    // printf("update ");
    vTaskDelay(10 / portTICK_PERIOD_MS);
}