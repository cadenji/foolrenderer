// Copyright (c) Caden Ji. All rights reserved.
//
// Licensed under the MIT License. See LICENSE file in the project root for
// license information.

#include "shaders/standard.h"

#include <math.h>

#include "math/math_utility.h"
#include "math/matrix.h"
#include "math/vector.h"
#include "shader_context.h"
#include "texture.h"

#define TEXCOORD 0
#define WORLD_SPACE_POSITION 0
#define WORLD_SPACE_NORMAL 1
#define WORLD_SPACE_TANGENT 2
#define WORLD_SPACE_BITANGENT 3

struct material_parameter {
    vector3 normal;  // In tangent space.
    vector3 base_color;
    float metallic;
    float roughness;
    float reflectance;
};

// Process user input of material properties into a form that is convenient for
// the shader to use.
static inline void compute_material_parameter(
    struct material_parameter *param, const struct standard_uniform *uniform,
    vector2 texcoord) {
    vector3 normal =
        vector4_to_3(texture_sample(uniform->normal_map, texcoord));
    normal =
        vector3_subtract_scalar(vector3_multiply_scalar(normal, 2.0f), 1.0f);
    param->normal = normal;
    vector3 base_color =
        vector4_to_3(texture_sample(uniform->base_color_map, texcoord));
    base_color = vector3_multiply(uniform->base_color, base_color);
    param->base_color = base_color;
    float metallic = texture_sample(uniform->metallic_map, texcoord).r;
    metallic *= uniform->metallic;
    param->metallic = metallic;
    float roughness = texture_sample(uniform->roughness_map, texcoord).r;
    roughness *= uniform->roughness;
    param->roughness = roughness;
    param->reflectance = uniform->reflectance;
}

static inline float perceptual_roughness_to_a2(float perceptual_roughness) {
    // Prevent being zero, and prevent perceptual_oughness^4 from going out of
    // range of precision.
    perceptual_roughness = float_max(perceptual_roughness, 0.045f);
    float roughness = perceptual_roughness * perceptual_roughness;
    return roughness * roughness;
}

static inline matrix3x3 construct_tangent2world(struct shader_context *input) {
    vector3 t =
        vector3_normalize(*shader_context_vector3(input, WORLD_SPACE_TANGENT));
    vector3 b = vector3_normalize(
        *shader_context_vector3(input, WORLD_SPACE_BITANGENT));
    vector3 n =
        vector3_normalize(*shader_context_vector3(input, WORLD_SPACE_NORMAL));
    return matrix3x3_construct(t, b, n);
}

static inline float pow5(float x) {
    float x2 = x * x;
    return x2 * x2 * x;
}

static inline vector3 f_schlick(vector3 f0, float l_dot_h) {
    // Schlick's approximation is defined as:
    // f_schlick = f0 + (1 - f0) * (1 - l_dot_h)^5
    // This is the optimized code after reducing vector operations.
    float f = pow5(1.0f - l_dot_h);
    return vector3_add_scalar(vector3_multiply_scalar(f0, 1.0f - f), f);
}

static inline float d_ggx(float a2, float n_dot_h) {
    float f = (n_dot_h * a2 - n_dot_h) * n_dot_h + 1.0;
    return a2 / (PI * f * f);
}

static inline float v_smith_ggx_correlated(float a2, float n_dot_l,
                                           float n_dot_v) {
    // Height correlated Smith-GGX formulation:
    // lambda_v = 0.5 * (-1 + sqrt(a2 + (1 - a2) * n_dot_l^2) / n_dot_l)
    // lambda_l = 0.5 * (-1 + sqrt(a2 + (1 - a2) * n_dot_v^2) / n_dot_v)
    // g_smith_ggx_correlated = 1 / (1 + lambda_v + lambda_l)
    // v_smith_ggx_correlated = g_smith_ggx_correlated /
    //                      (4.0f * n_dot_l * n_dot_v)
    // This is the optimized code.
    float lambda_v = n_dot_l * sqrtf((n_dot_v - a2 * n_dot_v) * n_dot_v + a2);
    float lambda_l = n_dot_v * sqrtf((n_dot_l - a2 * n_dot_l) * n_dot_l + a2);
    return 0.5 / (lambda_v + lambda_l);
}

