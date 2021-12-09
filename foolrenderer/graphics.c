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
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "math/math_utility.h"
#include "math/vector.h"
#include "shader_context.h"
#include "texture.h"

struct vertex {
    struct shader_context context;
    vector4 position;
    vector2 position_window;
    float depth;
    // The inverse of the w component of the vertex position in the clipping
    // space. Will be used for perspective correct interpolation.
    float inverse_w;
};

struct bounding_box {
    vector2 min, max;
};

static struct {
    int left, bottom;
    uint32_t width, height;
} viewport = {0};

static vertex_shader vs = NULL;
static fragment_shader fs = NULL;

// The vertex position should be in clippig space.
// Returns true if the vertex needs to be clipped, otherwise returns false.
static bool clipping_test(const struct vertex *vertex) {
    float w = vertex->position.w;
    for (int c = 0; c < 3; c++) {
        float component = vertex->position.elements[c];
        if (component < -w || component > w) {
            return true;
        }
    }
    return false;
}

// Transform vertex position from clipping space to normalized device
// coordinates (NDC).
static inline void perspective_division(struct vertex *vertex) {
    vector4 *position = &vertex->position;
    float inverse_w = 1.0f / position->w;
    vertex->inverse_w = inverse_w;
    position->x *= inverse_w;
    position->y *= inverse_w;
    position->z *= inverse_w;
    position->w = 1.0f;
}

// Transform the x and y components of position from the NDC to the window
// space, transform the value range of the z component from [-1, 1] to [0, 1].
static inline void viewport_transform(struct vertex *vertex) {
    vector4 *position = &vertex->position;
    vector2 *position_window = &vertex->position_window;
    position_window->x =
        (position->x + 1.0f) * 0.5f * viewport.width + viewport.left;
    position_window->y =
        (position->y + 1.0f) * 0.5f * viewport.height + viewport.bottom;
    vertex->depth = (position->z + 1.0f) * 0.5f;
}

static inline void update_bounding_box(struct bounding_box *bound,
                                       const struct vertex *vertex) {
    const vector2 *position = &vertex->position_window;
    vector2 *min = &bound->min;
    vector2 *max = &bound->max;
    min->x = min_float(min->x, position->x);
    min->y = min_float(min->y, position->y);
    max->x = max_float(max->x, position->x);
    max->y = max_float(max->y, position->y);
}

// Computes the determinant of a 2x2 matrix composed of vectors (c-a) and (b-a).
// The result can be interpreted as the signed area of a parallelogram with the
// two vectors as sides. At the same time, the sign of the area can be used to
// determine the left-right relationship between the two vectors.
static inline float edge_function(const vector2 *a, const vector2 *b,
                                  const vector2 *c) {
    return (c->x - a->x) * (b->y - a->y) - (c->y - a->y) * (b->x - a->x);
}

// Returns true if the fragment is hidden.
static inline bool depth_test(struct framebuffer *framebuffer, uint32_t x,
                              uint32_t y, const struct vertex vertices[],
                              const float barycentric[]) {
    // Interpolate depth, for more details refer to the OpenGL specification
    // section 3.6.1 equation 3.10:
    // https://www.khronos.org/registry/OpenGL/specs/gl/glspec33.core.pdf
    // For the purpose of reducing computational overhead, the calculated depth
    // value is in the window space, and the depth value in this space is not
    // linear. Although it is enough for depth testing.
    float new_depth = barycentric[0] * vertices[0].depth +
                      barycentric[1] * vertices[1].depth +
                      barycentric[2] * vertices[2].depth;
    // Get depth from framebuffer.
    float *depth = framebuffer->depth_buffer + (y * framebuffer->width + x);
    bool is_hidden = new_depth > *depth;
    if (!is_hidden) {
        *depth = new_depth;
    }
    return is_hidden;
}

// For the principle of perspective correct interpolation vertex variables,
// refer to:
// https://www.comp.nus.edu.sg/~lowkl/publications/lowk_persp_interp_techrep.pdf
//
// The OpenGL specification section 3.6.1 provides the same calculation method,
// refer to equation 3.9:
// https://www.khronos.org/registry/OpenGL/specs/gl/glspec33.core.pdf
// This interpolation method is suitable for both perspective projection and
// orthogonal projection.
static void interpolate_variables(float *result, const float *const sources[],
                                  size_t component_count,
                                  float inverse_denominator,
                                  const float bc_over_w[]) {
    const float *s0 = sources[0];
    const float *s1 = sources[1];
    const float *s2 = sources[2];
    for (size_t i = 0; i < component_count; i++) {
        float numerator =
            s0[i] * bc_over_w[0] + s1[i] * bc_over_w[1] + s2[i] * bc_over_w[2];
        result[i] = numerator * inverse_denominator;
    }
}

