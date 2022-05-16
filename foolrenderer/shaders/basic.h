// Copyright (c) Caden Ji. All rights reserved.
//
// Licensed under the MIT License. See LICENSE.txt in the project root for
// license information.

#ifndef FOOLRENDERER_SHADERS_BASIC_H_
#define FOOLRENDERER_SHADERS_BASIC_H_

#include "math/matrix.h"
#include "math/vector.h"
#include "shader_context.h"
#include "texture.h"

// The basic shader implements Blinn-Phong reflection model with Phong shading.
// References:
// https://en.wikipedia.org/wiki/Blinn%E2%80%93Phong_reflection_model
// https://en.wikipedia.org/wiki/Phong_shading

struct basic_uniform {
    matrix4x4 local2view;
    matrix4x4 view2clip;
    matrix3x3 loacl2view_direction;
    matrix3x3 local2view_normal;
    // In light space, each component of position should be in [0,1].
    matrix4x4 local2light;

    // Parameters of the directional light.
    vector3 light_direction;  // Normalized light direction in view space.
    vector3 light_color;
    vector3 ambient_color;
    struct texture *shadow_map;

    // Parameters of the material.
    vector3 ambient_reflectance;
    vector3 diffuse_reflectance;
    vector3 specular_reflectance;
    float shininess;
    struct texture *diffuse_map;
    struct texture *normal_map;
};

struct basic_vertex_attribute {
    vector3 position;
    vector3 normal;
    vector4 tangent;
    vector2 texcoord;
};

vector4 basic_vertex_shader(struct shader_context *output, const void *uniform,
                            const void *vertex_attribute);

vector4 basic_fragment_shader(struct shader_context *input,
                              const void *uniform);

#endif  // FOOLRENDERER_SHADERS_BASIC_H_
