// Copyright (c) Caden Ji. All rights reserved.
//
// Licensed under the MIT License. See LICENSE file in the project root for
// license information.

#include "graphics/framebuffer.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "graphics/texture.h"
#include "math/math_utility.h"

struct framebuffer {
    uint32_t width, height;
    struct texture *color_buffer;
    struct texture *depth_buffer;
};

struct framebuffer *create_framebuffer(void) {
    struct framebuffer *framebuffer;
    framebuffer = malloc(sizeof(struct framebuffer));
    if (framebuffer == NULL) {
        return NULL;
    }
    framebuffer->width = 0;
    framebuffer->height = 0;
    framebuffer->color_buffer = NULL;
    framebuffer->depth_buffer = NULL;
    return framebuffer;
}

void destroy_framebuffer(struct framebuffer *framebuffer) { free(framebuffer); }

#define SET_MIN_SIZE(buffer)                                                   \
    do {                                                                       \
        if (buffer != NULL) {                                                  \
            uint32_t buffer_width = get_texture_width(buffer);                 \
            uint32_t buffer_height = get_texture_height(buffer);               \
            framebuffer->width = uint32_min(framebuffer->width, buffer_width); \
            framebuffer->height =                                              \
                uint32_min(framebuffer->height, buffer_height);                \
        }                                                                      \
    } while (0)

bool attach_texture_to_framebuffer(struct framebuffer *framebuffer,
                                   enum attachment_type attachment,
                                   struct texture *texture) {
    if (framebuffer == NULL) {
        return false;
    }
    bool result = false;
    if (texture != NULL) {
        enum texture_format format = get_texture_format(texture);
        switch (attachment) {
            case COLOR_ATTACHMENT:
                if (format == TEXTURE_FORMAT_RGBA8 ||
                    format == TEXTURE_FORMAT_SRGB8_A8) {
                    framebuffer->color_buffer = texture;
                    result = true;
                }
                break;
            case DEPTH_ATTACHMENT:
                if (format == TEXTURE_FORMAT_DEPTH_FLOAT) {
                    framebuffer->depth_buffer = texture;
                    result = true;
                }
                break;
        }
    } else {
        switch (attachment) {
            case COLOR_ATTACHMENT:
                framebuffer->color_buffer = NULL;
                result = true;
                break;
            case DEPTH_ATTACHMENT:
                framebuffer->depth_buffer = NULL;
                result = true;
                break;
        }
    }
    // Update the framebuffer size.
    if (result) {
        if (framebuffer->color_buffer == NULL &&
            framebuffer->depth_buffer == NULL) {
            framebuffer->width = 0;
            framebuffer->height = 0;
        } else {
            framebuffer->width = UINT32_MAX;
            framebuffer->height = UINT32_MAX;
            SET_MIN_SIZE(framebuffer->color_buffer);
            SET_MIN_SIZE(framebuffer->depth_buffer);
        }
    }
    return result;
}

void clear_framebuffer(struct framebuffer *framebuffer) {
    if (framebuffer == NULL) {
        return;
    }
    struct texture *buffer;
    size_t pixel_count = (size_t)framebuffer->width * framebuffer->height;
    // Clear color buffer.
    buffer = framebuffer->color_buffer;
    if (buffer != NULL) {
        void *pixels = get_texture_pixels(buffer);
        memset(pixels, 0x0, pixel_count * 4);
    }
    // Clear depth buffer.
    buffer = framebuffer->depth_buffer;
    if (buffer != NULL) {
        float *pixels = get_texture_pixels(buffer);
        for (size_t i = 0; i < pixel_count; i++) {
            pixels[i] = 1.0f;
        }
    }
}

uint32_t get_framebuffer_width(const struct framebuffer *framebuffer) {
    return framebuffer->width;
}

uint32_t get_framebuffer_height(const struct framebuffer *framebuffer) {
    return framebuffer->height;
}

struct texture *get_framebuffer_attachment(struct framebuffer *framebuffer,
                                           enum attachment_type attachment) {
    if (framebuffer == NULL) {
        return NULL;
    }
    switch (attachment) {
        case COLOR_ATTACHMENT:
            return framebuffer->color_buffer;
        case DEPTH_ATTACHMENT:
            return framebuffer->depth_buffer;
        default:
            return NULL;
    }
}
