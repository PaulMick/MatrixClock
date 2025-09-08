#ifndef DISPLAY_UTILS_H
#define DISPLAY_UTILS_H

#include "assets.h"

void draw_pixel(uint8_t ****frame_buf_ptr, int x, int y, uint8_t r, uint8_t g, uint8_t b);
void draw_char_5x5_flex(uint8_t ****frame_buf_ptr, char c, int x, int y, uint8_t r, uint8_t g, uint8_t b);
void draw_str(uint8_t ****frame_buf_ptr, char *str, font_t font, int x, int y, int spacing, uint8_t r, uint8_t g, uint8_t b);
void draw_rect(uint8_t ****frame_buf_ptr, int x, int y, int width, int height, int thickness, uint8_t r, uint8_t g, uint8_t b);
void draw_line(uint8_t ****frame_buf_ptr, int x, int y, direction_t direction, int length, uint8_t r, uint8_t g, uint8_t b);
void fill_display(uint8_t ****frame_buf_ptr, uint8_t r, uint8_t g, uint8_t b);
void draw_img(uint8_t ****frame_buf_ptr, image_t img, int x, int y);
void draw_img_raw(uint8_t ****frame_buf_ptr, uint8_t ***img_data, int x, int y, int width, int height);

#endif