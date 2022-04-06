// Copyright (c) Caden Ji. All rights reserved.
//
// Licensed under the MIT License. See LICENSE.txt in the project root for
// license information.

#include "shaders/basic.h"

#include <math.h>
#include <stddef.h>

#include "math/math_utility.h"
#include "math/matrix.h"
#include "math/vector.h"
#include "shader_context.h"
#include "texture.h"

#define TEXCOORD 0
#define NORMAL 0
#define POSITION 1
#define LIGHT_SPACE_POSITION 2

static float shadow_calculation(const struct texture *shadow_map,
                                const vector3 *light_space_positon) {
    float visibility = 1.0f;
    if (shadow_map != NULL) {
        float current_depth = light_space_positon->z;

        vector2 shadow_map_coord;
        shadow_map_coord.u = light_space_positon->x;
        shadow_map_coord.v = light_space_positon->y;
        vector4 temp;
        texture_sample(&temp, shadow_map, shadow_map_coord);
        float closest_depth = temp.r;
        float bias = 0.005f;  // Slove shadow acne.
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

    vector3 *out_normal = shader_context_vector3(output, NORMAL);
    *out_normal =
        matrix3x3_multiply_vector3(unif->normal_obj2view, attr->normal);

    vector4 position_in_view = matrix4x4_multiply_vector4(
        unif->modelview, vector3_to_4(attr->position, 1.0f));
    vector3 *out_position = shader_context_vector3(output, POSITION);
    *out_position = vector4_to_3(position_in_view);

    vector3 *out_light_space_position =
        shader_context_vector3(output, LIGHT_SPACE_POSITION);
    vector4 light_space_position =
        matrix4x4_multiply_vector4(unif->normalized_light_space,
                                   vector3_to_4(attr->position, 1.0f));
    // When calculating directional light shadows, the projection matrix
    // contained in normalized_light_space is an orthogonal matrix, the w
    // component is always equal to 1.0f, so perspective division is not
    // required.
    *out_light_space_position = vector4_to_3(light_space_position);

    return matrix4x4_multiply_vector4(unif->projection, position_in_view);
}

vector4 basic_fragment_shader(struct shader_context *input,
                              const void *uniform) {
    const struct basic_uniform *unif = uniform;

    vector3 *in_normal = shader_context_vector3(input, NORMAL);
    vector3 normal = vector3_normalize(*in_normal);

    // Ambient lighting.
    vector3 ambient_lighting =
        vector3_multiply(unif->ambient_color, unif->ambient_reflectance);

    // Diffuse lighting.
    float n_dot_l = vector3_dot(normal, unif->view_space_light_dir);
    float diffuse_intensity = max_float(0.0f, n_dot_l);
    vector3 diffuse_lighting =
        vector3_multiply_scalar(unif->light_color, diffuse_intensity);
    diffuse_lighting =
        vector3_multiply(diffuse_lighting, unif->diffuse_reflectance);

    // Specular lighting.
    vector3 specular_lighting = VECTOR3_ZERO;
    if (n_dot_l > 0.0f) {
        // Because in view space, the camera is at the origin, so the
        // calculation of
        // the view direction is simplified.
        vector3 *postion = shader_context_vector3(input, POSITION);
        vector3 view_direction = vector3_multiply_scalar(*postion, -1.0f);
        view_direction = vector3_normalize(view_direction);
        // Calculate the halfway vector between the light direction and the view
        // direction.
        vector3 halfway =
            vector3_add(view_direction, unif->view_space_light_dir);
        halfway = vector3_normalize(halfway);
        float n_dot_h = vector3_dot(normal, halfway);
        float specular_intensity =
            powf(max_float(0.0f, n_dot_h), unif->shininess);
        specular_lighting =
            vector3_multiply_scalar(unif->light_color, specular_intensity);
        specular_lighting =
            vector3_multiply(specular_lighting, unif->specular_reflectance);
    }

    vector2 *texcoord = shader_context_vector2(input, TEXCOORD);
    vector4 texture_color = VECTOR4_ONE;
    texture_sample(&texture_color, unif->diffuse_texture, *texcoord);

    // Add shadow.
    vector3 *light_space_position =
        shader_context_vector3(input, LIGHT_SPACE_POSITION);
    float visibility =
        shadow_calculation(unif->shadow_map, light_space_position);
    diffuse_lighting = vector3_multiply_scalar(diffuse_lighting, visibility);
    specular_lighting = vector3_multiply_scalar(specular_lighting, visibility);

    vector3 fragment_color = vector3_add(ambient_lighting, diffuse_lighting);
    fragment_color =
        vector3_multiply(fragment_color, vector4_to_3(texture_color));
    fragment_color = vector3_add(fragment_color, specular_lighting);
    return vector3_to_4(fragment_color, 1.0f);
}
