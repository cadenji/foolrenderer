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

#include "shaders/shadow_casting.h"

#include "math/matrix.h"
#include "math/vector.h"
#include "shader_context.h"

vector4 shadow_casting_vertex_shader(struct shader_context *output,
                                     const void *uniform,
                                     const void *vertex_attribute) {
    (void)output;
    const struct shadow_casting_uniform *unif = uniform;
    const struct shadow_casting_vertex_attribute *attr = vertex_attribute;

    vector4 position = vector3_to_4(attr->position, 1.0f);
    return matrix4x4_multiply_vector4(unif->light_space_matrix, position);
}

vector4 shadow_casting_fragment_shader(struct shader_context *input,
                                       const void *uniform) {
    (void)input;
    (void)uniform;
    return VECTOR4_ZERO;
}
