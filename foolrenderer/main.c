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

#include "graphics.h"
#include "math/math_utility.h"
#include "math/matrix.h"
#include "math/vector.h"
#include "mesh.h"
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
        diffuse_texture = generate_texture(width, height, image_data);
    }

    tga_free_data(image_data);
    tga_free_info(image_info);
    return diffuse_texture;
}

static void draw_model(struct framebuffer *framebuffer, struct mesh *mesh) {
    set_viewport(0, 0, IMAGE_WIDTH, IMAGE_HEIGHT);
    vector3 ambient = {{0.3f, 0.3f, 0.3f}};
    vector3 world_light = {{0.0f, 0.0f, 1.0f}};
    matrix4x4 view_matrix = matrix4x4_look_at((vector3){{1.0f, 1.5f, 2.5f}},
                                              (vector3){{0.0f, 0.0f, 0.0f}},
                                              (vector3){{0.0f, 1.0f, 0.0f}});
    matrix4x4 projection_matrix = matrix4x4_perspective(
        HALF_PI / 2.0f, IMAGE_WIDTH / IMAGE_HEIGHT, 0.1f, 5.0f);
    matrix4x4 transform_matrix =
        matrix4x4_multiply(projection_matrix, view_matrix);

    struct texture *diffuse_texture = load_diffuse_texture(mesh);
    uint32_t triangle_count = mesh_triangle_count(mesh);
    float intensities[3];
    vector4 vertex_positions[3];
    vector2 texture_coordinates[3];
    for (size_t t = 0; t < triangle_count; t++) {
        mesh_get_texture_coordinates(texture_coordinates, mesh, t);
        vector3 positions[3];
        mesh_get_vertex_positions(positions, mesh, t);
        vector3 normals[3];
        mesh_get_normals(normals, mesh, t);
        for (int i = 0; i < 3; i++) {
            // Gouraud shading.
            float intensity = vector3_dot(normals[i], world_light);
            intensities[i] = intensity + ambient.elements[i];
            intensities[i] = clamp01_float(intensities[i]);
            // Transform positions to clip space.
            vertex_positions[i] = vector3_to_4(positions[i], 1);
            vertex_positions[i] = matrix4x4_multiply_vector4(
                transform_matrix, vertex_positions[i]);
        }
        draw_triangle(framebuffer, vertex_positions, texture_coordinates,
                      diffuse_texture, intensities);
    }
    delete_texture(diffuse_texture);
}

static void save_framebuffer(const struct framebuffer *framebuffer) {
    // Copy the color buffer data to the TGA image.
    uint8_t *tga_data;
    tga_info *tga_info;
    tga_create(&tga_data, &tga_info, IMAGE_WIDTH, IMAGE_HEIGHT,
               TGA_PIXEL_RGB24);
    memcpy(tga_data, framebuffer->color_buffer,
           (size_t)IMAGE_WIDTH * IMAGE_HEIGHT * 3);
    // Convert all pixels to little endian and save as TGA format file.
    uint8_t *pixel;
    for (int y = 0; y < IMAGE_WIDTH; y++) {
        for (int x = 0; x < IMAGE_HEIGHT; x++) {
            pixel = tga_get_pixel(tga_data, tga_info, x, y);
            endian_inversion(pixel, 3);
        }
    }
    // This program uses OpenGL style coordinate system, the origin of window
    // space is in the bottom-left corner. But the tgafunc default image origin
    // is in the upper-left corner, so need to flip the image in the Y-axis
    // direction.
    tga_image_flip_v(tga_data, tga_info);
    tga_save_from_info(tga_data, tga_info, "output.tga");

    tga_free_data(tga_data);
    tga_free_info(tga_info);
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
    struct framebuffer *framebuffer;
    framebuffer = generate_framebuffer(IMAGE_WIDTH, IMAGE_HEIGHT);

    draw_model(framebuffer, mesh);
    save_framebuffer(framebuffer);

    delete_framebuffer(framebuffer);
    mesh_free(mesh);
    return 0;
}
