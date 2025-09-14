#ifndef DISPLAY_UTILS_H
#define DISPLAY_UTILS_H

#include "display_assets.h"
#include "modes_assets.h"

void draw_pixel(uint8_t ****frame_buf_ptr, int x, int y, uint8_t r, uint8_t g, uint8_t b);
void draw_char_5x5_flex(uint8_t ****frame_buf_ptr, char c, int x, int y, uint8_t r, uint8_t g, uint8_t b);
void draw_str(uint8_t ****frame_buf_ptr, char *str, font_t font, int x, int y, int spacing, uint8_t r, uint8_t g, uint8_t b);
void draw_rect(uint8_t ****frame_buf_ptr, int x, int y, int width, int height, int thickness, uint8_t r, uint8_t g, uint8_t b);
void draw_line(uint8_t ****frame_buf_ptr, int x, int y, direction_t direction, int length, uint8_t r, uint8_t g, uint8_t b);
void fill_display(uint8_t ****frame_buf_ptr, uint8_t r, uint8_t g, uint8_t b);
void draw_img(uint8_t ****frame_buf_ptr, image_t img, int x, int y);
void draw_img_raw(uint8_t ****frame_buf_ptr, int x, int y, int width, int height, uint8_t img_data[height][width][3]);

void draw_menu(uint8_t ****frame_buf_ptr, modestate_menu_t state);
void draw_clock(uint8_t ****frame_buf_ptr, modestate_clock_t state);
void draw_clockweather(uint8_t ****frame_buf_ptr, modestate_clockweather_t state);
void draw_weather(uint8_t ****frame_buf_ptr, modestate_weather_t state);

#endif