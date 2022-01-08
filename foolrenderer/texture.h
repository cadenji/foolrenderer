// Copyright (c) Caden Ji. All rights reserved.
//
// Licensed under the MIT License. See LICENSE.txt in the project root for
// license information.

#ifndef FOOLRENDERER_TEXTURE_H_
#define FOOLRENDERER_TEXTURE_H_

#include <stdbool.h>
#include <stdint.h>

#include "math/vector.h"

enum texture_format {
    ///
    /// The components included in this format are R, G, B, A, and each
    /// component is an 8-bit unsigned integer type.
    ///
    TEXTURE_FORMAT_RGBA8,
    ///
    /// The format used to store depth information, the type is float.
    ///
    TEXTURE_FORMAT_DEPTH_FLOAT
};

///
/// \brief A texture is an object that saves image pixel data in a specific
///        format.
///
/// The first pixel corresponds to the bottom-left corner of the texture image.
///
struct texture;

///
/// \brief Creates a texture object.
///
/// \param internal_format The pixel format used internally by the texture.
/// \param width The width of the texture, the value must be greater than 0.
/// \param height The height of the texture, the value must be greater than 0.
/// \return Returns the texture pointer if successful, otherwise returns NULL.
///
struct texture *generate_texture(enum texture_format internal_format,
                                 uint32_t width, uint32_t height);

void delete_texture(struct texture *texture);

///
/// \brief Writes pixel data to the texture.
///
/// This function is only applicable to textures whose internal format is
/// TEXTURE_FORMAT_RGBA8. The data pointed to by pixels must be in RGB format,
/// and each component is an unsigned byte type. The origin of the image should
/// be in the bottom-left corner.
///
/// \param texture The texture pointer.
/// \param pixels Pointer to pixel data source.
/// \return Returns true if the data is written successfully.
///
bool set_texture_pixels(struct texture *texture, const void *pixels);

void *get_texture_pixels(struct texture *texture);

enum texture_format get_texture_format(const struct texture *texture);

uint32_t get_texture_width(const struct texture *texture);

uint32_t get_texture_height(const struct texture *texture);

bool texture_sample(vector4 *pixel, const struct texture *texture,
                    vector2 texture_coordinate);

#endif  // FOOLRENDERER_TEXTURE_H_
