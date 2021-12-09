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

#ifndef FOOLRENDERER_SHADERS_BASIC_H_
#define FOOLRENDERER_SHADERS_BASIC_H_

#include "math/matrix.h"
#include "math/vector.h"
#include "shader_context.h"
#include "texture.h"

struct basic_uniform {
    matrix4x4 mvp;
    vector3 light_direction;
    vector3 ambient_color;
    struct texture *diffuse_texture;
};

struct basic_vertex_attribute {
    vector3 position;
    vector3 normal;
    vector2 texcoord;
};

vector4 basic_vertex_shader(struct shader_context *output, const void *uniform,
                            const void *vertex_attribute);

vector4 basic_fragment_shader(struct shader_context *input,
                              const void *uniform);

#endif  // FOOLRENDERER_SHADERS_BASIC_H_
