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

// The basic shader implements Blinn-Phong reflection model with Phong shading.
// The lighting calculation is performed in the view space. One advantage of
// this is that the camera position is always at (0,0,0), and there is no need
// to provide the camera position to the shader.
// References:
// https://en.wikipedia.org/wiki/Blinn%E2%80%93Phong_reflection_model
// https://en.wikipedia.org/wiki/Phong_shading

struct basic_uniform {
    matrix4x4 modelview;
    matrix4x4 projection;
    // A matrix used to transform the normal direction from local space to view
    // space.
    matrix4x4 normal_matrix;
    // Direction of parallel light in view space.
    vector3 light_direction;
    vector3 light_intensity;
    vector3 ambient_intensity;
    float shininess;
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
