// Copyright (c) 2021 Caden Ji
//
// MIT License
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <cwalk.h>
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
#include "shaders/basic.h"
#include "texture.h"

#define IMAGE_WIDTH 512
#define IMAGE_HEIGHT 512

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

static struct texture *load_diffuse_texture(struct mesh *mesh) {
    const char *diffuse_texture_name = mesh_get_diffuse_texture_name(mesh);
    if (strlen(diffuse_texture_name) == 0) {
        return NULL;
    }
    const char *directory_name = mesh_get_directory_name(mesh);
    size_t image_name_length =
        strlen(directory_name) + strlen(diffuse_texture_name) + 2;
    char image_name[image_name_length];
    cwk_path_join(directory_name, diffuse_texture_name, image_name,
                  image_name_length);

    uint8_t *image_data;
    tga_info *image_info;
    enum tga_error error_code;
    error_code = tga_load(&image_data, &image_info, image_name);
    if (error_code != TGA_NO_ERROR) {
        return NULL;
    }
    tga_image_flip_v(image_data, image_info);

    struct texture *diffuse_texture = NULL;
    if (tga_get_pixel_format(image_info) == TGA_PIXEL_RGB24) {
        uint32_t width = tga_get_image_width(image_info);
        uint32_t height = tga_get_image_height(image_info);
        // Convert all pixels to big endian.
        uint8_t *pixel;
        for (uint32_t y = 0; y < height; y++) {
            for (uint32_t x = 0; x < width; x++) {
                pixel = tga_get_pixel(image_data, image_info, x, y);
                endian_inversion(pixel, 3);
            }
        }
        diffuse_texture = generate_texture(TEXTURE_FORMAT_RGBA8, width, height);
        if (diffuse_texture != NULL) {
            set_texture_pixels(diffuse_texture, image_data);
        }
    }

    tga_free_data(image_data);
    tga_free_info(image_info);
    return diffuse_texture;
}

static void draw_model(struct framebuffer *framebuffer, struct mesh *mesh) {
    set_viewport(0, 0, IMAGE_WIDTH, IMAGE_HEIGHT);
    set_vertex_shader(basic_vertex_shader);
    set_fragment_shader(basic_fragment_shader);
    clear_framebuffer(framebuffer);

    struct basic_uniform uniform;
    // No rotation, scaling, or translation of the model, so the model matrix is
    // the identity matrix.
    uniform.modelview = matrix4x4_look_at((vector3){{1.0f, 1.5f, 2.5f}},
                                          (vector3){{0.0f, 0.0f, 0.0f}},
                                          (vector3){{0.0f, 1.0f, 0.0f}});
    uniform.projection = matrix4x4_perspective(
        HALF_PI / 2.0f, IMAGE_WIDTH / IMAGE_HEIGHT, 0.1f, 5.0f);
    // The normal matrix is the inverse transpose matrix of the original
    // transformation matrix. And because the modelview matrix is actually the
    // view matrix, it only contains rotation and translation. So the normal
    // matrix in the view space is the modelview matrix (The inverse transpose
    // of the rotation matrix is itself, translation can be ignored).
    uniform.normal_matrix = uniform.modelview;
    // The direction of the light in world space.
    vector3 light_direction = {{0.5f, 0.5f, 1.0f}};
    vector3_normalize(light_direction);
    vector4 light_direction_in_view = matrix4x4_multiply_vector4(
        uniform.modelview, vector3_to_4(light_direction, 0.0f));
    uniform.light_direction = vector4_to_3(light_direction_in_view);
    uniform.light_color = VECTOR3_ONE;
    uniform.ambient_color = (vector3){{0.1f, 0.1f, 0.1f}};
    uniform.ambient_reflectance = VECTOR3_ONE;
    uniform.diffuse_reflectance = VECTOR3_ONE;
    uniform.specular_reflectance = VECTOR3_ONE;
    uniform.shininess = 100.0f;
    uniform.diffuse_texture = load_diffuse_texture(mesh);

    uint32_t triangle_count = mesh_triangle_count(mesh);
    for (size_t t = 0; t < triangle_count; t++) {
        struct basic_vertex_attribute attributes[3];
        const void *attribute_ptrs[3];
        for (uint32_t v = 0; v < 3; v++) {
            mesh_get_vertex_position(&attributes[v].position, mesh, t, v);
            mesh_get_normal(&attributes[v].normal, mesh, t, v);
            mesh_get_texture_coordinates(&attributes[v].texcoord, mesh, t, v);
            attribute_ptrs[v] = attributes + v;
        }
        draw_triangle(framebuffer, &uniform, attribute_ptrs);
    }
    delete_texture(uniform.diffuse_texture);
}

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
    // This program uses OpenGL style coordinate system, the origin of window
    // space is in the bottom-left corner. But the tgafunc default image origin
    // is in the upper-left corner, so need to flip the image in the Y-axis
    // direction.
    tga_image_flip_v(image_data, image_info);
    tga_save_from_info(image_data, image_info, "output.tga");

    tga_free_data(image_data);
    tga_free_info(image_info);
}

int main(int argc, char *argv[]) {
    char *model_name;
    if (argc < 2) {
        model_name = "assets/test_cube/test_cube.obj";
    } else {
        model_name = argv[1];
    }
    // Load .obj data.
    struct mesh *mesh;
    mesh = mesh_load(model_name);
    if (mesh == NULL) {
        printf("Cannot load .obj file.\n");
        return 0;
    }

    // Create framebuffer.
    struct framebuffer *framebuffer = generate_framebuffer();
    struct texture *color_texture =
        generate_texture(TEXTURE_FORMAT_RGBA8, IMAGE_WIDTH, IMAGE_HEIGHT);
    struct texture *depth_texture =
        generate_texture(TEXTURE_FORMAT_DEPTH_FLOAT, IMAGE_WIDTH, IMAGE_HEIGHT);
    attach_texture_to_framebuffer(framebuffer, COLOR_ATTACHMENT, color_texture);
    attach_texture_to_framebuffer(framebuffer, DEPTH_ATTACHMENT, depth_texture);

    draw_model(framebuffer, mesh);
    save_color_texture(color_texture);

    // Release framebuffer.
    delete_texture(color_texture);
    delete_texture(depth_texture);
    delete_framebuffer(framebuffer);

    mesh_free(mesh);
    return 0;
}
