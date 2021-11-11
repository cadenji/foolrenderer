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

#include "graphics.h"

#include <float.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "math/math_utility.h"
#include "math/vector.h"

#define MIN(a, b) (((a) < (b)) ? (a) : (b))

#define MAX(a, b) (((a) > (b)) ? (a) : (b))

static struct {
    int left, bottom;
    uint32_t width, height;
} viewport = {0};

// The vertices should be in clip space.
// Return true if the triangle needs to be discarded, otherwise returns false.
// This is just a rough method, if at least one vertex is outside the viewing
// volume, the entire triangle will be discarded.
static bool clipping_test(const vector4 vertices[]) {
    for (int v = 0; v < 3; v++) {
        float w = vertices[v].w;
        for (int c = 0; c < 3; c++) {
            float component = vertices[v].elements[c];
            if (component < -w || component > w) {
                return true;
            }
        }
    }
    return false;
}

// Transform vertex from clipping space to normalized device coordinates (NDC).
static inline void perspective_division(vector3 *vertex_ndc,
                                        const vector4 *vertex_clip) {
    float w = vertex_clip->w;
    vertex_ndc->x = vertex_clip->x / w;
    vertex_ndc->y = vertex_clip->y / w;
    vertex_ndc->z = vertex_clip->z / w;
}

// Transforms the vertex from NDC to window space.
static inline void viewport_transform(vector3 *vertex_window,
                                      const vector3 *vertex_ndc) {
    vertex_window->x =
        (vertex_ndc->x + 1.0f) * 0.5f * viewport.width + viewport.left;
    vertex_window->y =
        (vertex_ndc->y + 1.0f) * 0.5f * viewport.height + viewport.bottom;
    vertex_window->z = (vertex_ndc->z + 1.0f) * 0.5f;
}

// Computes the determinant of a 2x2 matrix composed of vectors (c-a) and (b-a).
// The result can be interpreted as the signed area of a parallelogram with the
// two vectors as sides. At the same time, the sign of the area can be used to
// determine the left-right relationship between the two vectors.
static inline float edge_function(vector2 a, vector2 b, vector2 c) {
    return (c.x - a.x) * (b.y - a.y) - (c.y - a.y) * (b.x - a.x);
}

static inline uint8_t *get_pixel(struct framebuffer *framebuffer, uint32_t x,
                                 uint32_t y) {
    return framebuffer->color_buffer + (y * framebuffer->width + x) * 3;
}

static inline float *get_depth(struct framebuffer *framebuffer, uint32_t x,
                               uint32_t y) {
    return framebuffer->depth_buffer + (y * framebuffer->width + x);
}

struct framebuffer *generate_framebuffer(uint32_t width, uint32_t height) {
    size_t buffer_dimension = (size_t)width * height;
    // Use RGB (3 bytes per pixel) pixel format.
    size_t color_buffer_size = buffer_dimension * 3;
    size_t depth_buffer_size = buffer_dimension * sizeof(float);

    uint8_t *color_buffer = (uint8_t *)malloc(color_buffer_size);
    float *depth_buffer = (float *)malloc(depth_buffer_size);
    if (color_buffer == NULL || depth_buffer == NULL) {
        free(color_buffer);
        free(depth_buffer);
        return NULL;
    }
    struct framebuffer *framebuffer;
    framebuffer = (struct framebuffer *)malloc(sizeof(struct framebuffer));
    if (framebuffer == NULL) {
        free(color_buffer);
        free(depth_buffer);
        return NULL;
    }

    framebuffer->width = width;
    framebuffer->height = height;
    framebuffer->color_buffer = color_buffer;
    framebuffer->depth_buffer = depth_buffer;
    clear_framebuffer(framebuffer);
    return framebuffer;
}

void clear_framebuffer(struct framebuffer *framebuffer) {
    size_t buffer_dimension = (size_t)framebuffer->width * framebuffer->height;
    size_t color_buffer_size = buffer_dimension * 3;
    memset(framebuffer->color_buffer, 0, color_buffer_size);
    for (size_t i = 0; i < buffer_dimension; i++) {
        framebuffer->depth_buffer[i] = 1.0f;
    }
}

