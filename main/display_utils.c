#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "assets.h"

#define DISPLAY_WIDTH 64
#define DISPLAY_HEIGHT 32

// Safe function for drawing a single pixel, checks if pixel coordinates are on the display before writing
void draw_pixel(uint8_t ****frame_buf_ptr, int x, int y, uint8_t r, uint8_t g, uint8_t b) {
    if (x >= 0 && x < DISPLAY_WIDTH && y >= 0 && y < DISPLAY_HEIGHT) {
        (*frame_buf_ptr)[y][x][0] = r;
        (*frame_buf_ptr)[y][x][1] = g;
        (*frame_buf_ptr)[y][x][2] = b;
    }
}

// Draw a single ASCII character
// x, y are the top-left corner
int draw_char_5x5_flex(uint8_t ****frame_buf_ptr, char c, int x, int y, uint8_t r, uint8_t g, uint8_t b) {
    uint32_t font_char = font_5x5_flex[(int) c];
    int width = font_char >> 25;
    for (int i = 0; i < width; i ++) {
        for (int j = 0; j < 5; j ++) {
            if ((font_char >> (i * 5 + j)) & 0x01) {
                draw_pixel(frame_buf_ptr, x + i, y + j, r, g, b);
            }
        }
    }
    return width;
}

// Draw a string of ASCII characters
// x, y are the top-let corner
void draw_str(uint8_t ****frame_buf_ptr, char *str, font_t font, int x, int y, int spacing, uint8_t r, uint8_t g, uint8_t b) {
    for (int i = 0; i < strlen(str); i ++) {
        int width = 0;
        if (font == FONT_5x5_FLEX) {
            width = draw_char_5x5_flex(frame_buf_ptr, str[i], x, y, r, g, b);
        }
        x += width + spacing;
    }
}

// Draws a rectangle in the specified location
// x, y are the top left corner
// thickness is the thickness of the perimeter, -1 for completely filled in
void draw_rect(uint8_t ****frame_buf_ptr, int x, int y, int width, int height, int thickness, uint8_t r, uint8_t g, uint8_t b) {
    for (int i = 0; i < width; i ++) {
        for (int j = 0; j < height; j ++) {
            if (thickness == -1 || i < thickness || i >= width - thickness || j < thickness || j >= height - thickness) {
                draw_pixel(frame_buf_ptr, x + i, y + j, r, g, b);
            }
        }
    }
}

// Draws a straight line in one of the four cardinal directions
void draw_line(uint8_t ****frame_buf_ptr, int x, int y, direction_t direction, int length, uint8_t r, uint8_t g, uint8_t b) {
    switch (direction) {
        case DIRECTION_UP:
            for (int i = 0; i < length; i ++) {
                draw_pixel(frame_buf_ptr, x, y - i, r, g, b);
            }
            break;
        case DIRECTION_DOWN:
            for (int i = 0; i < length; i ++) {
                draw_pixel(frame_buf_ptr, x, y + i, r, g, b);
            }
            break;
        case DIRECTION_LEFT:
            for (int i = 0; i < length; i ++) {
                draw_pixel(frame_buf_ptr, x - i, y, r, g, b);
            }
            break;
        case DIRECTION_RIGHT:
            for (int i = 0; i < length; i ++) {
                draw_pixel(frame_buf_ptr, x + i, y, r, g, b);
            }
            break;
        default:
            break;
    }
}

// Fill the entire display with one color
void fill_display(uint8_t ****frame_buf_ptr, uint8_t r, uint8_t g, uint8_t b) {
    for (int i = 0; i < DISPLAY_WIDTH; i ++) {
        for (int j = 0; j < DISPLAY_HEIGHT; j ++) {
            (*frame_buf_ptr)[j][i][0] = r;
            (*frame_buf_ptr)[j][i][1] = g;
            (*frame_buf_ptr)[j][i][2] = b;
        }
    }
}

// Draw image directly
void draw_img_raw(uint8_t ****frame_buf_ptr, int x, int y, int width, int height, uint8_t img_data[height][width][3]) {
    for (int i = 0; i < width; i ++) {
        for (int j = 0; j < height; j ++) {
            if (img_data[j][i][0] != TRANSPARENT_COLOR[0] && img_data[j][i][1] != TRANSPARENT_COLOR[1] && img_data[j][i][2] != TRANSPARENT_COLOR[2]) {
                draw_pixel(frame_buf_ptr, x + i, y + j, img_data[j][i][0], img_data[j][i][1], img_data[j][i][2]);
            }
        }
    }
}

// Draw image from the included assets
void draw_img(uint8_t ****frame_buf_ptr, image_t img, int x, int y) {
    switch (img) {
        case IMG_WIFI_CONNECTED:
            draw_img_raw(frame_buf_ptr, x, y, WIFI_CONNECTED_WIDTH, WIFI_CONNECTED_HEIGHT, WIFI_CONNECTED_IMG);
            break;
    }
}

// TODO functions:
// draw bmp img (bmp, x, y)
// draw animation frame (animation bmps, time, x, y) -> done?: int (yes/no)