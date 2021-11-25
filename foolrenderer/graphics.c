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
#include "texture.h"

static struct {
    int left, bottom;
    uint32_t width, height;
} viewport = {0};

// The vertex positions should be in clip space.
// Return true if the triangle needs to be discarded, otherwise returns false.
// This is just a rough method, if at least one vertex is outside the viewing
// volume, the entire triangle will be discarded.
static bool clipping_test(const vector4 positions[]) {
    for (int v = 0; v < 3; v++) {
        float w = positions[v].w;
        for (int c = 0; c < 3; c++) {
            float component = positions[v].elements[c];
            if (component < -w || component > w) {
                return true;
            }
        }
    }
    return false;
}

// Transform vertex position from clipping space to normalized device
// coordinates (NDC). The ndc_position.w component becomes 1/clip_position.w,
// which is used for perspective correct interpolation.
static inline void perspective_division(vector4 *ndc_position,
                                        const vector4 *clip_position) {
    ndc_position->w = 1.0f / clip_position->w;
    ndc_position->x = clip_position->x * ndc_position->w;
    ndc_position->y = clip_position->y * ndc_position->w;
    ndc_position->z = clip_position->z * ndc_position->w;
}

// Transform the x and y components of position from the clipping space to the
// window space, transform the value range of the z component from [-1, 1] to
// [0, 1].
static inline void viewport_transform(vector4 *position) {
    position->x = (position->x + 1.0f) * 0.5f * viewport.width + viewport.left;
    position->y =
        (position->y + 1.0f) * 0.5f * viewport.height + viewport.bottom;
    position->z = (position->z + 1.0f) * 0.5f;
}

// Computes the determinant of a 2x2 matrix composed of vectors (c-a) and (b-a).
// The result can be interpreted as the signed area of a parallelogram with the
// two vectors as sides. At the same time, the sign of the area can be used to
// determine the left-right relationship between the two vectors.
static inline float edge_function(vector2 a, vector2 b, vector2 c) {
    return (c.x - a.x) * (b.y - a.y) - (c.y - a.y) * (b.x - a.x);
}

// For depth interpolation, refer to the OpenGL specification section 3.6.1
// equation 3.10:
// https://www.khronos.org/registry/OpenGL/specs/gl/glspec33.core.pdf
// For the purpose of reducing computational overhead, the calculated depth
// value is in the window space, and the depth value in this space is not
// linear. Although it is enough for depth testing.
static inline float interpolate_depth(const vector4 positions[],
                                      const float barycentric[]) {
    return barycentric[0] * positions[0].z + barycentric[1] * positions[1].z +
           barycentric[2] * positions[2].z;
}

