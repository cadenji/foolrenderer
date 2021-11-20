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

#include "texture.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#include "math/math_utility.h"
#include "math/vector.h"

struct texture {
    uint32_t width, height;
    vector4 *pixels;
};

struct texture *generate_texture(uint32_t width, uint32_t height,
                                 const uint8_t *data) {
    if (width == 0 || height == 0 || data == NULL) {
        return NULL;
    }
    struct texture *texture;
    texture = (struct texture *)malloc(sizeof(struct texture));
    if (texture == NULL) {
        return NULL;
    }
    size_t pixel_count = (size_t)width * height;
    texture->pixels = (vector4 *)malloc(pixel_count * sizeof(vector4));
    if (texture->pixels == NULL) {
        free(texture);
        return NULL;
    }

    // Calculate floating-point type pixel data based on image data.
    for (size_t p = 0; p < pixel_count; p++) {
        const uint8_t *image_pixel = data + p;
        vector4 *texture_pixel = texture->pixels + p;
        texture_pixel->r = image_pixel[0] / (float)255.0f;
        texture_pixel->g = image_pixel[1] / (float)255.0f;
        texture_pixel->b = image_pixel[2] / (float)255.0f;
        texture_pixel->a = 1.0f;
    }
    texture->width = width;
    texture->height = height;
    return texture;
}

void delete_texture(struct texture *texture) {
    if (texture != NULL) {
        free(texture->pixels);
        free(texture);
    }
}

bool texture_sample(vector4 *pixel, const struct texture *texture,
                    vector2 texture_coordinate) {
    if (texture == NULL) {
        return false;
    }
    float u = clamp01_float(texture_coordinate.u);
    float v = clamp01_float(texture_coordinate.v);
    uint32_t u_index = (uint32_t)(u * texture->width);
    uint32_t v_index = (uint32_t)(v * texture->height);
    // Prevent array access out of bounds.
    u_index = u_index >= texture->width ? texture->width - 1 : u_index;
    v_index = v_index >= texture->height ? texture->height - 1 : v_index;
    size_t pixel_offset = v_index * texture->width + u_index;
    vector4 *target = texture->pixels + pixel_offset;
    pixel->r = target->r;
    pixel->g = target->g;
    pixel->b = target->b;
    pixel->a = target->a;
    return true;
}
