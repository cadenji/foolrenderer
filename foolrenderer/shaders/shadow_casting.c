// Copyright (c) Caden Ji. All rights reserved.
//
// Licensed under the MIT License. See LICENSE file in the project root for
// license information.

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
    return matrix4x4_multiply_vector4(unif->local2clip, position);
}

vector4 shadow_casting_fragment_shader(struct shader_context *input,
                                       const void *uniform) {
    (void)input;
    (void)uniform;
    return VECTOR4_ZERO;
}
