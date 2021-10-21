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

#include "math/math_utility.h"
#include "math/vector.h"

#define MIN(a, b) (((a) < (b)) ? (a) : (b))

#define MAX(a, b) (((a) > (b)) ? (a) : (b))

static struct {
    int left, bottom;
    uint32_t width, height;
} viewport = {0};

// Transforms the vertex position from normalized device coordinates (NDC) space
// to window space.
static inline vector3 viewport_transform(vector3 vertex) {
    vertex.x = (vertex.x + 1.0f) * 0.5f * viewport.width + viewport.left;
    // +Y is up in NDC space, but +Y is down in window space, so needs to flip
    // the Y axis from up to down.
    vertex.y = (-vertex.y + 1.0f) * 0.5f * viewport.height + viewport.bottom;
    vertex.z = (vertex.z + 1.0f) * 0.5f;
    return vertex;
}

// Computes the determinant of a 2x2 matrix composed of vectors (c-a) and (b-a).
// The result can be interpreted as the signed area of a parallelogram with the
// two vectors as sides. At the same time, the sign of the area can be used to
// determine the left-right relationship between the two vectors.
static inline float edge_function(vector2 a, vector2 b, vector2 c) {
    return (c.x - a.x) * (b.y - a.y) - (c.y - a.y) * (b.x - a.x);
}

static inline uint8_t *get_pixel(uint8_t *framebuffer, uint32_t x, uint32_t y) {
    return framebuffer + (y * viewport.width + x) * 3;
}

void set_viewport(int left, int bottom, uint32_t width, uint32_t height) {
    viewport.left = left;
    viewport.bottom = bottom;
    viewport.width = width;
    viewport.height = height;
}

void draw_triangle(const vector3 vertices[], uint8_t *framebuffer) {
    vector2 bbmin = {{FLT_MAX, FLT_MAX}}, bbmax = {{FLT_MIN, FLT_MIN}};
    vector3 transformed_vertices[3];
    for (int i = 0; i < 3; i++) {
        transformed_vertices[i] = viewport_transform(vertices[i]);
        // Construct the bounding box of the triangle.
        bbmin.x = MIN(bbmin.x, transformed_vertices[i].x);
        bbmin.y = MIN(bbmin.y, transformed_vertices[i].y);
        bbmax.x = MAX(bbmax.x, transformed_vertices[i].x);
        bbmax.y = MAX(bbmax.y, transformed_vertices[i].y);
    }

    // The 2D coordinates of the transformed_vertices projected on the window.
    vector2 v0 = vector3_to_2(transformed_vertices[0]);
    vector2 v1 = vector3_to_2(transformed_vertices[1]);
    vector2 v2 = vector3_to_2(transformed_vertices[2]);

    // Compute the area of the triangle multiplied by 2.
    float area = edge_function(v0, v1, v2);
    if (area < 0) {
        // Since the y-axis is flipped down when the vertex is transformed into
        // screen space. So the triangle on the front becomes a clockwise
        // winding and the area should be positive.
        return;
    }
    if (area < SMALL_ABSOLUTE_FLOAT) {
        // If the triangle area is too small, it is considered a degenerate
        // triangle. Won't draw.
        return;
    }

    // Traverse find the pixels covered by the triangle. If found, compute the
    // barycentric coordinates of the point in the triangle.
    vector2 p;
    // The barycentric coordinates of p.
    float b0, b1, b2;
    // No need to traverses pixels outside the window.
    uint32_t x_min = MAX(0, MIN(viewport.width - 1, floorf(bbmin.x)));
    uint32_t y_min = MAX(0, MIN(viewport.height - 1, floorf(bbmin.y)));
    uint32_t x_max = MAX(0, MIN(viewport.width - 1, floorf(bbmax.x)));
    uint32_t y_max = MAX(0, MIN(viewport.height - 1, floorf(bbmax.y)));
    for (uint32_t y = y_min; y <= y_max; y++) {
        for (uint32_t x = x_min; x <= x_max; x++) {
            p = (vector2){{x, y}};
            b0 = edge_function(v1, v2, p);
            b1 = edge_function(v2, v0, p);
            b2 = edge_function(v0, v1, p);
            if (b0 >= 0 && b1 >= 0 && b2 >= 0) {
                b0 /= area;
                b1 /= area;
                b2 /= area;
                uint8_t *pixel = get_pixel(framebuffer, x, y);
                pixel[0] = 0xFF; // Set the red channel to 255.
            }
        }
    }
}
