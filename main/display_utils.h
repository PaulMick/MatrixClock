#ifndef DISPLAY_UTILS_H
#define DISPLAY_UTILS_H

void draw_pixel(uint8_t ****frame_buf_ptr, int x, int y, uint8_t r, uint8_t g, uint8_t b);
void draw_char(uint8_t ****frame_buf_ptr, char c, int x, int y, uint8_t r, uint8_t g, uint8_t b);
void draw_str(uint8_t ****frame_buf_ptr, char *str, int x, int y, int spacing, uint8_t r, uint8_t g, uint8_t b);
void draw_line(uint8_t ****frame_buf_ptr, int x, int y, enum direction direction, uint8_t r, uint8_t g, uint8_t b);
void fill_display(uint8_t ****frame_buf_ptr, uint8_t r, uint8_t g, uint8_t b);

int load_font();

#endif