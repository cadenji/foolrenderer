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

#include "shader_context.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "math/vector.h"

#define RETURN_VARIABLE(type, max_variables)                      \
    do {                                                          \
        if (index >= max_variables) {                             \
            return NULL;                                          \
        }                                                         \
        type *variables = context->type##_variables;              \
        bool *allocations = context->type##_allocations;          \
        int8_t *index_queue = context->type##_index_queue;        \
        int8_t *variable_count = &context->type##_variable_count; \
        if (!allocations[index]) {                                \
            allocations[index] = true;                            \
            index_queue[*variable_count] = index;                 \
            ++(*variable_count);                                  \
        }                                                         \
        return variables + index;                                 \
    } while (0)

void clear_shader_context(struct shader_context *context) {
    for (int8_t i = 0; i < MAX_FLOAT_VARIABLES; i++) {
        context->float_allocations[i] = false;
    }
    for (int8_t i = 0; i < MAX_VECTOR2_VARIABLES; i++) {
        context->vector2_allocations[i] = false;
    }
    for (int8_t i = 0; i < MAX_VECTOR3_VARIABLES; i++) {
        context->vector3_allocations[i] = false;
    }
    for (int8_t i = 0; i < MAX_VECTOR4_VARIABLES; i++) {
        context->vector4_allocations[i] = false;
    }
    context->float_variable_count = 0;
    context->vector2_variable_count = 0;
    context->vector3_variable_count = 0;
    context->vector4_variable_count = 0;
}

float *shader_context_float(struct shader_context *context, int8_t index) {
    RETURN_VARIABLE(float, MAX_FLOAT_VARIABLES);
}

vector2 *shader_context_vector2(struct shader_context *context, int8_t index) {
    RETURN_VARIABLE(vector2, MAX_VECTOR2_VARIABLES);
}

vector3 *shader_context_vector3(struct shader_context *context, int8_t index) {
    RETURN_VARIABLE(vector3, MAX_VECTOR3_VARIABLES);
}

vector4 *shader_context_vector4(struct shader_context *context, int8_t index) {
    RETURN_VARIABLE(vector4, MAX_VECTOR4_VARIABLES);
}
