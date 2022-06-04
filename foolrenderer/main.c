// Copyright (c) Caden Ji. All rights reserved.
//
// Licensed under the MIT License. See LICENSE file in the project root for
// license information.

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <tgafunc.h>

#include "graphics/framebuffer.h"
#include "graphics/rasterizer.h"
#include "graphics/texture.h"
#include "math/math_utility.h"
#include "math/matrix.h"
#include "math/vector.h"
#include "mesh.h"
#include "shaders/standard.h"

#define IMAGE_WIDTH 1024
#define IMAGE_HEIGHT 1024

const vector3 LIGHT_DIRECTION = {{1.0f, 1.0f, 0.5f}};
const vector3 CAMERA_POSITION = {{0.0f, 0.0f, 2.5f}};

// Convert TGA image pixels to texture's pixels or texture's pixels to TGA image
// pixels. Each component of the pixel must be an 8-bit unsigned integer type,
// and the number of components of pixels must be greater than or equal to 3.
static inline void pixel_endian_inversion(uint8_t *pixel) {
    // Swap the values of the 1st and 3rd components of a pixel.
    uint8_t buffer = pixel[0];
    pixel[0] = pixel[2];
    pixel[2] = buffer;
}

// Modify the components of the TGA image pixels to the order expected by the
// texture. Only TGA images with pixel formats TGA_PIXEL_RGB24 and
// TGA_PIXEL_ARGB32 are supported.
static void modify_tga_image_pixel(uint8_t *image_data, tga_info *image_info) {
    enum tga_pixel_format pixel_format = tga_get_pixel_format(image_info);
    if (pixel_format != TGA_PIXEL_RGB24 && pixel_format != TGA_PIXEL_ARGB32) {
        return;
    }
    uint32_t width = tga_get_image_width(image_info);
    uint32_t height = tga_get_image_height(image_info);
    for (uint32_t y = 0; y < width; y++) {
        for (uint32_t x = 0; x < height; x++) {
            uint8_t *pixel = tga_get_pixel(image_data, image_info, x, y);
            pixel_endian_inversion(pixel);
        }
    }
}

static struct texture *load_texture(const char *filename,
                                    bool is_srgb_encoding) {
    if (filename == NULL || strlen(filename) == 0) {
        return NULL;
    }

    uint8_t *image_data;
    tga_info *image_info;
    enum tga_error error_code;
    error_code = tga_load(&image_data, &image_info, filename);
    if (error_code != TGA_NO_ERROR) {
        return NULL;
    }
    enum tga_pixel_format image_pixel_format = tga_get_pixel_format(image_info);
    uint32_t width = tga_get_image_width(image_info);
    uint32_t height = tga_get_image_height(image_info);
    tga_image_flip_v(image_data, image_info);

    struct texture *texture = NULL;
    if (image_pixel_format == TGA_PIXEL_BW8) {
        texture = create_texture(TEXTURE_FORMAT_R8, width, height);
        if (texture != NULL) {
            set_texture_pixels(texture, image_data);
        }
    } else if (image_pixel_format == TGA_PIXEL_RGB24) {
        modify_tga_image_pixel(image_data, image_info);
        enum texture_format texture_format =
            is_srgb_encoding ? TEXTURE_FORMAT_SRGB8 : TEXTURE_FORMAT_RGB8;
        texture = create_texture(texture_format, width, height);
        if (texture != NULL) {
            set_texture_pixels(texture, image_data);
        }
    } else if (image_pixel_format == TGA_PIXEL_ARGB32) {
        modify_tga_image_pixel(image_data, image_info);
        enum texture_format texture_format =
            is_srgb_encoding ? TEXTURE_FORMAT_SRGB8_A8 : TEXTURE_FORMAT_RGBA8;
        texture = create_texture(texture_format, width, height);
        if (texture != NULL) {
            set_texture_pixels(texture, image_data);
        }
    }

    tga_free_data(image_data);
    tga_free_info(image_info);
    return texture;
}

static void save_rendering_result(struct texture *texture) {
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
            // Convert the pixel components to the desired arrangement order of
            // the TGA image.
            pixel_endian_inversion(image_pixel);
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

    save_rendering_result(color_texture);

    // Release framebuffer.
    destroy_texture(color_texture);
    destroy_texture(depth_texture);
    destroy_framebuffer(framebuffer);
}

int main(void) {
    const char *model_path = "assets/sphere/sphere.obj";
    const char *base_color_map_path = "assets/suzanne/base_color.tga";
    const char *normal_map_path = "assets/suzanne/normal.tga";
    const char *metallic_map_path = "assets/suzanne/metallic.tga";
    const char *roughness_map_path = "assets/suzanne/roughness.tga";

    // Load model data.
    struct mesh *mesh;
    mesh = load_mesh(model_path);
    if (mesh == NULL) {
        printf("Cannot load .obj file.\n");
        return 0;
    }
    struct texture *base_color_map = load_texture(base_color_map_path, true);
    struct texture *normal_map = load_texture(normal_map_path, false);
    struct texture *metallic_map = load_texture(metallic_map_path, false);
    struct texture *roughness_map = load_texture(roughness_map_path, false);
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
