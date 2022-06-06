// Copyright (c) Caden Ji. All rights reserved.
//
// Licensed under the MIT License. See LICENSE file in the project root for
// license information.

#include "utilities/image.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <tgafunc.h>

#include "graphics/texture.h"

// Convert TGA image pixels to texture's pixels or texture's pixels to TGA image
// pixels. Each component of the pixel must be an 8-bit unsigned integer type,
// and the number of components of pixels must be greater than or equal to 3.
static inline void pixel_endian_inversion(uint8_t *pixel) {
    // Swap the values of the 1st and 3rd components of a pixel.
    uint8_t buffer = pixel[0];
    pixel[0] = pixel[2];
    pixel[2] = buffer;
}

// Modify the components of the TGA image pixels to the order expected by the
// texture. Only TGA images with pixel formats TGA_PIXEL_RGB24 and
// TGA_PIXEL_ARGB32 are supported.
static void modify_tga_image_pixel(uint8_t *image_data, tga_info *image_info) {
    enum tga_pixel_format pixel_format = tga_get_pixel_format(image_info);
    if (pixel_format != TGA_PIXEL_RGB24 && pixel_format != TGA_PIXEL_ARGB32) {
        return;
    }
    uint32_t width = tga_get_image_width(image_info);
    uint32_t height = tga_get_image_height(image_info);
    for (uint32_t y = 0; y < width; y++) {
        for (uint32_t x = 0; x < height; x++) {
            uint8_t *pixel = tga_get_pixel(image_data, image_info, x, y);
            pixel_endian_inversion(pixel);
        }
    }
}

struct texture *load_image(const char *filename, bool is_srgb_encoding) {
    if (filename == NULL || strlen(filename) == 0) {
        return NULL;
    }
    uint8_t *image_data;
    tga_info *image_info;
    enum tga_error error_code;
    error_code = tga_load(&image_data, &image_info, filename);
    if (error_code != TGA_NO_ERROR) {
        return NULL;
    }
    enum tga_pixel_format image_pixel_format = tga_get_pixel_format(image_info);
    uint32_t width = tga_get_image_width(image_info);
    uint32_t height = tga_get_image_height(image_info);
    // The coordinate system used by the loaded image data and the coordinate
    // system used by the texture are opposite on the Y axis. So need to flip
    // the image in the Y-axis direction.
    tga_image_flip_v(image_data, image_info);

    struct texture *texture = NULL;
    if (image_pixel_format == TGA_PIXEL_BW8) {
        texture = create_texture(TEXTURE_FORMAT_R8, width, height);
        if (texture != NULL) {
            set_texture_pixels(texture, image_data);
        }
    } else if (image_pixel_format == TGA_PIXEL_RGB24) {
        modify_tga_image_pixel(image_data, image_info);
        enum texture_format texture_format =
            is_srgb_encoding ? TEXTURE_FORMAT_SRGB8 : TEXTURE_FORMAT_RGB8;
        texture = create_texture(texture_format, width, height);
        if (texture != NULL) {
            set_texture_pixels(texture, image_data);
        }
    } else if (image_pixel_format == TGA_PIXEL_ARGB32) {
        modify_tga_image_pixel(image_data, image_info);
        enum texture_format texture_format =
            is_srgb_encoding ? TEXTURE_FORMAT_SRGB8_A8 : TEXTURE_FORMAT_RGBA8;
        texture = create_texture(texture_format, width, height);
        if (texture != NULL) {
            set_texture_pixels(texture, image_data);
        }
    }

    tga_free_data(image_data);
    tga_free_info(image_info);
    return texture;
}

bool save_image(struct texture *texture, const char *filename) {
    enum texture_format texture_format = get_texture_format(texture);
    if (texture_format != TEXTURE_FORMAT_SRGB8 &&
        texture_format != TEXTURE_FORMAT_RGB8 &&
        texture_format != TEXTURE_FORMAT_SRGB8_A8 &&
        texture_format != TEXTURE_FORMAT_RGBA8) {
        return false;
    }

    uint32_t texture_width = get_texture_width(texture);
    uint32_t texture_height = get_texture_height(texture);
    const uint8_t *texture_data = get_texture_pixels(texture);

    uint8_t *image_data;
    tga_info *image_info;
    tga_create(&image_data, &image_info, texture_width, texture_height,
               TGA_PIXEL_RGB24);

    // Copy the color buffer data to the TGA image.
    for (uint32_t y = 0; y < texture_width; y++) {
        for (uint32_t x = 0; x < texture_height; x++) {
            const uint8_t *texture_pixel =
                texture_data + (y * texture_width + x) * 4;
            uint8_t *image_pixel = tga_get_pixel(image_data, image_info, x, y);
            image_pixel[0] = texture_pixel[0];
            image_pixel[1] = texture_pixel[1];
            image_pixel[2] = texture_pixel[2];
            // Convert the pixel components to the desired arrangement order of
            // the TGA image.
            pixel_endian_inversion(image_pixel);
        }
    }
    // For the same reason as inverting the image data vertically in the
    // load_image() function.
    tga_image_flip_v(image_data, image_info);
    tga_save_from_info(image_data, image_info, filename);

    tga_free_data(image_data);
    tga_free_info(image_info);
    return true;
}
