// Copyright (c) Caden Ji. All rights reserved.
//
// Licensed under the MIT License. See LICENSE.txt in the project root for
// license information.

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
