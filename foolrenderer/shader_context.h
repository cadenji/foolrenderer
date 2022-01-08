// Copyright (c) Caden Ji. All rights reserved.
//
// Licensed under the MIT License. See LICENSE.txt in the project root for
// license information.

#ifndef FOOLRENDERER_SHADER_CONTEXT_H_
#define FOOLRENDERER_SHADER_CONTEXT_H_

#include <stdbool.h>
#include <stdint.h>

#include "math/vector.h"

#define MAX_FLOAT_VARIABLES 2
#define MAX_VECTOR2_VARIABLES 2
#define MAX_VECTOR3_VARIABLES 3
#define MAX_VECTOR4_VARIABLES 2

///
/// \brief Structure used to pass data between shaders in different stages.
///
/// The vertex shader stores floating-point-based data into this structure and
/// automatically interpolated on the surface of the triangle before the
/// fragment shader is executed. The interpolation result can be used in the
/// fragment shader.
///
/// IMPORTANT: Do not directly access the members of the structure in the
/// shader. Instead, use shader_context_*() functions.
///
struct shader_context {
    // Array to store various types of variables.
    float float_variables[MAX_FLOAT_VARIABLES];
    vector2 vector2_variables[MAX_VECTOR2_VARIABLES];
    vector3 vector3_variables[MAX_VECTOR3_VARIABLES];
    vector4 vector4_variables[MAX_VECTOR4_VARIABLES];
    // Record whether the variable at each index is used.
    bool float_allocations[MAX_FLOAT_VARIABLES];
    bool vector2_allocations[MAX_VECTOR2_VARIABLES];
    bool vector3_allocations[MAX_VECTOR3_VARIABLES];
    bool vector4_allocations[MAX_VECTOR4_VARIABLES];
    // Queue of indexes of variables that have been used.
    int8_t float_index_queue[MAX_FLOAT_VARIABLES];
    int8_t vector2_index_queue[MAX_VECTOR2_VARIABLES];
    int8_t vector3_index_queue[MAX_VECTOR3_VARIABLES];
    int8_t vector4_index_queue[MAX_VECTOR4_VARIABLES];
    // Number of variables used.
    int8_t float_variable_count;
    int8_t vector2_variable_count;
    int8_t vector3_variable_count;
    int8_t vector4_variable_count;
};

///
/// \brief Removes all data from the shader context and also serve as an
///        initialization function.
///
/// It should not be and is not necessary to use this function in the shader.
///
/// \param context The shader context object.
///
void clear_shader_context(struct shader_context *context);

///
/// \brief Gets the pointer of the float variable with the specified index in
///        the shader context.
///
/// The maximum number of float variables that can be used in the shader_context
/// is MAX_FLOAT_VARIABLES.
///
/// \param context The shader context object.
/// \param index The variable index, range from 0 to MAX_FLOAT_VARIABLES-1.
/// \return Returns variable pointer if successful. Returns NULL if contex is a
///         null pointer or index is out of range.
///
float *shader_context_float(struct shader_context *context, int8_t index);

///
/// \brief Gets the pointer of the vector2 variable with the specified index in
///        the shader context.
///
/// The maximum number of vector2 variables that can be used in the
/// shader_context is MAX_VECTOR2_VARIABLES.
///
/// \param context The shader context object.
/// \param index The variable index, range from 0 to MAX_VECTOR2_VARIABLES-1.
/// \return Returns variable pointer if successful. Returns NULL if contex is a
///         null pointer or index is out of range.
///
vector2 *shader_context_vector2(struct shader_context *context, int8_t index);

///
/// \brief Gets the pointer of the vector3 variable with the specified index in
///        the shader context.
///
/// The maximum number of vector3 variables that can be used in the
/// shader_context is MAX_VECTOR3_VARIABLES.
///
/// \param context The shader context object.
/// \param index The variable index, range from 0 to MAX_VECTOR3_VARIABLES-1.
/// \return Returns variable pointer if successful. Returns NULL if contex is a
///         null pointer or index is out of range.
///
vector3 *shader_context_vector3(struct shader_context *context, int8_t index);

///
/// \brief Gets the pointer of the vector4 variable with the specified index in
///        the shader context.
///
/// The maximum number of vector4 variables that can be used in the
/// shader_context is MAX_VECTOR4_VARIABLES.
///
/// \param context The shader context object.
/// \param index The variable index, range from 0 to MAX_VECTOR4_VARIABLES-1.
/// \return Returns variable pointer if successful. Returns NULL if contex is a
///         null pointer or index is out of range.
///
vector4 *shader_context_vector4(struct shader_context *context, int8_t index);

#endif  // FOOLRENDERER_SHADER_CONTEXT_H_
