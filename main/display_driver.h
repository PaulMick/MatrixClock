#ifndef DISPLAY_DRIVER_H
#define DISPLAY_DRIVER_H

#include <stdint.h>

#define DISPLAY_HEIGHT 32
#define DISPLAY_WIDTH 64

typedef struct {
    uint8_t ****frame_buf_ptr;
    int width;
    int height;
    int *in_done_ptr;
} DisplayHandle;

DisplayHandle get_display_handle();

void run_refresh();

#endif