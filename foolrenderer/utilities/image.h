// Copyright (c) Caden Ji. All rights reserved.
//
// Licensed under the MIT License. See LICENSE file in the project root for
// license information.

#ifndef FOOLRENDERER_UTILITIES_IMAGE_H_
#define FOOLRENDERER_UTILITIES_IMAGE_H_

#include <stdbool.h>

#include "graphics/texture.h"

///
/// \brief Loads image data from a TGA format file.
///
/// The corresponding format texture object will be created according to the
/// pixel format of the current image. Only supports TGA images in the format
/// TGA_PIXEL_BW8, TGA_PIXEL_RGB24 and TGA_PIXEL_ARGB32.
///
/// \param filename The TGA file to load.
/// \param is_srgb_encoding Whether the pixel value is sRGB encoded.
/// \return Returns a texture pointer on success, null pointer on failure.
///
struct texture *load_image(const char *filename, bool is_srgb_encoding);

///
/// \brief Saves the texture as a TGA format file.
///
/// Only supports textures in the format TEXTURE_FORMAT_RGB8,
/// TEXTURE_FORMAT_SRGB8, TEXTURE_FORMAT_SRGB8_A8 and TEXTURE_FORMAT_RGBA8.
///
/// \param texture Pointer to the texture to save.
/// \param filename The file to save to.
/// \return Returns true on success, false on failure.
///
bool save_image(struct texture *texture, const char *filename);

#endif  // FOOLRENDERER_UTILITIES_IMAGE_H_
