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

#ifndef FOOLRENDERER_GRAPHICS_H_
#define FOOLRENDERER_GRAPHICS_H_

#include <stdint.h>

#include "math/vector.h"
#include "shader_context.h"
#include "texture.h"

///
/// \brief Pointer to vertex shader.
///
/// The uniform is constant that can be accessed in vertex shader and fragment
/// shader. It usually used to pass transformation matrices, light directions,
/// and material properties to the shader.
///
/// The vertex attribute represents the data elements of a vertex that the
/// vertex shader needs to process，such as vertex positions, normal and texture
/// coordinates.
///
/// The vertex shader returns the clipping space position of the vertex, the
/// clipping space should follow the OpenGL convention, using the left-handed
/// coordinate system, the near plane is at z=-1, and the far plane is at z=1.
///
/// Any other output produced needs to be saved in the shader context. These
/// output values will be interpolated across the face of the rendered
/// triangles, and the value of each pixel will be passed as input to the
/// fragment shader.
///
typedef vector4 (*vertex_shader)(struct shader_context *output,
                                 const void *uniform,
                                 const void *vertex_attribute);

///
/// \brief Pointer to fragment shader.
///
/// The uniform is constant that can be accessed in vertex shader and fragment
/// shader. It usually used to pass transformation matrices, light directions,
/// and material properties to the shader.
///
/// The input stores the values assigned by the vertex shader, which are
/// interpolated before being received by the fragment shader.
///
/// The fragment shader returns the color value.
///
typedef vector4 (*fragment_shader)(struct shader_context *input,
                                   const void *uniform);

///
/// \brief A framebuffer is a collection of buffers that can be used as the
///        destination for rendering.
///
/// The origin (0, 0) of all buffers is in the bottom-left corner.
///
struct framebuffer {
    uint32_t width, height;
    uint8_t *color_buffer;
    float *depth_buffer;
};

///
/// \brief Creates a framebuffer of the specified dimension.
///
/// \param width The framebuffer width in pixel.
/// \param height The framebuffer height in pixel.
/// \return Returns the framebuffer pointer if the generation is successful,
///         otherwise returns NULL.
///
struct framebuffer *generate_framebuffer(uint32_t width, uint32_t height);

///
/// \brief Uses the default value to clear all the buffers in the framebuffer at
///        one time.
///
/// The default value for clearing the color buffer is 0x0, and the default
/// value for clearing the depth buffer is 1.0f.
///
/// \param framebuffer The framebuffer to be cleared.
///
void clear_framebuffer(struct framebuffer *framebuffer);

///
/// \brief Deletes the framebuffer.
///
/// \param framebuffer The framebuffer to be deleted.
///
void delete_framebuffer(struct framebuffer *framebuffer);

///
/// \brief Set the viewport parameters.
///
/// Viewport describe a view port by its bottom-left coordinate, width and
/// height in pixels.
///
/// \param left Left coordinate in pixel.
/// \param bottom Bottom coordinate in pixel.
/// \param width Width in pixel.
/// \param height Height in pixel.
///
void set_viewport(int left, int bottom, uint32_t width, uint32_t height);

void set_vertex_shader(vertex_shader shader);

void set_fragment_shader(fragment_shader shader);

///
/// \brief Render triangle.
///
/// Before calling this function, need to ensure that the rendering state has
/// been set through the set_viewport(), set_vertex_shader() and
/// set_fragment_shader() functions.
///
/// When the triangle finally appears on the screen after all transformations,
/// if the vertex connection sequence is counterclockwise, the triangle is
/// treated as front face. This function only draws front-facing triangles.
///
/// \param framebuffer Buffer for saving rendering results.
/// \param uniform Contains constants that can be accessed in the vertex shader
///                and fragment shader.
/// \param vertex_attributes An array containing vertex attributes, with a
///                          length of 3.
///
void draw_triangle(struct framebuffer *framebuffer, const void *uniform,
                   const void *const vertex_attributes[]);

#endif  // FOOLRENDERER_GRAPHICS_H_
