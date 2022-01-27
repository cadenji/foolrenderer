// Copyright (c) Caden Ji. All rights reserved.
//
// Licensed under the MIT License. See LICENSE.txt in the project root for
// license information.

#ifndef FOOLRENDERER_MESH_H_
#define FOOLRENDERER_MESH_H_

#include <stdint.h>

#include "math/vector.h"

///
/// The mesh is made of triangles, each triangle is defined by three vertex
/// indices. For example, a cube mesh has 12 triangles, then the indices array
/// length should be 36, with each value indicating which vertex to use. The
/// first three elements in the indices array are the indices of the vertices
/// that make up the triangle; the second three elements make up another
/// triangle, and so on.
///
/// For every vertex there can be a vertex position, texture coordinate
/// (texcoord), normal and tangent. These data are collectively called vertex
/// attributes. Vertex attributes other than position are optional.
//
/// All vertex attributes (if present) are stored in separate arrays of the same
/// size. For example, if a mesh has 100 vertices, and each vertex has position
/// and texcoord, then the mesh should have positions and texcoords arrays, each
/// being 100 in size. Data for i-th vertex is at index "i" in each array.
/// Pointers to other vertex attributes should point to null pointers.
///
/// If the mesh has no diffuse texture associated with it, diffuse_texture_path
/// points to a null pointer.
///
struct mesh {
    vector3 *positions;
    vector2 *texcoords;
    vector3 *normals;
    ///
    /// \brief The tangents of the mesh.
    ///
    /// Tangents are mostly used in normal mapping. A tangent is a unit vector
    /// that follows mesh surface along horizontal (u) texture direction. The
    /// type of tangents is vector4, with x, y, z components defining the
    /// vector, the w component is used to determine the direction of the
    /// bitangent when calculating the bitangent.
    ///
    /// In foolrenderer, calculate the bitangent by taking a cross product
    /// between the normal and the tangent, and multiplying the result by the w
    /// component of the tangent. Therefor, w should always be 1 or -1, if
    /// tangent exists.
    ///
    vector4 *tangents;
    uint32_t *indices;
    char *diffuse_texture_path;
    uint32_t vertex_count;
    uint32_t triangle_count;
};

///
/// \brief Load mesh data from .obj file.
///
/// Currently, only mesh consisting of triangular polygons are supported.
///
/// \param filename .obj filename.
/// \return Returns a pointer to the mesh object if the load was successful,
///         otherwise returns a null pointer.
///
struct mesh *mesh_load(const char *filename);

void mesh_release(struct mesh *mesh);

///
/// \brief Gets the position of a vertex in the mesh.
///
/// If the triangle or vertex index exceeds the range, returns zero vector.
///
/// \param position Pointer to the vector where the position is stored.
/// \param mesh The mesh to read.
/// \param triangle_index The index of the triangle, ranging from 0 to
///                       triangle_count-1.
/// \param vertex_index The index of the vertex in the triangle, ranging from 0
///                     to 2.
///
void mesh_get_position(vector3 *position, const struct mesh *mesh,
                       uint32_t triangle_index, uint32_t vertex_index);

///
/// \brief Gets the texcoord of a vertex in the mesh.
///
/// If the triangle or vertex index exceeds the range or if the texcoords are
/// not contained in the mesh, returns zero vector.
///
/// \param texcoord Pointer to the vector where the texcoord is stored.
/// \param mesh The mesh to read.
/// \param triangle_index The index of the triangle, ranging from 0 to
///                       triangle_count-1.
/// \param vertex_index The index of the vertex in the triangle, ranging from 0
///                     to 2.
///
void mesh_get_texcoord(vector2 *texcoord, const struct mesh *mesh,
                       uint32_t triangle_index, uint32_t vertex_index);

///
/// \brief Gets the normal of a vertex in the mesh.
///
/// If the triangle or vertex index exceeds the range or if the normals are not
/// contained in the mesh, returns zero vector.
///
/// \param normal Pointer to the vector where the normal is stored.
/// \param mesh The mesh to read.
/// \param triangle_index The index of the triangle, ranging from 0 to
///                       triangle_count-1.
/// \param vertex_index The index of the vertex in the triangle, ranging from 0
///                     to 2.
///
void mesh_get_normal(vector3 *normal, const struct mesh *mesh,
                     uint32_t triangle_index, uint32_t vertex_index);

///
/// \brief Gets the tangent of a vertex in the mesh.
///
/// If the triangle or vertex index exceeds the range or if the tangents are not
/// contained in the mesh, returns zero vector.
/// 
/// \param tangent Pointer to the vector where the tangent is stored.
/// \param mesh The mesh to read.
/// \param triangle_index The index of the triangle, ranging from 0 to
///                       triangle_count-1.
/// \param vertex_index The index of the vertex in the triangle, ranging from 0
///                     to 2.
///
void mesh_get_tangent(vector4 *tangent, const struct mesh *mesh,
                      uint32_t triangle_index, uint32_t vertex_index);

#endif  // FOOLRENDERER_MESH_H_
