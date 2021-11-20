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

#ifndef FOOLRENDERER_TEXTURE_H_
#define FOOLRENDERER_TEXTURE_H_

#include <stdbool.h>
#include <stdint.h>

#include "math/vector.h"

///
/// \brief A texture is an object that saves image pixel data in a specific
///        format.
///
/// The texture stores pixels in row-major order, the first pixel is at the
/// bottom-left corner. The texture pixel type is vector4, and the value range
/// of each component is [0, 1].
///
struct texture;

///
/// \brief Create texture objects based on the image data.
///
/// The pixel format of the image must be RGB, and each component is an unsigned
/// byte type. The origin of the image should be in the bottom-left corner.
///
/// Both the image width and height must be greater than 0, and the data cannot
/// be NULL. Otherwise, the creation fails.
///
/// \param width The width of the image.
/// \param height The height of the image.
/// \param data The pointer to the image pixel data.
/// \return Returns the texture object pointer successful, otherwise returns
///         NULL.
///
struct texture *generate_texture(uint32_t width, uint32_t height,
                                 const uint8_t *data);

void delete_texture(struct texture *texture);

bool texture_sample(vector4 *pixel, const struct texture *texture,
                    vector2 texture_coordinate);

#endif  // FOOLRENDERER_TEXTURE_H_
