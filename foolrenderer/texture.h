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
    /// The components included in this format are R, G, B, A, and each
    /// component is an 8-bit unsigned integer type. And the color values of the
    /// three components of R, G, and B are considered to be encoded in the sRGB
    /// color space.
    ///
    TEXTURE_FORMAT_SRGB8_A8,
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
/// \brief Creates a texture.
///
/// Returns a null pointer if the width or height value is equal to 0. Returns a
/// null pointer if the internal_format parameter is an invalid value. Returns a
/// null pointer if memory allocation fails.
///
/// \param internal_format The pixel format used internally by the texture.
/// \param width The width of the texture.
/// \param height The height of the texture.
/// \return Returns a texture pointer on success, null pointer on failure.
///
struct texture *generate_texture(enum texture_format internal_format,
                                 uint32_t width, uint32_t height);

///
/// \brief Destroys the texture created by generate_texture().
///
/// If texture is a null pointer, the function does nothing.
///
/// \param texture Pointer to the texture to destroy.
///
void delete_texture(struct texture *texture);

///
/// \brief Writes pixel data to the texture.
///
/// This function is only applicable to textures whose internal format is
/// TEXTURE_FORMAT_RGBA8. The data pointed to by pixels must be in RGB format,
/// and each component is an unsigned byte type. The origin of the image should
/// be in the bottom-left corner.
///
/// If texture or pixels is a null pointer, the data write fails. The behavior
/// is undefined if the size of the array pointed to by the pixel is smaller
/// than the data size required by the texture.
///
/// \param texture The texture pointer.
/// \param pixels Pointer to the pixel data source.
/// \return Returns true on success, false on failure.
///
bool set_texture_pixels(struct texture *texture, const void *pixels);

///
/// \brief Gets pixel data in the texture.
///
/// If texture is a null pointer, returns a null pointer.
///
/// \param texture Pointer to the texture to get.
/// \return Returns a pixel data pointer on success, null pointer on failure.
///
void *get_texture_pixels(struct texture *texture);

///
/// \brief Gets the texture format of the texture.
///
/// The behavior is undefined if texture is a null pointer.
///
/// \param texture Pointer to the texture to get.
/// \return Returns texture format.
///
enum texture_format get_texture_format(const struct texture *texture);

///
/// \brief Gets the width of the texture.
///
/// The behavior is undefined if texture is a null pointer.
///
/// \param texture Pointer to the texture to get.
/// \return Returns texture width.
///
uint32_t get_texture_width(const struct texture *texture);

///
/// \brief Gets the height of the texture.
///
/// The behavior is undefined if texture is a null pointer.
///
/// \param texture Pointer to the texture to get.
/// \return Returns texture height.
///
uint32_t get_texture_height(const struct texture *texture);

///
/// \brief Retrieves pixel from the texture.
///
/// If the texture's format is sRGB encoded, the function will inverse-correct
/// pixel values to linear color space.
///
/// If texture is a null pointer, returns a fallback pixel value (1,1,1,1).
///
/// \param texture Pointer to the texture to retrieve.
/// \param texcoord Texture coordinate at which the texture will be sampled.
/// \return Returns pixel on success. Returns fallback pixel on failure.
///
vector4 texture_sample(const struct texture *texture, vector2 texcoord);

#endif  // FOOLRENDERER_TEXTURE_H_