#define INTERPOLATION_HELPER(type, component_count)                     \
    do {                                                                \
        variable_count = vertices[0].context.type##_variable_count;     \
        for (int8_t i = 0; i < variable_count; i++) {                   \
            int8_t index = vertices[0].context.type##_index_queue[i];   \
            variables[0] =                                              \
                (float *)(vertices[0].context.type##_variables + i);    \
            variables[1] =                                              \
                (float *)(vertices[1].context.type##_variables + i);    \
            variables[2] =                                              \
                (float *)(vertices[2].context.type##_variables + i);    \
            float *interpolate_result =                                 \
                (float *)shader_context_##type(result, index);          \
            interpolate_variables(interpolate_result, variables,        \
                                  component_count, inverse_denominator, \
                                  bc_over_w);                           \
        }                                                               \
    } while (0)

static void set_fragment_shader_input(struct shader_context *result,
                                      const struct vertex vertices[],
                                      const float barycentric[]) {
    float bc_over_w[3];
    for (int i = 0; i < 3; i++) {
        bc_over_w[i] = barycentric[i] * vertices[i].inverse_w;
    }
    float inverse_denominator =
        1.0f / (bc_over_w[0] + bc_over_w[1] + bc_over_w[2]);

    const float *variables[3];
    int8_t variable_count;
    INTERPOLATION_HELPER(float, 1);
    INTERPOLATION_HELPER(vector2, 2);
    INTERPOLATION_HELPER(vector3, 3);
    INTERPOLATION_HELPER(vector4, 4);
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

void set_vertex_shader(vertex_shader shader) { vs = shader; }

void set_fragment_shader(fragment_shader shader) { fs = shader; }

// Using edge functions to raster triangles, refer to:
// https://www.scratchapixel.com/lessons/3d-basic-rendering/rasterization-practical-implementation/rasterization-stage
void draw_triangle(struct framebuffer *framebuffer, const void *uniform,
                   const void *const vertex_attributes[]) {
    if (vs == NULL || fs == NULL) {
        return;
    }
    struct vertex vertices[3];
    // The bounding box of the triangle.
    struct bounding_box bound = {{{FLT_MAX, FLT_MAX}}, {{FLT_MIN, FLT_MIN}}};
    for (int i = 0; i < 3; i++) {
        struct vertex *vertex = vertices + i;
        clear_shader_context(&vertex->context);
        vertex->position = vs(&vertex->context, uniform, vertex_attributes[i]);
        // Perform a rough clipping test, if at least one vertex is outside the
        // viewing volume, the entire triangle will be discarded.
        if (clipping_test(vertex)) {
            return;
        }
        perspective_division(vertex);
        viewport_transform(vertex);
        update_bounding_box(&bound, vertex);
    }
    // Compute the area of the triangle multiplied by 2.
    float area = edge_function(&vertices[0].position_window,
                               &vertices[1].position_window,
                               &vertices[2].position_window);
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
    uint32_t x_min = clamp_int(floorf(bound.min.x), 0, framebuffer->width - 1);
    uint32_t y_min = clamp_int(floorf(bound.min.y), 0, framebuffer->height - 1);
    uint32_t x_max = clamp_int(floorf(bound.max.x), 0, framebuffer->width - 1);
    uint32_t y_max = clamp_int(floorf(bound.max.y), 0, framebuffer->height - 1);

    for (uint32_t y = y_min; y <= y_max; y++) {
        for (uint32_t x = x_min; x <= x_max; x++) {
            vector2 p = (vector2){{x, y}};
            // The barycentric coordinates of p.
            float bc[3];
            // Note that this is not the final barycentric coordinates.
            bc[0] = edge_function(&vertices[1].position_window,
                                  &vertices[2].position_window, &p);
            bc[1] = edge_function(&vertices[2].position_window,
                                  &vertices[0].position_window, &p);
            bc[2] = edge_function(&vertices[0].position_window,
                                  &vertices[1].position_window, &p);
            if (bc[0] > 0.0f || bc[1] > 0.0f || bc[2] > 0.0f) {
                // If any component of the barycentric coordinates is greater
                // than 0, it means that the pixel is outside the triangle.
                continue;
            }
            // Calculate the barycentric coordinates of point p.
            bc[0] *= inverse_area;
            bc[1] *= inverse_area;
            bc[2] *= inverse_area;

            if (depth_test(framebuffer, x, y, vertices, bc)) {
                continue;
            }
            struct shader_context input;
            set_fragment_shader_input(&input, vertices, bc);
            vector4 fragment_color = fs(&input, uniform);

            uint8_t *pixel =
                framebuffer->color_buffer + (y * framebuffer->width + x) * 3;
            pixel[0] = clamp01_float(fragment_color.r) * 0xFF;
            pixel[1] = clamp01_float(fragment_color.g) * 0xFF;
            pixel[2] = clamp01_float(fragment_color.b) * 0xFF;
        }
    }
}