static inline vector3 specular_lobe(float a2, vector3 f0, float n_dot_h,
                                    float n_dot_l, float n_dot_v,
                                    float l_dot_h) {
    // Using Cook-Torrance microfacet BRDF.
    vector3 f = f_schlick(f0, l_dot_h);
    float d = d_ggx(a2, n_dot_h);
    float v = v_smith_ggx_correlated(a2, n_dot_l, n_dot_v);
    return vector3_multiply_scalar(f, d * v);
}

static inline vector3 diffuse_lobe(vector3 diffuse_color) {
    // Using Lambertian BRDF.
    return vector3_multiply_scalar(diffuse_color, 1.0f / PI);
}

vector4 standard_vertex_shader(struct shader_context *output,
                               const void *uniform,
                               const void *vertex_attribute) {
    const struct standard_uniform *unif = uniform;
    const struct standard_vertex_attribute *attr = vertex_attribute;

    vector2 *out_texcoord = shader_context_vector2(output, TEXCOORD);
    *out_texcoord = attr->texcoord;

    vector4 world_position = matrix4x4_multiply_vector4(
        unif->local2world, vector3_to_4(attr->position, 1.0f));
    vector3 *out_position =
        shader_context_vector3(output, WORLD_SPACE_POSITION);
    *out_position = vector4_to_3(world_position);

    vector3 *out_normal = shader_context_vector3(output, WORLD_SPACE_NORMAL);
    *out_normal =
        matrix3x3_multiply_vector3(unif->local2world_normal, attr->normal);

    vector3 *out_tangent = shader_context_vector3(output, WORLD_SPACE_TANGENT);
    *out_tangent = matrix3x3_multiply_vector3(unif->local2world_direction,
                                              vector4_to_3(attr->tangent));

    vector3 *out_bitangent =
        shader_context_vector3(output, WORLD_SPACE_BITANGENT);
    *out_bitangent = vector3_multiply_scalar(
        vector3_cross(*out_normal, *out_tangent), attr->tangent.w);

    return matrix4x4_multiply_vector4(unif->world2clip, world_position);
}

vector4 standard_fragment_shader(struct shader_context *input,
                                 const void *uniform) {
    vector2 texcoord = *shader_context_vector2(input, TEXCOORD);
    vector3 position = *shader_context_vector3(input, WORLD_SPACE_POSITION);
    const struct standard_uniform *unif = uniform;
    vector3 camera_position = unif->camera_position;
    vector3 light_direction = unif->light_direction;
    vector3 illuminance = unif->illuminance;

    struct material_parameter material;
    compute_material_parameter(&material, unif, texcoord);

    vector3 diffuse_color = vector3_multiply_scalar(material.base_color,
                                                    (1.0f - material.metallic));
    float dielectric_f0 = 0.16f * material.reflectance * material.reflectance *
                          (1.0f - material.metallic);
    vector3 conductor_f0 =
        vector3_multiply_scalar(material.base_color, material.metallic);
    vector3 f0 = vector3_add_scalar(conductor_f0, dielectric_f0);
    float a2 = perceptual_roughness_to_a2(material.roughness);
    matrix3x3 tangent2world = construct_tangent2world(input);
    // Normalized normal, in world space.
    vector3 normal = matrix3x3_multiply_vector3(tangent2world, material.normal);
    // Normalized vector from the fragment to the camera, in world space.
    vector3 view =
        vector3_normalize(vector3_subtract(camera_position, position));
    // Normalized halfway vector between the light direction and the view
    // direction, in world space.
    vector3 halfway = vector3_normalize(vector3_add(view, light_direction));

    float n_dot_v =
        float_max(vector3_dot(normal, view), 1e-4f);  // Avoid artifact.
    float n_dot_l = float_max(vector3_dot(normal, light_direction), 0.0f);
    float n_dot_h = float_max(vector3_dot(normal, halfway), 0.0f);
    float l_dot_h = float_max(vector3_dot(light_direction, halfway), 0.0f);

    vector3 fr = specular_lobe(a2, f0, n_dot_h, n_dot_l, n_dot_v, l_dot_h);
    vector3 fd = diffuse_lobe(diffuse_color);
    vector3 output = vector3_multiply(vector3_add(fr, fd), illuminance);
    output = vector3_multiply_scalar(output, n_dot_l);
    return vector3_to_4(output, 1.0f);
}