// For the principle of perspective correct interpolation vertex attributes,
// refer to:
// https://www.comp.nus.edu.sg/~lowkl/publications/lowk_persp_interp_techrep.pdf
// The OpenGL specification section 3.6.1 provides the same calculation method,
// refer to equation 3.9:
// https://www.khronos.org/registry/OpenGL/specs/gl/glspec33.core.pdf
// This interpolation method is suitable for both perspective projection and
// orthogonal projection.
static float interpolate_attributes(const float attributes[],
                                    const float barycentric[],
                                    const float inverse_w[]) {
    float b_inverse_w[3];
    for (int i = 0; i < 3; i++) {
        b_inverse_w[i] = barycentric[i] * inverse_w[i];
    }
    float inverse_denominator =
        1 / (b_inverse_w[0] + b_inverse_w[1] + b_inverse_w[2]);
    float numerator = attributes[0] * b_inverse_w[0] +
                      attributes[1] * b_inverse_w[1] +
                      attributes[2] * b_inverse_w[2];
    return numerator * inverse_denominator;
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
void draw_triangle(struct framebuffer *framebuffer,
                   const vector4 vertex_positions[],
                   const vector2 texture_coordinates[],
                   const struct texture *diffuse_texture,
                   const float intensities[]) {
    if (clipping_test(vertex_positions)) {
        return;
    }
    vector4 positions[3];
    vector2 bbmin = {{FLT_MAX, FLT_MAX}}, bbmax = {{FLT_MIN, FLT_MIN}};
    for (int i = 0; i < 3; i++) {
        perspective_division(positions + i, vertex_positions + i);
        viewport_transform(positions + i);
        // Construct the bounding box of the triangle.
        bbmin.x = min_float(bbmin.x, positions[i].x);
        bbmin.y = min_float(bbmin.y, positions[i].y);
        bbmax.x = max_float(bbmax.x, positions[i].x);
        bbmax.y = max_float(bbmax.y, positions[i].y);
    }
    float inverse_w[3];
    for (int i = 0; i < 3; i++) {
        inverse_w[i] = positions[i].w;
    }
    vector2 positions_2d[3];
    for (int i = 0; i < 3; i++) {
        positions_2d[i] = (vector2){{positions[i].x, positions[i].y}};
    }

    // Compute the area of the triangle multiplied by 2.
    float area =
        edge_function(positions_2d[0], positions_2d[1], positions_2d[2]);
    if (area >= 0) {
        // If the area is 0, it means this is a degenerate triangle. If the area
        // is positive, the triangle with clockwise winding.
        // In both cases, the triangle does not need to be drawn.
        return;
    }
    float inverse_area = 1 / area;

    // Traverse find the pixels covered by the triangle. If found, compute the
    // barycentric coordinates of the point in the triangle.
    // No need to traverses pixels outside the window.
    uint32_t x_min = clamp_int(floorf(bbmin.x), 0, framebuffer->width - 1);
    uint32_t y_min = clamp_int(floorf(bbmin.y), 0, framebuffer->height - 1);
    uint32_t x_max = clamp_int(floorf(bbmax.x), 0, framebuffer->width - 1);
    uint32_t y_max = clamp_int(floorf(bbmax.y), 0, framebuffer->height - 1);
    for (uint32_t y = y_min; y <= y_max; y++) {
        for (uint32_t x = x_min; x <= x_max; x++) {
            vector2 p = (vector2){{x, y}};
            // The barycentric coordinates of p.
            float barycentric[3];
            barycentric[0] = edge_function(positions_2d[1], positions_2d[2], p);
            barycentric[1] = edge_function(positions_2d[2], positions_2d[0], p);
            barycentric[2] = edge_function(positions_2d[0], positions_2d[1], p);
            if (barycentric[0] > 0 || barycentric[1] > 0 ||
                barycentric[2] > 0) {
                // If any component of the barycentric coordinates is greater
                // than 0, it means that the pixel is outside the triangle.
                continue;
            }
            // Calculate the barycentric coordinates of point p.
            barycentric[0] *= inverse_area;
            barycentric[1] *= inverse_area;
            barycentric[2] *= inverse_area;
            // Depth test.
            float depth = interpolate_depth(positions, barycentric);
            float *depth_ptr = get_depth(framebuffer, x, y);
            if (depth > *depth_ptr) {
                continue;
            }
            *depth_ptr = depth;
            // Shading.
            float attributes[3];
            vector4 diffuse_color;
            if (diffuse_texture != NULL) {
                vector2 uv;
                for (int i = 0; i < 3; i++) {
                    attributes[i] = texture_coordinates[i].u;
                }
                uv.u =
                    interpolate_attributes(attributes, barycentric, inverse_w);
                for (int i = 0; i < 3; i++) {
                    attributes[i] = texture_coordinates[i].v;
                }
                uv.v =
                    interpolate_attributes(attributes, barycentric, inverse_w);
                texture_sample(&diffuse_color, diffuse_texture, uv);
            } else {
                diffuse_color = VECTOR4_ONE;
            }

            float intensity =
                interpolate_attributes(intensities, barycentric, inverse_w);

            vector3 color_p;
            color_p.r = diffuse_color.r * intensity;
            color_p.g = diffuse_color.g * intensity;
            color_p.b = diffuse_color.b * intensity;
            uint8_t *pixel = get_pixel(framebuffer, x, y);
            pixel[0] = color_p.r * 0xFF;
            pixel[1] = color_p.g * 0xFF;
            pixel[2] = color_p.b * 0xFF;
        }
    }
}
