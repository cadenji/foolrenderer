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

///
/// \brief Draws the triangle to the framebuffer.
///
/// The vertices of the triangle in normalized device coordinates (NDC). And
/// triangles defined with counterclockwise winding are treated as front-facing
/// triangles. This function only draws front-facing triangles.
///
/// The NDC uses a left-handed coordinate system, and point (-1, -1) is located
/// at the the bottom-left corner (Y up).
///
/// The origin (0, 0) of the framebuffer is in the upper-left corner (Y down).
///
/// \param vertices The vertices of the triangle in NDC space.
/// \param colors The vertex colors of the triangle.
/// \param framebuffer The framebuffer which the triangle is drawn.
///
void draw_triangle(const vector3 vertices[], const vector3 colors[],
                   uint8_t *framebuffer);

#endif  // FOOLRENDERER_GRAPHICS_H_
