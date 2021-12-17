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

#include "framebuffer.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

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
