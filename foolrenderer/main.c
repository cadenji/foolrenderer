// Copyright (c) Caden Ji. All rights reserved.
//
// Licensed under the MIT License. See LICENSE file in the project root for
// license information.

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <tgafunc.h>

#include "framebuffer.h"
#include "graphics.h"
#include "math/math_utility.h"
#include "math/matrix.h"
#include "math/vector.h"
#include "mesh.h"
#include "shaders/standard.h"
#include "texture.h"

#define IMAGE_WIDTH 512
#define IMAGE_HEIGHT 512

const vector3 LIGHT_DIRECTION = {{1.0f, 1.0f, 0.5f}};
const vector3 CAMERA_POSITION = {{0.0f, 0.0f, 2.5f}};

static void endian_inversion(uint8_t *bytes, size_t size) {
    uint8_t buffer;
    size_t left = 0, right = size - 1;
    while (left < right) {
        buffer = bytes[left];
        bytes[left] = bytes[right];
        bytes[right] = buffer;
        ++left;
        --right;
    }
}

// static struct texture *load_texture(const char *filename) {
//     if (filename == NULL || strlen(filename) == 0) {
//         return NULL;
//     }

//     uint8_t *image_data;
//     tga_info *image_info;
//     enum tga_error error_code;
//     error_code = tga_load(&image_data, &image_info, filename);
//     if (error_code != TGA_NO_ERROR) {
//         return NULL;
//     }
//     tga_image_flip_v(image_data, image_info);

//     struct texture *texture = NULL;
//     if (tga_get_pixel_format(image_info) == TGA_PIXEL_RGB24) {
//         uint32_t width = tga_get_image_width(image_info);
//         uint32_t height = tga_get_image_height(image_info);
//         // Convert all pixels to big endian.
//         uint8_t *pixel;
//         for (uint32_t y = 0; y < height; y++) {
//             for (uint32_t x = 0; x < width; x++) {
//                 pixel = tga_get_pixel(image_data, image_info, x, y);
//                 endian_inversion(pixel, 3);
//             }
//         }
//         texture = create_texture(TEXTURE_FORMAT_RGB8, width, height);
//         if (texture != NULL) {
//             set_texture_pixels(texture, image_data);
//         }
//     }

//     tga_free_data(image_data);
//     tga_free_info(image_info);
//     return texture;
// }

static void save_color_texture(struct texture *texture) {
    uint32_t texture_width = get_texture_width(texture);
    uint32_t texture_height = get_texture_height(texture);
    const uint8_t *texture_data = get_texture_pixels(texture);

    uint8_t *image_data;
    tga_info *image_info;
    tga_create(&image_data, &image_info, texture_width, texture_height,
               TGA_PIXEL_RGB24);

    // Copy the color buffer data to the TGA image.
    for (uint32_t y = 0; y < texture_width; y++) {
        for (uint32_t x = 0; x < texture_height; x++) {
            const uint8_t *texture_pixel =
                texture_data + (y * texture_width + x) * 4;
            uint8_t *image_pixel = tga_get_pixel(image_data, image_info, x, y);
            image_pixel[0] = texture_pixel[0];
            image_pixel[1] = texture_pixel[1];
            image_pixel[2] = texture_pixel[2];
            // Convert all pixels to little endian.
            endian_inversion(image_pixel, 3);
        }
    }
    // This program uses OpenGL style coordinate system, the origin of screen
    // space is in the bottom-left corner. But the tgafunc default image origin
    // is in the upper-left corner, so need to flip the image in the Y-axis
    // direction.
    tga_image_flip_v(image_data, image_info);
    tga_save_from_info(image_data, image_info, "output.tga");

    tga_free_data(image_data);
    tga_free_info(image_info);
}

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
    uniform->light_intensity = VECTOR3_ONE;
    uniform->base_color = (vector3){{1.0f, 0.0f, 0.0f}};
    uniform->metallic = 0.0f;
    uniform->roughness = 0.2f;
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

    save_color_texture(color_texture);

    // Release framebuffer.
    destroy_texture(color_texture);
    destroy_texture(depth_texture);
    destroy_framebuffer(framebuffer);
}

int main(void) {
    const char *model_path = "assets/sphere/sphere.obj";

    // Load model data.
    struct mesh *mesh;
    mesh = load_mesh(model_path);
    if (mesh == NULL) {
        printf("Cannot load .obj file.\n");
        return 0;
    }
    struct texture *base_color_map = create_texture(TEXTURE_FORMAT_RGBA8, 1, 1);
    struct texture *normal_map = create_texture(TEXTURE_FORMAT_RGBA8, 1, 1);
    struct texture *metallic_map = create_texture(TEXTURE_FORMAT_R8, 1, 1);
    struct texture *roughness_map = create_texture(TEXTURE_FORMAT_R8, 1, 1);
    if (base_color_map == NULL || normal_map == NULL || metallic_map == NULL ||
        roughness_map == NULL) {
        printf("Cannot create texture.\n");
        destroy_mesh(mesh);
        destroy_texture(base_color_map);
        destroy_texture(normal_map);
        destroy_texture(metallic_map);
        destroy_texture(roughness_map);
        return 0;
    }
    // Set textures to their respective defaults, equivalent to not using
    // textures.
    uint8_t base_color_data[3] = {255, 255, 255};
    set_texture_pixels(base_color_map, base_color_data);
    uint8_t normal_data[3] = {128, 128, 255};
    set_texture_pixels(normal_map, normal_data);
    uint8_t white = 255;
    set_texture_pixels(metallic_map, &white);
    set_texture_pixels(roughness_map, &white);

    draw_model(mesh, base_color_map, normal_map, metallic_map, roughness_map);

    destroy_mesh(mesh);
    destroy_texture(base_color_map);
    destroy_texture(normal_map);
    destroy_texture(metallic_map);
    destroy_texture(roughness_map);
    return 0;
}
