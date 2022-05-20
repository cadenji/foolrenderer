// Copyright (c) Caden Ji. All rights reserved.
//
// Licensed under the MIT License. See LICENSE file in the project root for
// license information.

#include "texture.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "color.h"
#include "math/math_utility.h"
#include "math/vector.h"

#define FALLBACK_PIXEL VECTOR4_ONE

struct texture {
    enum texture_format format;
    uint32_t width, height;
    void *pixels;
};

static size_t get_pixel_size(enum texture_format format) {
    size_t pixel_size;
    switch (format) {
        case TEXTURE_FORMAT_RGBA8:
        case TEXTURE_FORMAT_SRGB8_A8:
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

struct texture *create_texture(enum texture_format internal_format,
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

void destroy_texture(struct texture *texture) {
    if (texture != NULL) {
        free(texture->pixels);
        free(texture);
    }
}

bool set_texture_pixels(struct texture *texture, const void *pixels) {
    if (texture == NULL || pixels == NULL) {
        return false;
    }
    if (texture->format == TEXTURE_FORMAT_RGBA8 ||
        texture->format == TEXTURE_FORMAT_SRGB8_A8) {
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
    return texture->format;
}

uint32_t get_texture_width(const struct texture *texture) {
    return texture->width;
}

uint32_t get_texture_height(const struct texture *texture) {
    return texture->height;
}

vector4 texture_sample(const struct texture *texture, vector2 texcoord) {
    if (texture == NULL) {
        return FALLBACK_PIXEL;
    }

    float u = float_clamp01(texcoord.u);
    float v = float_clamp01(texcoord.v);
    uint32_t u_index = (uint32_t)(u * texture->width);
    uint32_t v_index = (uint32_t)(v * texture->height);
    // Prevent array access out of bounds.
    u_index = u_index >= texture->width ? texture->width - 1 : u_index;
    v_index = v_index >= texture->height ? texture->height - 1 : v_index;
    size_t pixel_offset = (size_t)u_index + v_index * texture->width;

    vector4 pixel = FALLBACK_PIXEL;
    if (texture->format == TEXTURE_FORMAT_RGBA8) {
        const uint8_t *target = (uint8_t *)texture->pixels + pixel_offset * 4;
        pixel.r = uint8_to_float(target[0]);
        pixel.g = uint8_to_float(target[1]);
        pixel.b = uint8_to_float(target[2]);
        pixel.a = uint8_to_float(target[3]);
    } else if (texture->format == TEXTURE_FORMAT_SRGB8_A8) {
        const uint8_t *target = (uint8_t *)texture->pixels + pixel_offset * 4;
        pixel.r = convert_to_linear_color(uint8_to_float(target[0]));
        pixel.g = convert_to_linear_color(uint8_to_float(target[1]));
        pixel.b = convert_to_linear_color(uint8_to_float(target[2]));
        pixel.a = uint8_to_float(target[3]);
    } else if (texture->format == TEXTURE_FORMAT_DEPTH_FLOAT) {
        const float *target = (float *)texture->pixels + pixel_offset;
        pixel.r = *target;
        pixel.g = *target;
        pixel.b = *target;
        pixel.a = 1.0f;
    }
    return pixel;
}
