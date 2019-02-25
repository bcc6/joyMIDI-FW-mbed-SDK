#ifndef SSD1306_CONFIG_H
#define SSD1306_CONFIG_H

#include "mbed.h"

#define RAW_WIDTH 128
#define RAW_HEIGHT 32
#define FLIP_VERTICAL 1

#define FRAME_BUFFER_SIZE 512           // 128 * 32 / 8
#define TRANSFER_BUFFER_SIZE (7 + 128)  // Addressing bytes + Page buffer
#endif