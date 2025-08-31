#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_rom_sys.h"
#include "esp_heap_caps.h"

#include "display_driver.h"

// Board dependent
#define R1 9
#define G1 10
#define B1 11
#define R2 12
#define G2 13
#define B2 14

// Board dependent
#define A 35
#define B 45
#define C 48
#define D 47
#define CLK 21
#define LAT 20
#define OE 19

#define DISPLAY_WIDTH 64 // Horizontal pixel count of display
#define DISPLAY_HEIGHT 32 // Vertical pixel count of display
#define COLOR_DEPTH 4 // Bit depth of each of the 3 color channels
#define SCAN_LINES 2 // 1/16 -> 2 lines are scanned at a time (1/16th of the display is scanned at a time)

#define I2S_SAMPLE_RATE 150000
#define BASE_DELAY_US 200 // Shortest delay used for I2S

// Double frame buffer
uint8_t frame_buf_in[DISPLAY_HEIGHT][DISPLAY_WIDTH][3];
uint8_t frame_buf_out[DISPLAY_HEIGHT][DISPLAY_WIDTH][3];
// Frame buffer pointers
uint8_t (*frame_buf_in_ptr)[DISPLAY_HEIGHT][DISPLAY_WIDTH][3] = &frame_buf_in;
uint8_t (*frame_buf_out_ptr)[DISPLAY_HEIGHT][DISPLAY_WIDTH][3] = &frame_buf_out;
// Temp pointer for swapping frame buffer pointers
uint8_t (*tmp)[DISPLAY_HEIGHT][DISPLAY_WIDTH][3];

// Indicator of whether the in frame buffer is completed
int in_done = 0;
int *in_done_ptr = &in_done;

// Scanning bitplanes used to correctly format row pixel data into the correct format for RGB channel PWM
uint8_t bitplane_buf[DISPLAY_HEIGHT][DISPLAY_WIDTH][COLOR_DEPTH];

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

// Swaps the pointers to the two frame buffers
void swap_frame_buffers() {
    tmp = frame_buf_in_ptr;
    frame_buf_in_ptr = frame_buf_out_ptr;
    frame_buf_out_ptr = tmp;
}

// Prepare the bitplanes for a pair of rows
void prep_bitplanes() {
    for (uint8_t row = 0; row < (uint8_t) (DISPLAY_HEIGHT / SCAN_LINES); row ++) {
        for (int col = 0; col < DISPLAY_WIDTH; col ++) {
            for (int depth = 0; depth < COLOR_DEPTH; depth ++) {
                uint8_t r1 = (*frame_buf_out_ptr)[row][col][0];
                uint8_t g1 = (*frame_buf_out_ptr)[row][col][1];
                uint8_t b1 = (*frame_buf_out_ptr)[row][col][2];
                uint8_t r2 = (*frame_buf_out_ptr)[row + DISPLAY_HEIGHT / SCAN_LINES][col][0];
                uint8_t g2 = (*frame_buf_out_ptr)[row + DISPLAY_HEIGHT / SCAN_LINES][col][1];
                uint8_t b2 = (*frame_buf_out_ptr)[row + DISPLAY_HEIGHT / SCAN_LINES][col][2];
                printf("r1 %d, g1 %d, b1 %d, r2 %d, g2 %d, b2 %d ", r1, g1, b2, r2, g2, b2);
                uint8_t rgb_bit_slice = 0;
                rgb_bit_slice |= ((r1 >> (7 - depth)) & 1) << 7;
                rgb_bit_slice |= ((g1 >> (7 - depth)) & 1) << 6;
                rgb_bit_slice |= ((b1 >> (7 - depth)) & 1) << 5;
                rgb_bit_slice |= ((r2 >> (7 - depth)) & 1) << 4;
                rgb_bit_slice |= ((g2 >> (7 - depth)) & 1) << 3;
                rgb_bit_slice |= ((b2 >> (7 - depth)) & 1) << 2;
                printf("slice %d\n", rgb_bit_slice);
                bitplane_buf[row][col][depth] = rgb_bit_slice;
            }
        }
    }
}

