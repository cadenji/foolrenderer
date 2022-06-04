// Copyright (c) Caden Ji. All rights reserved.
//
// Licensed under the MIT License. See LICENSE file in the project root for
// license information.

#include "shaders/basic.h"

#include <math.h>
#include <stddef.h>

#include "graphics/shader_context.h"
#include "graphics/texture.h"
#include "math/math_utility.h"
#include "math/matrix.h"
#include "math/vector.h"

#define TEXCOORD 0
#define VIEW_SPACE_POSITION 0
#define LIGHT_SPACE_POSITION 1
#define VIEW_SPACE_NORMAL 2
#define VIEW_SPACE_TANGENT 3
#define VIEW_SPACE_BITANGENT 4

static float shadow_calculation(const struct texture *shadow_map,
                                const vector3 light_space_positon) {
    float visibility = 1.0f;
    if (shadow_map != NULL) {
        float current_depth = light_space_positon.z;
        float bias = 0.005f;  // Slove shadow acne.
        float closest_depth =
            texture_sample(shadow_map, vector3_to_2(light_space_positon)).r;
        visibility = current_depth - bias > closest_depth ? 0.1f : 1.0f;
    }
    return visibility;
}

vector4 basic_vertex_shader(struct shader_context *output, const void *uniform,
                            const void *vertex_attribute) {
    const struct basic_uniform *unif = uniform;
    const struct basic_vertex_attribute *attr = vertex_attribute;

    vector2 *out_texcoord = shader_context_vector2(output, TEXCOORD);
    *out_texcoord = attr->texcoord;

    vector4 view_space_position = matrix4x4_multiply_vector4(
        unif->local2view, vector3_to_4(attr->position, 1.0f));
    vector3 *out_position = shader_context_vector3(output, VIEW_SPACE_POSITION);
    *out_position = vector4_to_3(view_space_position);

    vector3 *out_light_space_position =
        shader_context_vector3(output, LIGHT_SPACE_POSITION);
    vector4 light_space_position = matrix4x4_multiply_vector4(
        unif->local2light, vector3_to_4(attr->position, 1.0f));
    // When calculating directional light shadows, the view2clip matrix
    // contained in local2light is an orthogonal matrix, the w component is
    // always equal to 1.0f, so perspective division is not required.
    *out_light_space_position = vector4_to_3(light_space_position);

    // Calculate t,b,n vectors.
    vector3 *out_normal = shader_context_vector3(output, VIEW_SPACE_NORMAL);
    *out_normal =
        matrix3x3_multiply_vector3(unif->local2view_normal, attr->normal);

    vector3 *out_tangent = shader_context_vector3(output, VIEW_SPACE_TANGENT);
    *out_tangent = matrix3x3_multiply_vector3(unif->loacl2view_direction,
                                              vector4_to_3(attr->tangent));

    vector3 *out_bitangent =
        shader_context_vector3(output, VIEW_SPACE_BITANGENT);
    *out_bitangent = vector3_multiply_scalar(
        vector3_cross(*out_normal, *out_tangent), attr->tangent.w);

    return matrix4x4_multiply_vector4(unif->view2clip, view_space_position);
}

vector4 basic_fragment_shader(struct shader_context *input,
                              const void *uniform) {
    const struct basic_uniform *unif = uniform;
    vector2 texcoord = *shader_context_vector2(input, TEXCOORD);

    // Get the normal in tangent space.
    vector3 normal = vector4_to_3(texture_sample(unif->normal_map, texcoord));
    normal =
        vector3_subtract_scalar(vector3_multiply_scalar(normal, 2.0f), 1.0f);
    // Transform the normal from tangent space to view space
    vector3 t =
        vector3_normalize(*shader_context_vector3(input, VIEW_SPACE_TANGENT));
    vector3 b =
        vector3_normalize(*shader_context_vector3(input, VIEW_SPACE_BITANGENT));
    vector3 n =
        vector3_normalize(*shader_context_vector3(input, VIEW_SPACE_NORMAL));
    normal = matrix3x3_multiply_vector3(matrix3x3_construct(t, b, n), normal);

    // Ambient lighting.
    vector3 ambient_lighting =
        vector3_multiply(unif->ambient_color, unif->ambient_reflectance);

    // Diffuse lighting.
    float n_dot_l = vector3_dot(normal, unif->light_direction);
    float diffuse_intensity = float_max(0.0f, n_dot_l);
    vector3 diffuse_lighting =
        vector3_multiply_scalar(unif->light_color, diffuse_intensity);
    diffuse_lighting =
        vector3_multiply(diffuse_lighting, unif->diffuse_reflectance);

    // Specular lighting.
    vector3 specular_lighting = VECTOR3_ZERO;
    if (n_dot_l > 0.0f) {
        // Because in view space,the camera position is always at (0,0,0), so
        // the calculation of the view direction is simplified.
        vector3 postion = *shader_context_vector3(input, VIEW_SPACE_POSITION);
        vector3 view_direction = vector3_multiply_scalar(postion, -1.0f);
        view_direction = vector3_normalize(view_direction);
        // Calculate the halfway vector between the light direction and the view
        // direction.
        vector3 halfway = vector3_add(view_direction, unif->light_direction);
        halfway = vector3_normalize(halfway);
        float n_dot_h = vector3_dot(normal, halfway);
        float specular_intensity =
            powf(float_max(0.0f, n_dot_h), unif->shininess);
        specular_lighting =
            vector3_multiply_scalar(unif->light_color, specular_intensity);
        specular_lighting =
            vector3_multiply(specular_lighting, unif->specular_reflectance);
    }

    // Add shadow.
    vector3 light_space_position =
        *shader_context_vector3(input, LIGHT_SPACE_POSITION);
    float visibility =
        shadow_calculation(unif->shadow_map, light_space_position);
    diffuse_lighting = vector3_multiply_scalar(diffuse_lighting, visibility);
    specular_lighting = vector3_multiply_scalar(specular_lighting, visibility);

    vector4 texture_color = texture_sample(unif->diffuse_map, texcoord);
    vector3 fragment_color = vector3_add(ambient_lighting, diffuse_lighting);
    fragment_color =
        vector3_multiply(fragment_color, vector4_to_3(texture_color));
    fragment_color = vector3_add(fragment_color, specular_lighting);
    return vector3_to_4(fragment_color, 1.0f);
}
