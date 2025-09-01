#include <stdio.h>

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
    uint8_t ***frame_buf;
    if (*(display_handle.buf_num_ptr) == 0) {
        frame_buf = (uint8_t ***) display_handle.frame_buf_ptr_1;
        *(display_handle.buf_num_ptr) = 1;
    } else {
        frame_buf = (uint8_t ***) display_handle.frame_buf_ptr_0;
        *(display_handle.buf_num_ptr) = 0;
    }

    frame_buf[31][63][0] = 255;
}