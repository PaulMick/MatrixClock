#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define DISPLAY_WIDTH 64
#define DISPLAY_HEIGHT 32

#define FONT_FILE "5x5_flex.font"

const char *months_full[12] = {"JANUARY", "FEBRUARY", "MARCH", "APRIL", "MAY", "JUNE", "JULY", "AUGUST", "SEPTEMBER", "OCTOBER", "NOVEMBER", "DECEMBER"};
const char *months_short[12] = {"JAN", "FEB", "MAR", "APR", "MAY", "JUN", "JUL", "AUG", "SEP", "OCT", "NOV", "DEC"};

const char *week_days_full[7] = {"MONDAY", "TUESDAY", "WEDNESDAY", "THURSDAY", "FRIDAY", "SATURDAY", "SUNDAY"};
const char *week_days_short[7] = {"MON", "TUE", "WED", "THU", "FRI", "SAT", "SUN"};

const uint32_t font[128];

enum direction {
    DIRECTION_UP,
    DIRECTION_DOWN,
    DIRECTION_LEFT,
    DIRECTION_RIGHT
};

// Safe function for drawing a single pixel, checks if pixel coordinates are on the display before writing
void draw_pixel(uint8_t (*frame_buf)[DISPLAY_HEIGHT][DISPLAY_WIDTH][3], int x, int y, uint8_t r, uint8_t g, uint8_t b) {
    if (x >= 0 && x < DISPLAY_WIDTH && y >= 0 && y < DISPLAY_HEIGHT) {
        (*frame_buf)[y][x][0] = r;
        (*frame_buf)[y][x][1] = g;
        (*frame_buf)[y][x][2] = b;
    }
}

// Draw a single ASCII character
// x, y are the top-left corner
int draw_char(uint8_t (*frame_buf)[DISPLAY_HEIGHT][DISPLAY_WIDTH][3], char c, int x, int y, uint8_t r, uint8_t g, uint8_t b) {
    uint32_t font_char = font[(int) c];
    int width = font_char >> 25;
    for (int i = 0; i < width; i ++) {
        for (int j = 0; j < 5; j ++) {
            draw_pixel(frame_buf, x + i, y + j, r, g, b);
        }
    }
    return width;
}

// Draw a string of ASCII characters
// x, y are the top-let corner
void draw_str(uint8_t (*frame_buf)[DISPLAY_HEIGHT][DISPLAY_WIDTH][3], char *str, int x, int y, int spacing, uint8_t r, uint8_t g, uint8_t b) {
    for (int i = 0; i < strlen(str); i ++) {
        int width = draw_char(frame_buf, str[i], x, y, r, g, b);
        x += width + spacing;
    }
}

// Draws a rectangle in the specified location
// x, y are the top left corner
// thickness is the thickness of the perimeter, -1 for completely filled in
void draw_rect(uint8_t (*frame_buf)[DISPLAY_HEIGHT][DISPLAY_WIDTH][3], int x, int y, int width, int height, int thickness, uint8_t r, uint8_t g, uint8_t b) {
    for (int i = 0; i < width; i ++) {
        for (int j = 0; j < height; j ++) {
            if (thickness == -1 || i < thickness || i > width - thickness || j < thickness || j > height - thickness) {
                draw_pixel(frame_buf, x + i, y + j, r, g, b);
            }
        }
    }
}

// Draws a straight line in one of the four cardinal directions
void draw_line(uint8_t (*frame_buf)[DISPLAY_HEIGHT][DISPLAY_WIDTH][3], int x, int y, enum direction direction, int length, uint8_t r, uint8_t g, uint8_t b) {
    switch (direction) {
        case DIRECTION_UP:
            for (int i = 0; i < length; i ++) {
                draw_pixel(frame_buf, x, y - i, r, g, b);
            }
            break;
        case DIRECTION_DOWN:
            for (int i = 0; i < length; i ++) {
                draw_pixel(frame_buf, x, y + i, r, g, b);
            }
            break;
        case DIRECTION_LEFT:
            for (int i = 0; i < length; i ++) {
                draw_pixel(frame_buf, x - i, y, r, g, b);
            }
            break;
        case DIRECTION_RIGHT:
            for (int i = 0; i < length; i ++) {
                draw_pixel(frame_buf, x + i, y, r, g, b);
            }
            break;
        default:
            break;
    }
}

// Fill the entire display with one color
void fill_dislpay(uint8_t (*frame_buf)[DISPLAY_HEIGHT][DISPLAY_WIDTH][3], uint8_t r, uint8_t g, uint8_t b) {
    for (int i = 0; i < DISPLAY_WIDTH; i ++) {
        for (int j = 0; j < DISPLAY_HEIGHT; j ++) {
            (*frame_buf)[j][i][0] = r;
            (*frame_buf)[j][i][1] = g;
            (*frame_buf)[j][i][2] = b;
        }
    }
}

// TODO functions:
// draw bmp img (bmp, x, y)
// draw animation frame (animation bmps, time, x, y) -> done?: int (yes/no)

int load_font() {
    FILE * fp;
    fp = fopen(FONT_FILE, "r");
    if (fp == NULL) {
        return 0;
    }
    size_t bytes_read = fread(font, 4, 128, fp);
    if (bytes_read != 128) {
        return 0;
    }
    return 1;
}