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

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Needs input .obj file name.\n");
        return 0;
    }
    // Load .obj data.
    struct mesh *mesh;
    mesh = mesh_load(argv[1]);
    if (mesh == NULL) {
        printf("Cannot load .obj file.\n");
        return 0;
    }
    uint32_t triangle_count = mesh_triangle_count(mesh);

    // Create framebuffer.
    struct framebuffer *framebuffer;
    framebuffer = generate_framebuffer(IMAGE_WIDTH, IMAGE_HEIGHT);

    // For shading, use the direction opposite to the direction of the parallel
    // light.
    vector3 light_direction = {{0.0f, 0.0f, 1.0f}};
    matrix4x4 view_matrix = matrix4x4_look_at((vector3){{1.0f, 1.0f, 3.0f}},
                                              (vector3){{0.0f, 0.0f, 0.0f}},
                                              (vector3){{0.0f, 1.0f, 0.0f}});
    matrix4x4 projection_matrix = matrix4x4_perspective(
        HALF_PI / 2.0f, IMAGE_WIDTH / IMAGE_HEIGHT, 0.1f, 5.0f);
    matrix4x4 transform_matrix =
        matrix4x4_multiply(projection_matrix, view_matrix);

    // Draw the model.
    set_viewport(0, 0, IMAGE_WIDTH, IMAGE_HEIGHT);
    vector3 colors[3];
    vector4 triangle_vertices[3];
    for (size_t t = 0; t < triangle_count; t++) {
        vector3 vertices[3];
        mesh_get_vertex_positions(vertices, mesh, t);
        vector3 normals[3];
        mesh_get_normals(normals, mesh, t);
        for (int i = 0; i < 3; i++) {
            // Gouraud shading.
            float intensity =
                clamp01_float(vector3_dot(normals[i], light_direction));
            colors[i].x = intensity;
            colors[i].y = intensity;
            colors[i].z = intensity;
            // Transform vertices to clip space.
            triangle_vertices[i] = vector3_to_4(vertices[i], 1);
            triangle_vertices[i] = matrix4x4_multiply_vector4(
                transform_matrix, triangle_vertices[i]);
        }
        draw_triangle(framebuffer, triangle_vertices, colors);
    }

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
    delete_framebuffer(framebuffer);
    mesh_free(mesh);
    return 0;
}
