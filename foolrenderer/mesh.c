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

#include "mesh.h"

#include <cwalk.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TINYOBJ_LOADER_C_IMPLEMENTATION
#include <tinyobj_loader_c.h>

#include "math/vector.h"

struct mesh {
    tinyobj_attrib_t attribute;
    tinyobj_shape_t *shapes;
    tinyobj_material_t *materials;
    char *directory_name;
    size_t shape_count;
    size_t material_count;
};

// As a callback function of tinyobj_parse_obj(). Provides services for loading
// files into memory.
static void get_file_data(void *context, const char *file_name, int is_mtl,
                          const char *obj_file_name, char **buffer,
                          size_t *buffer_size) {
    void **user_context = context;
    *user_context = NULL;
    *buffer = NULL;
    *buffer_size = 0;
    if (file_name == NULL) {
        return;
    }

    FILE *file;
    if (is_mtl && obj_file_name != NULL) {
        // For .mtl, extract directory name from .obj filename and append
        // .mtl filename.
        size_t directory_name_length;
        cwk_path_get_dirname(obj_file_name, &directory_name_length);
        if (directory_name_length > 0) {
            char directory_name[directory_name_length];
            strncpy(directory_name, obj_file_name, directory_name_length - 1);
            directory_name[directory_name_length - 1] = '\0';
            size_t final_name_length =
                directory_name_length + strlen(file_name) + 1;
            char final_name[final_name_length];
            cwk_path_join(directory_name, file_name, final_name,
                          final_name_length);
            file = fopen(final_name, "r");
        } else {
            file = fopen(file_name, "r");
        }
    } else {
        file = fopen(file_name, "r");
    }
    if (file == NULL) {
        return;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *file_data = (char *)malloc(file_size + 1);
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

struct mesh *mesh_load(const char *file_name) {
    struct mesh *mesh;
    mesh = (struct mesh *)malloc(sizeof(struct mesh));
    if (mesh == NULL) {
        return NULL;
    }

    size_t directory_name_length;
    cwk_path_get_dirname(file_name, &directory_name_length);
    mesh->directory_name = (char *)malloc(directory_name_length + 1);
    if (mesh->directory_name == NULL) {
        free(mesh);
        return NULL;
    }
    if (directory_name_length > 0) {
        strncpy(mesh->directory_name, file_name, directory_name_length);
    }
    mesh->directory_name[directory_name_length] = '\0';

    void *context;
    int result =
        tinyobj_parse_obj(&mesh->attribute, &mesh->shapes, &mesh->shape_count,
                          &mesh->materials, &mesh->material_count, file_name,
                          get_file_data, &context, TINYOBJ_FLAG_TRIANGULATE);
    // The file parsing is complete, release the file data.
    free(context);
    if (result != TINYOBJ_SUCCESS) {
        free(mesh->directory_name);
        free(mesh);
        return NULL;
    }
    return mesh;
}

void mesh_free(struct mesh *mesh) {
    if (mesh != NULL) {
        tinyobj_attrib_free(&mesh->attribute);
        tinyobj_shapes_free(mesh->shapes, mesh->shape_count);
        tinyobj_materials_free(mesh->materials, mesh->material_count);
        free(mesh->directory_name);
        free(mesh);
    }
}

uint32_t mesh_triangle_count(const struct mesh *mesh) {
    return (uint32_t)mesh->attribute.num_face_num_verts;
}

bool mesh_get_vertex_position(vector3 *position, const struct mesh *mesh,
                              uint32_t triangle_index, uint32_t vertex_index) {
    uint32_t triangle_count = mesh_triangle_count(mesh);
    if (triangle_index >= triangle_count) {
        return false;
    }

    const tinyobj_attrib_t *attribute = &mesh->attribute;
    uint32_t offset = triangle_index * 3;
    const tinyobj_vertex_index_t *tinyobj_index =
        attribute->faces + offset + vertex_index;
    int index = tinyobj_index->v_idx * 3;
    position->x = attribute->vertices[index];
    position->y = attribute->vertices[index + 1];
    position->z = attribute->vertices[index + 2];
    return true;
}

bool mesh_get_texture_coordinates(vector2 *texture_coordinates,
                                  const struct mesh *mesh,
                                  uint32_t triangle_index,
                                  uint32_t vertex_index) {
    uint32_t triangle_count = mesh_triangle_count(mesh);
    if (triangle_index >= triangle_count) {
        return false;
    }

    const tinyobj_attrib_t *attribute = &mesh->attribute;
    if (attribute->num_texcoords <= 0) {
        return false;
    }

    uint32_t offset = triangle_index * 3;
    const tinyobj_vertex_index_t *tinyobj_index =
        attribute->faces + offset + vertex_index;
    if (tinyobj_index->vt_idx < 0) {
        return false;
    }
    int index = tinyobj_index->vt_idx * 2;
    texture_coordinates->x = attribute->texcoords[index];
    texture_coordinates->y = attribute->texcoords[index + 1];
    return true;
}

bool mesh_get_normal(vector3 *normal, const struct mesh *mesh,
                     uint32_t triangle_index, uint32_t vertex_index) {
    uint32_t triangle_count = mesh_triangle_count(mesh);
    if (triangle_index >= triangle_count) {
        return false;
    }

    const tinyobj_attrib_t *attribute = &mesh->attribute;
    if (attribute->num_normals > 0) {
        uint32_t offset = triangle_index * 3;
        const tinyobj_vertex_index_t *tinyobj_index =
            attribute->faces + offset + vertex_index;
        if (tinyobj_index->vn_idx >= 0) {
            int index = tinyobj_index->vn_idx * 3;
            normal->x = attribute->normals[index];
            normal->y = attribute->normals[index + 1];
            normal->z = attribute->normals[index + 2];
        } else {
            // Normal index is not defined for this triangle.
            // Compute normals.
            vector3 positions[3];
            for (uint32_t v = 0; v < 3; v++) {
                mesh_get_vertex_position(positions + v, mesh, triangle_index,
                                         v);
            }
            *normal = calculate_triangle_normal(positions);
        }
    } else {
        // Compute normals.
        vector3 positions[3];
        for (uint32_t v = 0; v < 3; v++) {
            mesh_get_vertex_position(positions + v, mesh, triangle_index, v);
        }
        *normal = calculate_triangle_normal(positions);
    }
    return true;
}

const char *mesh_get_directory_name(const struct mesh *mesh) {
    return mesh->directory_name;
}

const char *mesh_get_diffuse_texture_name(const struct mesh *mesh) {
    if (mesh->material_count > 0) {
        tinyobj_material_t *material = mesh->materials;
        return material->diffuse_texname;
    } else {
        return "";
    }
}
