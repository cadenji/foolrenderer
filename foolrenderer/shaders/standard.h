// Copyright (c) Caden Ji. All rights reserved.
//
// Licensed under the MIT License. See LICENSE file in the project root for
// license information.

#ifndef FOOLRENDERER_SHADERS_STANDARD_H_
#define FOOLRENDERER_SHADERS_STANDARD_H_

#include "graphics/shader_context.h"
#include "graphics/texture.h"
#include "math/matrix.h"
#include "math/vector.h"

// The standard shader implements a physically based rendering (PBR) material
// model. And use the specific implementation of the Google Filament project:
// https://google.github.io/filament/Filament.html
//
// This model is composed of a diffuse term and a specular term. Can be used to
// render common opaque metallic/non-metallic objects.

struct standard_uniform {
    matrix4x4 local2world;
    matrix4x4 world2clip;
    matrix3x3 local2world_direction;
    matrix3x3 local2world_normal;
    // Camera position in world space.
    vector3 camera_position;
    // Normalized directional light direction in world space.
    vector3 light_direction;
    // Directional light illuminance.
    vector3 illuminance;
    // Suppose the ambient lighting is uniform from all directions.
    vector3 ambient_luminance;

    ////////////////////////////////////////////////////////////////////////////
    //
    // Material parameters.
    //
    ////////////////////////////////////////////////////////////////////////////
    struct texture *normal_map;
    // Diffuse albedo for non-metallic surfaces and specular color for metallic
    // surfaces, should be in linear color space. A specular color reference
    // table for metals can be found in the Filament documentation:
    // https://google.github.io/filament/Filament.html#table_fnormalmetals
    vector3 base_color;
    struct texture *base_color_map;
    // Whether a surface appears to be dielectric (0.0) or conductor (1.0).
    float metallic;
    struct texture *metallic_map;
    // Perceived smoothness (0.0) or roughness (1.0) of a surface.
    float roughness;
    struct texture *roughness_map;
    // Fresnel reflectance at normal incidence for dielectric surfaces, not
    // useful for conductor surfaces. A reference table of reflectance for
    // dielectric can be found in the Filament documentation:
    // https://google.github.io/filament/Filament.html#table_commonmatreflectance
    float reflectance;
};

struct standard_vertex_attribute {
    vector3 position;
    vector3 normal;
    vector4 tangent;
    vector2 texcoord;
};

vector4 standard_vertex_shader(struct shader_context *output,
                               const void *uniform,
                               const void *vertex_attribute);

vector4 standard_fragment_shader(struct shader_context *input,
                                 const void *uniform);

#endif  // FOOLRENDERER_SHADERS_STANDARD_H_
