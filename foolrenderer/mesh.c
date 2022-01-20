// Copyright (c) Caden Ji. All rights reserved.
//
// Licensed under the MIT License. See LICENSE.txt in the project root for
// license information.

#include "mesh.h"

#include <fast_obj.h>
#include <stdbool.h>
#include <stddef.h>

#include "math/vector.h"

struct mesh {
    fastObjMesh *m;
};

struct mesh *mesh_load(const char *filename) {
    struct mesh *mesh;
    mesh = (struct mesh *)malloc(sizeof(struct mesh));
    if (mesh == NULL) {
        return NULL;
    }

    fastObjMesh *obj = fast_obj_read(filename);

    if (obj != NULL) {
        // Failed to load mesh if mesh contains non-triangular faces.
        for (unsigned int f = 0; f < obj->face_count; f++) {
            unsigned int vertex_count = obj->face_vertices[f];
            // Faces with 0 vertices can be ignored directly.
            if (vertex_count != 0 && vertex_count != 3) {
                fast_obj_destroy(obj);
                obj = NULL;
                break;
            }
        }
    }
    if (obj == NULL) {
        free(mesh);
        return NULL;
    }
    mesh->m = obj;
    return mesh;
}

void mesh_free(struct mesh *mesh) {
    if (mesh != NULL) {
        fast_obj_destroy(mesh->m);
        free(mesh);
    }
}

uint32_t mesh_triangle_count(const struct mesh *mesh) {
    return mesh->m->face_count;
}

bool mesh_get_vertex_position(vector3 *position, const struct mesh *mesh,
                              uint32_t triangle_index, uint32_t vertex_index) {
    uint32_t triangle_count = mesh_triangle_count(mesh);
    if (triangle_index >= triangle_count || vertex_index > 2) {
        return false;
    }

    const fastObjIndex *fastobj_index =
        mesh->m->indices + triangle_index * 3 + vertex_index;
    uint32_t index = fastobj_index->p;
    if (index > mesh->m->position_count) {
        return false;
    }
    index *= 3;
    position->x = mesh->m->positions[index];
    position->y = mesh->m->positions[index + 1];
    position->z = mesh->m->positions[index + 2];
    return true;
}

bool mesh_get_texture_coordinates(vector2 *texture_coordinates,
                                  const struct mesh *mesh,
                                  uint32_t triangle_index,
                                  uint32_t vertex_index) {
    uint32_t triangle_count = mesh_triangle_count(mesh);
    if (triangle_index >= triangle_count || vertex_index > 2) {
        return false;
    }

    const fastObjIndex *fastobj_index =
        mesh->m->indices + triangle_index * 3 + vertex_index;
    uint32_t index = fastobj_index->t;
    if (index > mesh->m->texcoord_count) {
        return false;
    }
    index *= 2;
    texture_coordinates->u = mesh->m->texcoords[index];
    texture_coordinates->v = mesh->m->texcoords[index + 1];
    return true;
}

bool mesh_get_normal(vector3 *normal, const struct mesh *mesh,
                     uint32_t triangle_index, uint32_t vertex_index) {
    uint32_t triangle_count = mesh_triangle_count(mesh);
    if (triangle_index >= triangle_count || vertex_index > 2) {
        return false;
    }

    const fastObjIndex *fastobj_index =
        mesh->m->indices + triangle_index * 3 + vertex_index;
    uint32_t index = fastobj_index->n;
    if (index > mesh->m->normal_count) {
        return false;
    }
    index*=3;
    normal->x = mesh->m->normals[index];
    normal->y = mesh->m->normals[index + 1];
    normal->z = mesh->m->normals[index + 2];
    return true;
}

const char *mesh_get_diffuse_texture_name(const struct mesh *mesh) {
    if (mesh->m->material_count > 0) {
        return mesh->m->materials->map_Kd.path;
    } else {
        return "";
    }
}
