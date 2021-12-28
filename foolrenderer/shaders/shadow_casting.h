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

#ifndef FOOLRENDERER_SHADERS_SHADOW_CASTING_H_
#define FOOLRENDERER_SHADERS_SHADOW_CASTING_H_

#include "math/matrix.h"
#include "math/vector.h"
#include "shader_context.h"

// The shadow casting shader is used to render shadow maps. For shadow mapping
// algorithm, refer to:
// https://en.wikipedia.org/wiki/Shadow_mapping

struct shadow_casting_uniform {
    matrix4x4 light_space_matrix;
};

struct shadow_casting_vertex_attribute {
    vector3 position;
};

vector4 shadow_casting_vertex_shader(struct shader_context *output,
                                     const void *uniform,
                                     const void *vertex_attribute);

vector4 shadow_casting_fragment_shader(struct shader_context *input,
                                       const void *uniform);

#endif  // FOOLRENDERER_SHADERS_SHADOW_CASTING_H_
