// Copyright (c) Caden Ji. All rights reserved.
//
// Licensed under the MIT License. See LICENSE file in the project root for
// license information.

#include "graphics/texture.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "graphics/color.h"
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
        case TEXTURE_FORMAT_R8:
            pixel_size = 1;
            break;
        case TEXTURE_FORMAT_RGB8:
        case TEXTURE_FORMAT_SRGB8:
            pixel_size = 3;
            break;
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

static inline bool is_srgb_encoding(enum texture_format format) {
    if (format == TEXTURE_FORMAT_SRGB8 || format == TEXTURE_FORMAT_SRGB8_A8) {
        return true;
    }
    return false;
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
    size_t pixel_size = get_pixel_size(texture->format);
    if (pixel_size == 0) {
        return false;
    }
    size_t pixel_count = (size_t)texture->width * texture->height;
    memcpy(texture->pixels, pixels, pixel_size * pixel_count);
    return true;
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
    float u = float_clamp01(texcoord.u);
    float v = float_clamp01(texcoord.v);
    uint32_t u_index = (uint32_t)(u * texture->width);
    uint32_t v_index = (uint32_t)(v * texture->height);
    // Prevent array access out of bounds.
    u_index = u_index >= texture->width ? texture->width - 1 : u_index;
    v_index = v_index >= texture->height ? texture->height - 1 : v_index;
    size_t pixel_offset = (size_t)u_index + v_index * texture->width;

    vector4 pixel = VECTOR4_ONE;
    enum texture_format format = texture->format;
    if (format == TEXTURE_FORMAT_DEPTH_FLOAT) {
        const float *target = (float *)texture->pixels + pixel_offset;
        pixel.r = *target;
        pixel.g = *target;
        pixel.b = *target;
    } else if (format == TEXTURE_FORMAT_R8) {
        const uint8_t *target = (uint8_t *)texture->pixels + pixel_offset;
        pixel.r = uint8_to_float(target[0]);
        pixel.g = pixel.r;
        pixel.b = pixel.r;
    } else {
        size_t pixel_size = get_pixel_size(format);
        if (pixel_size == 0) {
            pixel = VECTOR4_ZERO;
        } else {
            // format == TEXTURE_FORMAT_RGB8 ||
            // format == TEXTURE_FORMAT_SRGB8 ||
            // format == TEXTURE_FORMAT_RGBA8 ||
            // format == TEXTURE_FORMAT_SRGB8_A8)
            const uint8_t *target =
                (uint8_t *)texture->pixels + pixel_offset * pixel_size;
            for (size_t i = 0; i < pixel_size; i++) {
                pixel.elements[i] = uint8_to_float(target[i]);
            }
            if (is_srgb_encoding(format)) {
                pixel.r = convert_to_linear_color(pixel.r);
                pixel.g = convert_to_linear_color(pixel.g);
                pixel.b = convert_to_linear_color(pixel.b);
            }
        }
    }
    return pixel;
}
