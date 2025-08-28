#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2s_std.h"
#include "driver/gpio.h"
#include "esp_rom_sys.h"
#include "esp_heap_caps.h"

#include "display_driver.h"

// Board dependent
#define R1 25
#define G1 26
#define B1 27
#define R2 14
#define G2 13
#define B2 12

// Board dependent
#define A 23
#define B 22
#define C 5
#define D 17
#define LAT 4
#define OE 15
#define CLK 16

#define DISPLAY_WIDTH 64 // Horizontal pixel count of display
#define DISPLAY_HEIGHT 32 // Vertical pixel count of display
#define COLOR_DEPTH 4 // Bit depth of each of the 3 color channels
#define SCAN_LINES 2 // 1/16 -> 2 lines are scanned at a time (1/16th of the display is scanned at a time)

#define I2S_SAMPLE_RATE 2000000
#define BASE_DELAY_US 200 // Shortest delay used for I2S

// Double frame buffer
uint8_t frame_buf_in[DISPLAY_HEIGHT][DISPLAY_WIDTH][3];
uint8_t frame_buf_out[DISPLAY_HEIGHT][DISPLAY_WIDTH][3];
// Frame buffer pointers
uint8_t (*frame_buf_in_ptr)[DISPLAY_HEIGHT][DISPLAY_WIDTH][3] = &frame_buf_in;
uint8_t (*frame_buf_out_ptr)[DISPLAY_HEIGHT][DISPLAY_WIDTH][3] = &frame_buf_out;
// Temp pointer for swapping frame buffer pointers
uint8_t (*tmp)[DISPLAY_HEIGHT][DISPLAY_WIDTH][3];
int swap = 0;

// Indicator of whether the in frame buffer is completed
int in_done = 0;
int *in_done_ptr = &in_done;

// Scanning bitplanes used to correctly format row pixel data into the correct format for RGB channel PWM
uint8_t *bitplane_buf[COLOR_DEPTH];

// Get a display handle
DisplayHandle get_display_handle() {
    DisplayHandle display_handle = {
        .width = DISPLAY_WIDTH,
        .height = DISPLAY_HEIGHT,
        .frame_buf_ptr = frame_buf_in_ptr,
        .in_done_ptr = in_done_ptr
    };
    return display_handle;
}

