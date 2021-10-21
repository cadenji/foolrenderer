// Copyright (c) 2021 Caden Ji
//
// MIT License
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <stdint.h>
#include <stdio.h>
#include <tgafunc.h>

#include "graphics.h"

#define IMAGE_WIDTH 512
#define IMAGE_HEIGHT 512

static inline void endian_inversion(uint8_t *bytes, size_t size) {
    uint8_t buffer;
    size_t left = 0, right = size - 1;
    while (left < right) {
        buffer = bytes[left];
        bytes[left] = bytes[right];
        bytes[right] = buffer;
        ++left;
        --right;
    }
}

int main(void) {
    uint8_t *data;
    tga_info *info;
    tga_create(&data, &info, IMAGE_WIDTH, IMAGE_HEIGHT, TGA_PIXEL_RGB24);

    // Draw a red trangle.
    vector3 v[3] = {{{0.5f, -0.5f, 1.0f}},
                    {{-0.5f, 0.5f, 1.0f}},
                    {{-0.5f, -0.5f, 1.0f}}};
    set_viewport(0, 0, IMAGE_WIDTH, IMAGE_HEIGHT);
    draw_triangle(v, data);

    // Convert all pixels to little endian and save as TGA format file.
    uint8_t *pixel;
    for (int y = 0; y < IMAGE_WIDTH; y++) {
        for (int x = 0; x < IMAGE_HEIGHT; x++) {
            pixel = tga_get_pixel(data, info, x, y);
            endian_inversion(pixel, 3);
        }
    }
    tga_save_from_info(data, info, "output.tga");

    tga_free_data(data);
    tga_free_info(info);
    return 0;
}
