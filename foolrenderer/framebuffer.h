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

#ifndef FOOLRENDERER_FRAMEBUFFER_H_
#define FOOLRENDERER_FRAMEBUFFER_H_

#include <stdbool.h>
#include <stdint.h>

#include "texture.h"

enum attachment_type { COLOR_ATTACHMENT, DEPTH_ATTACHMENT };

///
/// \brief A framebuffer is a collection of buffers that can be used as the
///        destination for rendering.
///
struct framebuffer;

///
/// \brief Creates a framebuffer.
///
/// The created framebuffer does not contain any buffer. It can only be used
/// after attaching at least one buffer to the framebuffer using the
/// attach_texture_to_framebuffer() function.
///
/// \return Returns a pointer to the new framebuffer if the generation is
///         successful, otherwise returns a null pointer.
///
struct framebuffer *generate_framebuffer(void);

///
/// \brief Release the memory pointed to by framebuffer.
///
/// The buffer attached to the framebuffer will not be released.
///
/// \param framebuffer The target framebuffer.
///
void delete_framebuffer(struct framebuffer *framebuffer);

///
/// \brief Attaches a texture as one of the logical buffers of the framebuffer.
///
/// If the attached texture size is inconsistent, the width and height of the
/// framebuffer will use the minimum of all texture sizes respectively.
///
/// Different attachment type correspond to specific valid texture types:
///
/// Attachment Type  | Texture Format
/// ---------------- | -----------------------------
/// COLOR_ATTACHMENT | TEXTURE_FORMAT_RGBA8
/// DEPTH_ATTACHMENT | TEXTURE_FORMAT_DEPTH_FLOAT
///
/// \param framebuffer The target framebuffer.
/// \param attachment The type of buffer will to be attached.
/// \param texture The texture to attach. If it is a null pointer detach the
///                texture.
/// \return Returns true if the attachment is successful. If the framebuffer
///         points to a null pointer or the attachment type is invalid or the
///         attached texture type is invalid, false is returned.
///
bool attach_texture_to_framebuffer(struct framebuffer *framebuffer,
                                   enum attachment_type attachment,
                                   struct texture *texture);

///
/// \brief Uses default values to clear all buffers in the framebuffer.
///
/// The default value for clearing the color buffer is 0x0, and the default
/// value for clearing the depth buffer is 1.0f.
///
/// \param framebuffer The target framebuffer, do nothing if it is a null
///                    pointer.
///
void clear_framebuffer(struct framebuffer *framebuffer);

///
/// \brief Gets the width of the framebuffer.
///
/// \param framebuffer The target framebuffer.
/// \return The width of the framebuffer. If framebuffer is a null pointer or
///         framebuffer does not contain a buffer, returns 0.
///
uint32_t get_framebuffer_width(const struct framebuffer *framebuffer);

///
/// \brief Gets the height of the framebuffer.
///
/// \param framebuffer The target framebuffer.
/// \return The height of the framebuffer. If framebuffer is a null pointer or
///         framebuffer does not contain a buffer, returns 0.
///
uint32_t get_framebuffer_height(const struct framebuffer *framebuffer);

///
/// \brief Gets the buffer of the specified attachment type from the
///        framebuffer.
///
/// \param framebuffer The target framebuffer.
/// \param attachment The attachment type of the framebuffer.
/// \return Returns the pointer of the buffer. If the framebuffer points to a
///         null pointer, returns a null pointer. If the attachment type is
///         invalid or there is no buffer of the specified attachment type
///         attached to the framebuffer, returns a null pointer.
///
struct texture *get_framebuffer_attachment(struct framebuffer *framebuffer,
                                           enum attachment_type attachment);

#endif  // FOOLRENDERER_FRAMEBUFFER_H_
