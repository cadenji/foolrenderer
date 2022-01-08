// Copyright (c) Caden Ji. All rights reserved.
//
// Licensed under the MIT License. See LICENSE.txt in the project root for
// license information.

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