void init_gpio() {
    gpio_config_t gpio_cfg = {
        .pin_bit_mask = ((1ULL << R1) | (1ULL << G1) | (1ULL << B1) | (1ULL << R2) | (1ULL << G2) | (1ULL << B2) | (1ULL << A) | (1ULL << B) | (1ULL << C) | (1ULL << D) | (1ULL << LAT) | (1ULL << OE) | (1ULL << CLK)),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = 0,
        .pull_down_en = 0,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config(&gpio_cfg);
    gpio_set_level(CLK, 0);
    gpio_set_level(LAT, 0);
    gpio_set_level(OE, 0);
    gpio_set_level(R1, 0);
    gpio_set_level(G1, 0);
    gpio_set_level(B1, 0);
    gpio_set_level(R2, 0);
    gpio_set_level(G2, 0);
    gpio_set_level(B2, 0);
}

void refresh_task(void *param) {
    while (1) {
        if (*in_done_ptr) {
            swap_frame_buffers();
            *in_done_ptr = 0;
        }
        // Main render
        // printf("b %d\n", bitplane_buf[0][0][0]);
        prep_bitplanes();
        // printf("c %d\n", bitplane_buf[0][0][0]);
        for (int depth = 0; depth < COLOR_DEPTH; depth ++) {
            for (int i = 0; i < 1 << depth; i ++) {
                for (uint8_t row = 0; row < (uint8_t) (DISPLAY_HEIGHT / SCAN_LINES); row ++) {
                    gpio_set_level(A, (row & 0x01) >> 0);
                    gpio_set_level(B, (row & 0x02) >> 1);
                    gpio_set_level(C, (row & 0x04) >> 2);
                    gpio_set_level(D, (row & 0x08) >> 3);
                    for (uint8_t col = 0; col < DISPLAY_WIDTH; col ++) {
                        
                        gpio_set_level(R1, (bitplane_buf[row][col][depth] & 0x01) >> 0);
                        gpio_set_level(G1, (bitplane_buf[row][col][depth] & 0x02) >> 1);
                        gpio_set_level(B1, (bitplane_buf[row][col][depth] & 0x04) >> 2);
                        gpio_set_level(R2, (bitplane_buf[row][col][depth] & 0x08) >> 3);
                        gpio_set_level(G2, (bitplane_buf[row][col][depth] & 0x10) >> 4);
                        gpio_set_level(B2, (bitplane_buf[row][col][depth] & 0x20) >> 5);
                        // gpio_set_level(R1, 0);
                        // gpio_set_level(G1, 0);
                        // gpio_set_level(B1, 0);
                        // gpio_set_level(R2, 0);
                        // gpio_set_level(G2, 0);
                        // gpio_set_level(B2, 0);
                        gpio_set_level(CLK, 1);
                        gpio_set_level(CLK, 0);
                    }
                    gpio_set_level(LAT, 1);
                    gpio_set_level(LAT, 0);
                }
            }
        }
        // End main render
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

// Call this to start the display refresh task
void run_refresh() {
    init_gpio();

    // for (int row = 0; row < (DISPLAY_HEIGHT / SCAN_LINES);  row ++) {
    //     for (int bit_depth = 0; bit_depth < COLOR_DEPTH; bit_depth ++) {
    //         bitplane_buf[row][bit_depth] = heap_caps_malloc(DISPLAY_WIDTH, MALLOC_CAP_DMA);
    //         memset(bitplane_buf[row][bit_depth], 0, DISPLAY_WIDTH);
    //     }
    // }
    for (int i = 0; i < DISPLAY_HEIGHT; i ++) {
        for (int j = 0; j < DISPLAY_WIDTH; j ++) {
            for (int d = 0; d < COLOR_DEPTH; d ++) {
                bitplane_buf[i][j][d] = 255;
            }
        }
    }
    // printf("a %d\n", bitplane_buf[0][0][0]);

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

    xTaskCreatePinnedToCore(refresh_task, "Refresh Task", 4096, xTaskGetCurrentTaskHandle(), 10, NULL, 1);
}