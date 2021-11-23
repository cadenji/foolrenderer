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

#ifndef FOOLRENDERER_MESH_H_
#define FOOLRENDERER_MESH_H_

#include <stdbool.h>
#include <stdint.h>

#include "math/vector.h"

struct mesh;

struct mesh *mesh_load(const char *file_name);

void mesh_free(struct mesh *mesh);

uint32_t mesh_triangle_count(const struct mesh *mesh);

///
/// \brief Gets the vertex positions of a triangle in the mesh.
///
/// If the triangle index value exceeds the range, do nothing.
///
/// \param positions The array of vertex positions to be saved, the length is 3.
/// \param mesh The mesh object.
/// \param triangle_index The index of the triangle, ranging from 0 to
///                       triangle_count-1.
/// \return Returns true if gets positions successful. Returns false if the
///         triangle index value exceeds the range.
///
bool mesh_get_vertex_positions(vector3 positions[], const struct mesh *mesh,
                               uint32_t triangle_index);

///
/// \brief Gets the texture coordinates of the three vertices of a triangle in
///        the mesh.
///
/// If the triangle index value exceeds the range or texture coordinates are not
/// included in the triangle, do nothing.
///
/// \param texture_coordinates The array of texture coordinates to be saved, the
///                            length is 3.
/// \param mesh The mesh object.
/// \param triangle_index The index of the triangle, ranging from 0 to
///                       triangle_count-1.
/// \return Returns true if successful. Returns false if the triangle index
///         value exceeds the range or texture coordinates are not included in
///         the triangle.
///
bool mesh_get_texture_coordinates(vector2 texture_coordinates[],
                                  const struct mesh *mesh,
                                  uint32_t triangle_index);

///
/// \brief Gets the normals of the three vertices of a triangle in the mesh.
//
/// If the triangle index value exceeds the range, do nothing. If the index
/// value is valid and the mesh itself does not contain normal data, the normal
/// will be calculated based on the vertex position.
///
/// \param normals The array of normals to be saved, the length is 3.
/// \param mesh The mesh object.
/// \param triangle_index The index of the triangle, ranging from 0 to
///                       triangle_count-1.
/// \return Returns true if successful. Returns false if the triangle index
///         value exceeds the range.
///
bool mesh_get_normals(vector3 normals[], const struct mesh *mesh,
                      uint32_t triangle_index);

///
/// \brief Gets the directory where the model file is located when the mesh is
///        loaded.
///
/// \param mesh The mesh object.
/// \return The directory name. If no directory name, returns an empty string.
///
const char *mesh_get_directory_name(const struct mesh *mesh);

///
/// \brief Gets the file name of the diffuse texture map of the mesh object.
///
/// \param mesh The mesh object.
/// \return The file name of the diffuse texture map. If not exist, returns an
///         empty string.
///
const char *mesh_get_diffuse_texture_name(const struct mesh *mesh);

#endif  // FOOLRENDERER_MESH_H_
