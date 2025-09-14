#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "display_assets.h"
#include "modes_assets.h"

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
        case IMG_AM:
            draw_img_raw(frame_buf_ptr, x, y, AM_WIDTH, AM_HEIGHT, AM_IMG);
            break;
        case IMG_PM:
            draw_img_raw(frame_buf_ptr, x, y, PM_WIDTH, PM_HEIGHT, PM_IMG);
            break;
    }
}

// Draw menu mode
void draw_menu(uint8_t ****frame_buf_ptr, modestate_menu_t state) {

}

// Draw clock mode
void draw_clock(uint8_t ****frame_buf_ptr, modestate_clock_t state) {
    

}

// Draw clockweather mode
void draw_clockweather(uint8_t ****frame_buf_ptr, modestate_clockweather_t state) {
    // Clear background
    fill_display(frame_buf_ptr, 0, 0, 0);

    // Date
    draw_str(frame_buf_ptr, WEEK_DAYS_SHORT[state.week_day], FONT_5x5_FLEX, 1, 1, 1, 255, 255, 255);
    char date_str[6];
    sprintf(date_str, "%02d/%02d", state.month, state.month_day);
    draw_str(frame_buf_ptr, date_str, FONT_5x5_FLEX, 16, 1, 1, 255, 255, 255);

    // Temperature
    char temp_str[6];
    sprintf(temp_str, "%03d*F", state.temp_f);
    if (state.temp_f >= TEMP_HIGH_THRESHOLD_F) {
        draw_str(frame_buf_ptr, temp_str, FONT_5x5_FLEX, 36, 1, 1, 255, 100, 100);
    } else if (state.temp_f <= TEMP_LOW_THRESHOLD_F) {
        draw_str(frame_buf_ptr, temp_str, FONT_5x5_FLEX, 36, 1, 1, 100, 100, 255);
    } else {
        draw_str(frame_buf_ptr, temp_str, FONT_5x5_FLEX, 36, 1, 1, 255, 255, 255);
    }

    // Weather Code

    // High and low temps
    char high_str[4];
    char low_str[4];
    sprintf(high_str, "%03d", state.temp_high_f);
    sprintf(low_str, "%03d", state.temp_low_f);
    draw_str(frame_buf_ptr, high_str, FONT_5x5_FLEX, 52, 7, 1, 255, 255, 255);
    draw_str(frame_buf_ptr, low_str, FONT_5x5_FLEX, 52, 13, 1, 255, 255, 255);
    draw_line(frame_buf_ptr, 49, 7, DIRECTION_DOWN, 5, 255, 100, 100);
    draw_line(frame_buf_ptr, 48, 8, DIRECTION_RIGHT, 3, 255, 100, 100);
    draw_line(frame_buf_ptr, 49, 13, DIRECTION_DOWN, 5, 100, 100, 255);
    draw_line(frame_buf_ptr, 48, 16, DIRECTION_RIGHT, 3, 100, 100, 255);

    // AM/PM
    printf("hour %d\n", state.hour);
    if (state.hour >= 12) {
        draw_img(frame_buf_ptr, IMG_PM, 49, 20);
    } else {
        draw_img(frame_buf_ptr, IMG_AM, 49, 20);
    }

    // Time
    draw_rect(frame_buf_ptr, 19, 13, 3, 3, -1, 255, 255, 255);
    draw_rect(frame_buf_ptr, 19, 22, 3, 3, -1, 255, 255, 255);
    int hour = state.hour % 12;
    if (hour == 0) {
        hour = 12;
    }
    if (hour >= 10) {
        draw_rect(frame_buf_ptr, 2, 8, 3, 22, -1, 255, 255, 255);
    }
    int hour_right = hour % 10;
    switch (hour_right) {
        case 0:
            draw_rect(frame_buf_ptr, 7, 8, 10, 22, 3, 255, 255, 255);
            break;
        case 1:
            draw_rect(frame_buf_ptr, 14, 8, 3, 22, -1, 255, 255, 255);
            break;
        case 2:
            draw_rect(frame_buf_ptr, 7, 8, 10, 3, -1, 255, 255, 255);
            draw_rect(frame_buf_ptr, 14, 8, 3, 12, -1, 255, 255, 255);
            draw_rect(frame_buf_ptr, 7, 17, 10, 3, -1, 255, 255, 255);
            draw_rect(frame_buf_ptr, 7, 17, 3, 13, -1, 255, 255, 255);
            draw_rect(frame_buf_ptr, 7, 27, 10, 3, -1, 255, 255, 255);
            break;
        case 3:
            break;
        case 4:
            break;
        case 5:
            break;
        case 6:
            break;
        case 7:
            break;
        case 8:
            break;
        case 9:
            break;
    }
}

// Draw weather mode
void draw_weather(uint8_t ****frame_buf_ptr, modestate_weather_t state) {

}

// TODO functions:
// draw animation frame (animation bmps, time, x, y) -> done?: int (yes/no)