void init_gpio() {
    gpio_config_t gpio_cfg = {
        .pin_bit_mask = (1ULL << A) | (1ULL << B) | (1ULL << C) | (1ULL << D) | (1ULL << LAT) | (1ULL << OE) | (1ULL << CLK),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = 0,
        .pull_down_en = 0,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config(&gpio_cfg);
    gpio_set_level(LAT, 0);
    gpio_set_level(OE, 1);
}

void init_i2s() {
    i2s_chan_config_t chan_cfg = I2S_CHANNEL_DEFAULT_CONFIG(I2S_NUM_0, I2S_ROLE_MASTER);
    chan_cfg.dma_desc_num = 64;
    chan_cfg.dma_frame_num = 64;
    chan_cfg.auto_clear = true;
    i2s_chan_handle_t tx_chan;
    i2s_new_channel(&chan_cfg, &tx_chan, NULL);
    i2s_std_config_t std_cfg = {
        .clk_cfg = I2S_STD_CLK_DEFAULT_CONFIG(I2S_SAMPLE_RATE),
        .slot_cfg = I2S_STD_PHILIPS_SLOT_DEFAULT_CONFIG(I2S_DATA_BIT_WIDTH_8BIT, I2S_SLOT_MODE_MONO),
        .gpio_cfg = {
            .mclk = I2S_GPIO_UNUSED,
            .bclk = CLK,
            .ws = I2S_GPIO_UNUSED,
            .dout = {R1, G1, B1, R2, G2, B2, -1, -1},
            .din = I2S_GPIO_UNUSED
        }
    };
    i2s_channel_init_std_mode(tx_chan, &std_cfg);
}

// TODO: Might not be needed???
void set_row(uint8_t row) {
    gpio_set_level(A, row & 0x01);
    gpio_set_level(B, row & 0x02);
    gpio_set_level(C, row & 0x04);
    gpio_set_level(D, row & 0x08);
}

// Swaps the pointers to the two frame buffers
void swap_frame_buffers() {
    tmp = frame_buf_in_ptr;
    frame_buf_in_ptr = frame_buf_out_ptr;
    frame_buf_out_ptr = tmp;
}

// Prepare the bitplanes for a pair of rows
void prep_bitplanes(uint8_t row) {
    // Add color channels in by bit depth
    for (int bit_depth = 0; bit_depth < COLOR_DEPTH; bit_depth ++) {
        // Add the bit at the current bit depth from every pixel in the row to the bitplane
        for (int col = 0; col < DISPLAY_WIDTH; col ++) {
            uint8_t r1 = *frame_buf_out_ptr[row][col][0];
            uint8_t g1 = *frame_buf_out_ptr[row][col][1];
            uint8_t b1 = *frame_buf_out_ptr[row][col][2];
            uint8_t r2 = *frame_buf_out_ptr[row + DISPLAY_HEIGHT / SCAN_LINES][col][0];
            uint8_t g2 = *frame_buf_out_ptr[row + DISPLAY_HEIGHT / SCAN_LINES][col][1];
            uint8_t b2 = *frame_buf_out_ptr[row + DISPLAY_HEIGHT / SCAN_LINES][col][2];
            uint8_t rgb_bit_slice = 0;
            rgb_bit_slice |= ((r1 >> (7 - bit_depth)) & 1) << 0;
            rgb_bit_slice |= ((g1 >> (7 - bit_depth)) & 1) << 1;
            rgb_bit_slice |= ((b1 >> (7 - bit_depth)) & 1) << 2;
            rgb_bit_slice |= ((r2 >> (7 - bit_depth)) & 1) << 3;
            rgb_bit_slice |= ((g2 >> (7 - bit_depth)) & 1) << 4;
            rgb_bit_slice |= ((b2 >> (7 - bit_depth)) & 1) << 5;
            bitplane_buf[bit_depth][col] = rgb_bit_slice;
        }
    }
}

// Render a row to the display
void render_row(uint8_t row) {
    set_row(row);
    prep_bitplanes(row);
    for (int bit_depth = 0; bit_depth < COLOR_DEPTH; bit_depth ++) {
        size_t bytes_written;
        gpio_set_level(OE, 1);
        i2s_channel_write(I2S_NUM_0, bitplane_buf[bit_depth], DISPLAY_WIDTH, &bytes_written, portMAX_DELAY);
        gpio_set_level(LAT, 1);
        gpio_set_level(LAT, 0);
        gpio_set_level(OE, 0);
        esp_rom_delay_us(BASE_DELAY_US * (1 << bit_depth));
    }
}

void refresh_task(void *param) {
    while (1) {
        if (*in_done_ptr) {
            swap_frame_buffers();
            *in_done_ptr = 0;
        }
        for (uint8_t row = 0; row < (uint8_t) (DISPLAY_HEIGHT / SCAN_LINES); row ++) {
            render_row(row);
        }
    }
}

void run_refresh() {
    init_gpio();
    init_i2s();

    for (int bit_depth = 0; bit_depth < COLOR_DEPTH; bit_depth ++) {
        bitplane_buf[bit_depth] = heap_caps_malloc(DISPLAY_WIDTH, MALLOC_CAP_DMA);
        memset(bitplane_buf[bit_depth], 0, DISPLAY_WIDTH);
    }

    for (int y = 0; y < DISPLAY_HEIGHT; y ++) {
        for (int x = 0; x < DISPLAY_WIDTH; x ++) {
            frame_buf_in[y][x][0] = x;
            frame_buf_in[y][x][1] = y;
            frame_buf_in[y][x][2] = x + y;
        }
    }

    for (int y = 0; y < DISPLAY_HEIGHT; y ++) {
        for (int x = 0; x < DISPLAY_WIDTH; x ++) {
            frame_buf_out[y][x][0] = x;
            frame_buf_out[y][x][1] = y;
            frame_buf_out[y][x][2] = x + y;
        }
    }

    xTaskCreatePinnedToCore(refresh_task, "Refresh Task", 4096, NULL, 1, NULL, 0);
}