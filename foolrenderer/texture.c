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
#include <stdint.h>
#include <stdlib.h>

#include "math/math_utility.h"
#include "math/vector.h"

struct texture {
    enum texture_format format;
    uint32_t width, height;
    void *pixels;
};

static size_t get_pixel_size(enum texture_format format) {
    size_t pixel_size;
    switch (format) {
        case TEXTURE_FORMAT_RGBA8:
            pixel_size = 4;
            break;
        case TEXTURE_FORMAT_DEPTH_FLOAT:
            pixel_size = sizeof(float);
            break;
        default:
            pixel_size = 0;
            break;
    }
    return pixel_size;
}

struct texture *generate_texture(enum texture_format internal_format,
                                 uint32_t width, uint32_t height) {
    if (width == 0 || height == 0) {
        return NULL;
    }
    size_t pixel_size = get_pixel_size(internal_format);
    if (pixel_size == 0) {
        return NULL;
    }
    struct texture *texture = malloc(sizeof(struct texture));
    if (texture == NULL) {
        return NULL;
    }
    size_t pixel_count = (size_t)width * height;
    texture->pixels = malloc(pixel_count * pixel_size);
    if (texture->pixels == NULL) {
        free(texture);
        return NULL;
    }

    texture->format = internal_format;
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

bool set_texture_pixels(struct texture *texture, const void *pixels) {
    if (texture == NULL || pixels == NULL) {
        return false;
    }
    if (texture->format == TEXTURE_FORMAT_RGBA8) {
        size_t pixel_count = (size_t)texture->width * texture->height;
        for (size_t p = 0; p < pixel_count; p++) {
            const uint8_t *image_pixel = (uint8_t *)pixels + p * 3;
            uint8_t *texture_pixel = (uint8_t *)texture->pixels + p * 4;
            texture_pixel[0] = image_pixel[0];
            texture_pixel[1] = image_pixel[1];
            texture_pixel[2] = image_pixel[2];
            texture_pixel[3] = 0xFF;
        }
        return true;
    }
    return false;
}

void *get_texture_pixels(struct texture *texture) {
    if (texture == NULL) {
        return NULL;
    }
    return texture->pixels;
}

enum texture_format get_texture_format(const struct texture *texture) {
    // Do not check whether the texture points to a null pointer, because there
    // is no simple way to return an error.
    return texture->format;
}

uint32_t get_texture_width(const struct texture *texture) {
    // Do not check whether the texture points to a null pointer, because there
    // is no simple way to return an error.
    return texture->width;
}

uint32_t get_texture_height(const struct texture *texture) {
    // Do not check whether the texture points to a null pointer, because there
    // is no simple way to return an error.
    return texture->height;
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
    size_t pixel_offset = (size_t)u_index + v_index * texture->width;

    if (texture->format == TEXTURE_FORMAT_RGBA8) {
        const uint8_t *target = (uint8_t *)texture->pixels + pixel_offset * 4;
        pixel->r = target[0] / 255.0f;
        pixel->g = target[1] / 255.0f;
        pixel->b = target[2] / 255.0f;
        pixel->a = target[3] / 255.0f;
        return true;
    } else if (texture->format == TEXTURE_FORMAT_DEPTH_FLOAT) {
        const float *target = (float *)texture->pixels + pixel_offset;
        pixel->r = *target;
        pixel->g = *target;
        pixel->b = *target;
        pixel->a = 1.0f;
        return true;
    }
    return false;
}