void delete_framebuffer(struct framebuffer *framebuffer) {
    free(framebuffer->color_buffer);
    free(framebuffer->depth_buffer);
    free(framebuffer);
}

void set_viewport(int left, int bottom, uint32_t width, uint32_t height) {
    viewport.left = left;
    viewport.bottom = bottom;
    viewport.width = width;
    viewport.height = height;
}

// Using edge functions to raster triangles, refer to:
// https://www.scratchapixel.com/lessons/3d-basic-rendering/rasterization-practical-implementation/rasterization-stage
void draw_triangle(struct framebuffer *framebuffer, const vector4 vertices[],
                   const vector3 colors[]) {
    if (clipping_test(vertices)) {
        return;
    }
    vector3 vertex_array[3];
    for (int i = 0; i < 3; i++) {
        perspective_division(vertex_array + i, vertices + i);
        viewport_transform(vertex_array + i, vertex_array + i);
    }
    // Construct the bounding box of the triangle.
    vector2 bbmin = {{FLT_MAX, FLT_MAX}}, bbmax = {{FLT_MIN, FLT_MIN}};
    for (int i = 0; i < 3; i++) {
        bbmin.x = MIN(bbmin.x, vertex_array[i].x);
        bbmin.y = MIN(bbmin.y, vertex_array[i].y);
        bbmax.x = MAX(bbmax.x, vertex_array[i].x);
        bbmax.y = MAX(bbmax.y, vertex_array[i].y);
    }

    // The 2D coordinates of the vertex_array projected on the window.
    vector2 v0 = vector3_to_2(vertex_array[0]);
    vector2 v1 = vector3_to_2(vertex_array[1]);
    vector2 v2 = vector3_to_2(vertex_array[2]);

    // Compute the area of the triangle multiplied by 2.
    float area = edge_function(v0, v1, v2);
    if (area >= 0) {
        // If the area is 0, it means this is a degenerate triangle. If the area
        // is positive, the triangle with clockwise winding.
        // In both cases, the triangle does not need to be drawn.
        return;
    }

    // Traverse find the pixels covered by the triangle. If found, compute the
    // barycentric coordinates of the point in the triangle.
    vector2 p;
    // The barycentric coordinates of p.
    float b0, b1, b2;
    // The color of p.
    vector3 color_p;
    // The depth of p.
    float depth_p;
    // No need to traverses pixels outside the window.
    uint32_t x_min = MAX(0, MIN(framebuffer->width - 1, floorf(bbmin.x)));
    uint32_t y_min = MAX(0, MIN(framebuffer->height - 1, floorf(bbmin.y)));
    uint32_t x_max = MAX(0, MIN(framebuffer->width - 1, floorf(bbmax.x)));
    uint32_t y_max = MAX(0, MIN(framebuffer->height - 1, floorf(bbmax.y)));
    for (uint32_t y = y_min; y <= y_max; y++) {
        for (uint32_t x = x_min; x <= x_max; x++) {
            p = (vector2){{x, y}};
            b0 = edge_function(v1, v2, p);
            b1 = edge_function(v2, v0, p);
            b2 = edge_function(v0, v1, p);
            if (b0 <= 0 && b1 <= 0 && b2 <= 0) {
                b0 /= area;
                b1 /= area;
                b2 /= area;

                // Depth test.
                depth_p = b0 * vertex_array[0].z + b1 * vertex_array[1].z +
                          b2 * vertex_array[2].z;
                float *depth = get_depth(framebuffer, x, y);
                if (depth_p > *depth) {
                    continue;
                }
                *depth = depth_p;

                color_p.r =
                    b0 * colors[0].r + b1 * colors[1].r + b2 * colors[2].r;
                color_p.g =
                    b0 * colors[0].g + b1 * colors[1].g + b2 * colors[2].g;
                color_p.b =
                    b0 * colors[0].b + b1 * colors[1].b + b2 * colors[2].b;
                uint8_t *pixel = get_pixel(framebuffer, x, y);
                pixel[0] = color_p.r * 0xFF;
                pixel[1] = color_p.g * 0xFF;
                pixel[2] = color_p.b * 0xFF;
            }
        }
    }
}
