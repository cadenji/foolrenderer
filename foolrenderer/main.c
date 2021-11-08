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

#define TINYOBJ_LOADER_C_IMPLEMENTATION
#include <tinyobj_loader_c.h>

#include "graphics.h"
#include "math/matrix.h"
#include "math/vector.h"

#define IMAGE_WIDTH 512
#define IMAGE_HEIGHT 512

// As a callback function of tinyobj_parse_obj(). Provides services for loading
// files into memory.
static void get_file_data(void *context, const char *file_name, int is_mtl,
                          const char *obj_file_name, char **buffer,
                          size_t *buffer_size) {
    (void)is_mtl;
    (void)obj_file_name;
    void **user_context = context;
    *user_context = NULL;
    *buffer = NULL;
    *buffer_size = 0;

    FILE *file = fopen(file_name, "r");
    if (file == NULL) {
        return;
    }
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *file_data = malloc(file_size + 1);
    if (file_data == NULL) {
        fclose(file);
        return;
    }
    if (fread(file_data, 1, file_size, file) != (unsigned long)file_size) {
        fclose(file);
        free(file_data);
        return;
    }
    fclose(file);
    file_data[file_size] = '\0';

    *buffer = file_data;
    *buffer_size = file_size + 1;
    // The file data will be used in tinyobj_parse_obj(), so it cannot be
    // released now. The allocated memory address is stored in the context, so
    // that it can be released (use free() function) after the
    // tinyobj_parse_obj() call ends.
    *user_context = file_data;
}

// Loads model data from .obj file.
// Returns true if encountered an error while loading, otherwise returns false.
static bool load_obj(vector3 **vertex_array, size_t *triangle_count,
                     const char *file_name) {
    void *context = NULL;
    tinyobj_attrib_t attrib;
    tinyobj_shape_t *shapes = NULL;
    size_t num_shapes;
    tinyobj_material_t *materials = NULL;
    size_t num_materials;
    int result = tinyobj_parse_obj(&attrib, &shapes, &num_shapes, &materials,
                                   &num_materials, file_name, get_file_data,
                                   &context, TINYOBJ_FLAG_TRIANGULATE);
    // The file parsing is complete, release the file data.
    free(context);
    if (result != TINYOBJ_SUCCESS) {
        return true;
    }

    bool has_error = false;
    size_t num_triangles = attrib.num_face_num_verts;
    vector3 *vertices = (vector3 *)malloc(sizeof(vector3) * 3 * num_triangles);
    if (vertices == NULL) {
        has_error = true;
    } else {
        for (size_t t = 0; t < num_triangles; t++) {
            for (int point = 0; point < 3; point++) {
                // Get vertex data based on vertex index.
                const size_t offset = t * 3 + point;
                tinyobj_vertex_index_t tinyobj_index = attrib.faces[offset];
                int vertex_index = tinyobj_index.v_idx;
                size_t attrib_vertex_index = 3 * (size_t)vertex_index;
                vector3 vertex_data;
                vertex_data.x = attrib.vertices[attrib_vertex_index + 0];
                vertex_data.y = attrib.vertices[attrib_vertex_index + 1];
                vertex_data.z = attrib.vertices[attrib_vertex_index + 2];
                // Copy vertex data to vertices.
                vector3 *v = vertices + offset;
                memcpy(v, &vertex_data, sizeof(vertex_data));
            }
        }
        *vertex_array = vertices;
        *triangle_count = num_triangles;
    }
    tinyobj_attrib_free(&attrib);
    tinyobj_shapes_free(shapes, num_shapes);
    tinyobj_materials_free(materials, num_materials);
    return has_error;
}

// This function requires the right-handed coordinate system, and the triangle
// use counterclockwise winding.
static vector3 calculate_triangle_normal(const vector3 vertices[]) {
    // Refer to:
    // https://www.khronos.org/opengl/wiki/Calculating_a_Surface_Normal
    vector3 v01 = vector3_subtract(vertices[1], vertices[0]);
    vector3 v02 = vector3_subtract(vertices[2], vertices[0]);
    vector3 n = vector3_cross(v01, v02);
    return vector3_normalize(n);
}

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
    vector3 *vertex_array;
    size_t triangle_count;
    if (load_obj(&vertex_array, &triangle_count, argv[1])) {
        printf("Cannot load .obj file.\n");
        return 0;
    }
    // Create framebuffer.
    uint8_t *data;
    tga_info *info;
    tga_create(&data, &info, IMAGE_WIDTH, IMAGE_HEIGHT, TGA_PIXEL_RGB24);

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
    for (size_t t = 0; t < triangle_count; t++) {
        vector3 *triangle_vertices = vertex_array + (t * 3);
        // Flat shading.
        vector3 normal = calculate_triangle_normal(triangle_vertices);
        float intensity = vector3_dot(normal, light_direction);
        if (intensity > 0.0f) {
            for (int i = 0; i < 3; i++) {
                // Transform vertices to NDC.
                vector4 vertex = vector3_to_4(triangle_vertices[i], 1);
                vertex = matrix4x4_multiply_vector4(transform_matrix, vertex);
                triangle_vertices[i].x = vertex.x / vertex.w;
                triangle_vertices[i].y = vertex.y / vertex.w;
                triangle_vertices[i].z = vertex.z / vertex.w;
                // Set vertice color.
                colors[i].x = intensity;
                colors[i].y = intensity;
                colors[i].z = intensity;
            }
            draw_triangle(triangle_vertices, colors, data);
        }
    }

    // Convert all pixels to little endian and save as TGA format file.
    uint8_t *pixel;
    for (int y = 0; y < IMAGE_WIDTH; y++) {
        for (int x = 0; x < IMAGE_HEIGHT; x++) {
            pixel = tga_get_pixel(data, info, x, y);
            endian_inversion(pixel, 3);
        }
    }
    // This program uses OpenGL style coordinate system, the origin of window
    // space is in the bottom-left corner. But the tgafunc default image origin
    // is in the upper-left corner, so need to flip the image in the Y-axis
    // direction.
    tga_image_flip_v(data, info);
    tga_save_from_info(data, info, "output.tga");

    free(vertex_array);
    tga_free_data(data);
    tga_free_info(info);
    return 0;
}
