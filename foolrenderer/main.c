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
#include "math/math_utility.h"
#include "math/matrix.h"
#include "math/vector.h"

#define IMAGE_WIDTH 512
#define IMAGE_HEIGHT 512

static void endian_inversion(uint8_t *bytes, size_t size) {
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

    vector3 vertices[3] = {{{0.5f, -0.5f, 1.0f}},
                           {{-0.5f, 0.5f, 1.0f}},
                           {{-0.5f, -0.5f, 1.0f}}};
    vector3 colors[3] = {{{1.0f, 0.0f, 0.0f}},
                         {{0.0f, 1.0f, 0.0f}},
                         {{0.0f, 0.0f, 1.0f}}};
    // Transform vertices: scale to (1, 0.5, 1), then rotate PI/2 (90 degrees)
    // counterclockwise, then move 0.25 units to the left.
    matrix4x4 mat_scaling = matrix4x4_scale((vector3){{1, 0.5f, 1}});
    matrix4x4 mat_rotation = matrix4x4_rotate_z(HALF_PI);
    matrix4x4 mat_translation = matrix4x4_translate((vector3){{0.25f, 0, 0}});
    matrix4x4 mat_model = matrix4x4_multiply(mat_rotation, mat_scaling);
    mat_model = matrix4x4_multiply(mat_translation, mat_model);
    for (int i = 0; i < 3; i++) {
        vector4 vertex = vector3_to_4(vertices[i], 1);
        vertex = matrix4x4_multiply_vector4(mat_model, vertex);
        vertices[i] = vecotr4_to_3(vertex);
    }

    set_viewport(0, 0, IMAGE_WIDTH, IMAGE_HEIGHT);
    draw_triangle(vertices, colors, data);

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
