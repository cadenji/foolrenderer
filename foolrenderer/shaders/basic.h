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
// The lighting calculation is performed in the view space. One advantage of
// this is that the camera position is always at (0,0,0), and there is no need
// to provide the camera position to the shader.
// References:
// https://en.wikipedia.org/wiki/Blinn%E2%80%93Phong_reflection_model
// https://en.wikipedia.org/wiki/Phong_shading

struct basic_uniform {
    matrix4x4 modelview;
    matrix4x4 projection;
    matrix3x3 normal_obj2view;

    // Parameters of the light.
    vector3 view_space_light_dir;
    vector3 light_color;
    vector3 ambient_color;

    // Parameters of the material.
    vector3 ambient_reflectance;
    vector3 diffuse_reflectance;
    vector3 specular_reflectance;
    float shininess;
    struct texture *diffuse_map;
    struct texture *normal_map;

    // Parameters used to calculate directional light shadows.
    matrix4x4 normalized_light_space;
    struct texture *shadow_map;
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
