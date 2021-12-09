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

#include "shaders/basic.h"

#include "math/math_utility.h"
#include "math/matrix.h"
#include "math/vector.h"
#include "shader_context.h"
#include "texture.h"

#define TEXCOORD 0
#define COLOR 0

vector4 basic_vertex_shader(struct shader_context *output, const void *uniform,
                            const void *vertex_attribute) {
    const struct basic_uniform *unif = uniform;
    const struct basic_vertex_attribute *attr = vertex_attribute;

    float intensity = vector3_dot(attr->normal, unif->light_direction);
    vector3 color = vector3_add_scalar(unif->ambient_color, intensity);
    color.x = clamp01_float(color.x);
    color.y = clamp01_float(color.y);
    color.z = clamp01_float(color.z);

    vector2 *out_texcoord = shader_context_vector2(output, TEXCOORD);
    *out_texcoord = attr->texcoord;

    vector3 *out_color = shader_context_vector3(output, COLOR);
    *out_color = color;

    return matrix4x4_multiply_vector4(unif->mvp,
                                      vector3_to_4(attr->position, 1.0f));
}

vector4 basic_fragment_shader(struct shader_context *input,
                              const void *uniform) {
    const struct basic_uniform *unif = uniform;

    vector2 *texcoord = shader_context_vector2(input, TEXCOORD);
    vector3 *in_color = shader_context_vector3(input, COLOR);

    vector4 texture_color = VECTOR4_ONE;
    texture_sample(&texture_color, unif->diffuse_texture, *texcoord);

    vector3 color = vector3_multiply(vector4_to_3(texture_color), *in_color);
    return vector3_to_4(color, 1.0f);
}
