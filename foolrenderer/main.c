// Copyright (c) Caden Ji. All rights reserved.
//
// Licensed under the MIT License. See LICENSE file in the project root for
// license information.

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include "graphics/framebuffer.h"
#include "graphics/rasterizer.h"
#include "graphics/texture.h"
#include "math/math_utility.h"
#include "math/matrix.h"
#include "math/vector.h"
#include "shaders/standard.h"
#include "utilities/image.h"
#include "utilities/mesh.h"

#define IMAGE_WIDTH 1024
#define IMAGE_HEIGHT 1024

const vector3 LIGHT_DIRECTION = {{1.0f, 1.0f, 0.5f}};
const vector3 CAMERA_POSITION = {{0.0f, 0.0f, 2.5f}};

static void set_shader_uniform(struct standard_uniform *uniform) {
    uniform->local2world = MATRIX4X4_IDENTITY;
    matrix4x4 world2view = matrix4x4_look_at(CAMERA_POSITION, VECTOR3_ZERO,
                                             (vector3){{0.0f, 1.0f, 0.0f}});
    matrix4x4 view2clip = matrix4x4_perspective(
        PI / 4.0f, IMAGE_WIDTH / IMAGE_HEIGHT, 0.1f, 5.0f);
    uniform->world2clip = matrix4x4_multiply(view2clip, world2view);
    uniform->local2world_direction = matrix4x4_to_3x3(uniform->local2world);
    // There is no non-uniform scaling so the normal transformation matrix is
    // the direction transformation matrix.
    uniform->local2world_normal = uniform->local2world_direction;
    uniform->camera_position = CAMERA_POSITION;
    uniform->light_direction = vector3_normalize(LIGHT_DIRECTION);
    uniform->illuminance = (vector3){{2.0f, 2.0f, 2.0f}};
    uniform->ambient_luminance = (vector3){{0.02f, 0.02f, 0.02f}};
    uniform->base_color = VECTOR3_ONE;
    uniform->metallic = 1.0f;
    uniform->roughness = 1.0f;
    uniform->reflectance = 0.5f;  // Common dielectric surfaces F0.
}

static void draw_model(struct mesh *mesh, struct texture *base_color_map,
                       struct texture *normal_map, struct texture *metallic_map,
                       struct texture *roughness_map) {
    struct framebuffer *framebuffer = create_framebuffer();
    struct texture *color_texture =
        create_texture(TEXTURE_FORMAT_SRGB8_A8, IMAGE_WIDTH, IMAGE_HEIGHT);
    struct texture *depth_texture =
        create_texture(TEXTURE_FORMAT_DEPTH_FLOAT, IMAGE_WIDTH, IMAGE_HEIGHT);
    attach_texture_to_framebuffer(framebuffer, COLOR_ATTACHMENT, color_texture);
    attach_texture_to_framebuffer(framebuffer, DEPTH_ATTACHMENT, depth_texture);

    // Draw the model.
    set_viewport(0, 0, IMAGE_WIDTH, IMAGE_HEIGHT);
    set_vertex_shader(standard_vertex_shader);
    set_fragment_shader(standard_fragment_shader);
    clear_framebuffer(framebuffer);

    struct standard_uniform uniform;
    set_shader_uniform(&uniform);
    uniform.base_color_map = base_color_map;
    uniform.normal_map = normal_map;
    uniform.metallic_map = metallic_map;
    uniform.roughness_map = roughness_map;

    uint32_t triangle_count = mesh->triangle_count;
    for (size_t t = 0; t < triangle_count; t++) {
        struct standard_vertex_attribute attributes[3];
        const void *attribute_ptrs[3];
        for (uint32_t v = 0; v < 3; v++) {
            get_mesh_position(&attributes[v].position, mesh, t, v);
            get_mesh_normal(&attributes[v].normal, mesh, t, v);
            get_mesh_tangent(&attributes[v].tangent, mesh, t, v);
            get_mesh_texcoord(&attributes[v].texcoord, mesh, t, v);
            attribute_ptrs[v] = attributes + v;
        }
        draw_triangle(framebuffer, &uniform, attribute_ptrs);
    }

    save_image(color_texture, "output.tga");

    // Release framebuffer.
    destroy_texture(color_texture);
    destroy_texture(depth_texture);
    destroy_framebuffer(framebuffer);
}

int main(void) {
    const char *model_path = "assets/sphere/sphere.obj";
    const char *base_color_map_path = "assets/rusted_iron/base_color.tga";
    const char *normal_map_path = "assets/rusted_iron/normal.tga";
    const char *metallic_map_path = "assets/rusted_iron/metallic.tga";
    const char *roughness_map_path = "assets/rusted_iron/roughness.tga";

    // Load model data.
    struct mesh *mesh;
    mesh = load_mesh(model_path);
    if (mesh == NULL) {
        printf("Cannot load .obj file.\n");
        return 0;
    }
    struct texture *base_color_map = load_image(base_color_map_path, true);
    struct texture *normal_map = load_image(normal_map_path, false);
    struct texture *metallic_map = load_image(metallic_map_path, false);
    struct texture *roughness_map = load_image(roughness_map_path, false);
    if (base_color_map == NULL || normal_map == NULL || metallic_map == NULL ||
        roughness_map == NULL) {
        printf("Cannot load texture files.\n");
        destroy_mesh(mesh);
        destroy_texture(base_color_map);
        destroy_texture(normal_map);
        destroy_texture(metallic_map);
        destroy_texture(roughness_map);
        return 0;
    }

    draw_model(mesh, base_color_map, normal_map, metallic_map, roughness_map);

    destroy_mesh(mesh);
    destroy_texture(base_color_map);
    destroy_texture(normal_map);
    destroy_texture(metallic_map);
    destroy_texture(roughness_map);
    return 0;
}
