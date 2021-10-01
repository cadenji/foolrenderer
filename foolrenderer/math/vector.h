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

#ifndef FOOLRENDERER_MATH_VECTOR_H_
#define FOOLRENDERER_MATH_VECTOR_H_

#include <math.h>

#include "math/math_utility.h"

/// A vector in 2D space composed of components (x, y) with float value.
/// The x, y components can be accessed using u, v respectively.
/// Or access in an array through the member: elements.
typedef union vector2 {
    struct {
        float x, y;
    };
    struct {
        float u, v;
    };
    float elements[2];
} vector2;

/// A vector in 3D space composed of components (x, y, z) with float value.
/// The x, y, z components can also be accessed access in an array through the
/// member: elements.
typedef union vector3 {
    struct {
        float x, y, z;
    };
    float elements[3];
} vector3;

/// A vector in 4D space composed of components (x, y, z, w) with float value.
/// The x, y, z, w components can also be accessed access in an array through
/// the member: elements.
typedef union vector4 {
    struct {
        float x, y, z, w;
    };
    float elements[4];
} vector4;

/// Constant zero vector (0,0).
#define VECTOR2_ZERO ((const vector2){0.0f, 0.0f})

/// Constant one vector (1,1).
#define VECTOR2_ONE ((const vector2){1.0f, 1.0f})

/// Constant zero vector (0,0,0).
#define VECTOR3_ZERO ((const vector3){0.0f, 0.0f, 0.0f})

/// Constant one vector (1,1,1).
#define VECTOR3_ONE ((const vector3){1.0f, 1.0f, 1.0f})

/// Constant zero vector (0,0,0,0).
#define VECTOR4_ZERO ((const vector4){0.0f, 0.0f, 0.0f, 0.0f})

// Constant one vector (1,1,1,1).
#define VECTOR4_ONE ((const vector4){1.0f, 1.0f, 1.0f, 1.0f})

////////////////////////////////////////////////////////////////////////////////
//
// Construct vector utilities.
//
////////////////////////////////////////////////////////////////////////////////

/// Construct a 2D vector from a 3D vector.
/// The 3D vector provides x and y compnent values.
///
/// \param v Vector to copy from.
/// \return The constructed 2D vector.
inline vector2 vector3_to_2(vector3 v) { return (vector2){v.x, v.y}; }

/// Construct a 3D vector from a 2D vector and z value.
///
/// \param v The vector provides x and y component values.
/// \param z The z component value.
/// \return The constructed 3D vector.
inline vector3 vector2_to_3(vector2 v, float z) {
    return (vector3){v.x, v.y, z};
}

/// Construct a 3D vector from a 4D vector.
///
/// \param v The vector provides x, y and z component values.
/// \return The constructed 3D vector.
inline vector3 vecotr4_to_3(vector4 v) { return (vector3){v.x, v.y, v.z}; }

/// Construct a 4D vector from a 3D vector and w value.
///
/// \param v The vector provides x, y and z component values.
/// \param w The w component value.
/// \return The constructed 4D vector.
inline vector4 vector3_to_4(vector3 v, float w) {
    return (vector4){v.x, v.y, v.z, w};
}

////////////////////////////////////////////////////////////////////////////////
//
// 2D vector functions.
//
////////////////////////////////////////////////////////////////////////////////

inline vector2 vector2_add(vector2 v1, vector2 v2) {
    return (vector2){v1.x + v2.x, v1.y + v2.y};
}

inline vector2 vector2_subtract(vector2 v1, vector2 v2) {
    return (vector2){v1.x - v2.x, v1.y - v2.y};
}

/// Get the result of multiplying each component of the 2D vector by a value.
///
/// \param v The multiplied vector.
/// \param scalar What to multiply each component by.
/// \return The result of multiplication.
inline vector2 vector2_multiply(vector2 v, float scalar) {
    return (vector2){v.x * scalar, v.y * scalar};
}

/// Get the result of dividing each component of the 2D vector by a value.
///
/// \param v The divided vector.
/// \param scalar What to divide each component by.
/// \return The result of division.
inline vector2 vector2_divide(vector2 v, float scalar) {
    return (vector2){v.x / scalar, v.y / scalar};
}

inline float vector2_dot(vector2 v1, vector2 v2) {
    return v1.x * v2.x + v1.y * v2.y;
}

/// Get the lenght of a 2D vector.
///
/// If only need to compare lenght of some vectors, using
/// vector2_magnitude_squared(vector2) instead. Because this function
/// doesn't perform square root, the execution speed is faster.
inline float vector2_magnitude(vector2 v) { return sqrtf(vector2_dot(v, v)); }

/// Get the squared lenght of a 2D vector.
///
/// See also vector2_magnitude(vector2).
inline float vector2_magnitude_squared(vector2 v) { return vector2_dot(v, v); }

/// Get a normalized copy of the 2D vector.
/// Return a zero vector if the vector is too small to be normalized.
///
/// \param v The vector to be normalized.
/// \return A normalized copy if the vector is not too small, zero vector
///         otherwise.
inline vector2 vector2_normalize(vector2 v) {
    float square_magnitude = vector2_magnitude_squared(v);
    if (square_magnitude == 1.0f) {
        return v;
    } else if (square_magnitude < SMALL_ABSOLUTE_FLOAT) {
        return VECTOR2_ZERO;
    }
    return vector2_divide(v, sqrtf(square_magnitude));
}

////////////////////////////////////////////////////////////////////////////////
//
// 3D vector functions.
//
////////////////////////////////////////////////////////////////////////////////

inline vector3 vector3_add(vector3 v1, vector3 v2) {
    return (vector3){v1.x + v2.x, v1.y + v2.y, v1.z + v2.z};
}

inline vector3 vector3_subtract(vector3 v1, vector3 v2) {
    return (vector3){v1.x - v2.x, v1.y - v2.y, v1.z - v2.z};
}

/// Get the result of multiplying each component of the 3D vector by a value.
///
/// \param v The multiplied vector.
/// \param scalar What to multiply each component by.
/// \return The result of multiplication.
inline vector3 vector3_multiply(vector3 v, float scalar) {
    return (vector3){v.x * scalar, v.y * scalar, v.z * scalar};
}

/// Get the result of dividing each component of the 3D vector by a value.
///
/// \param v The divided vector.
/// \param scalar What to divide each component by.
/// \return The result of division.
inline vector3 vector3_divide(vector3 v, float scalar) {
    return (vector3){v.x / scalar, v.y / scalar, v.z / scalar};
}

inline float vector3_dot(vector3 v1, vector3 v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

inline vector3 vector3_cross(vector3 v1, vector3 v2) {
    vector3 v_out;
    v_out.x = v1.y * v2.z - v1.z * v2.y;
    v_out.y = v1.z * v2.x - v1.x * v2.z;
    v_out.z = v1.x * v2.y - v1.y * v2.x;
    return v_out;
}

/// Get the lenght of a 3D vector.
///
/// If only need to compare lenght of some vectors, using
/// vector3_magnitude_squared(vector3) instead. Because this function
/// doesn't perform square root, the execution speed is faster.
inline float vector3_magnitude(vector3 v) { return sqrtf(vector3_dot(v, v)); }

/// Get the squared lenght of a 3D vector.
///
/// See also vector3_magnitude(vector3).
inline float vector3_magnitude_squared(vector3 v) { return vector3_dot(v, v); }

/// Get a normalized copy of the 3D vector.
/// Return a zero vector if the vector is too small to be normalized.
///
/// \param v The vector to be normalized.
/// \return A normalized copy if the vector is not too small, zero vector
///         otherwise.
inline vector3 vector3_normalize(vector3 v) {
    float square_magnitude = vector3_magnitude_squared(v);
    if (square_magnitude == 1.0f) {
        return v;
    } else if (square_magnitude < SMALL_ABSOLUTE_FLOAT) {
        return VECTOR3_ZERO;
    }
    return vector3_divide(v, sqrtf(square_magnitude));
}

////////////////////////////////////////////////////////////////////////////////
//
// 4D vector functions.
//
////////////////////////////////////////////////////////////////////////////////

inline vector4 vector4_add(vector4 v1, vector4 v2) {
    return (vector4){v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w};
}

inline vector4 vector4_subtract(vector4 v1, vector4 v2) {
    return (vector4){v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w};
}

/// Get the result of multiplying each component of the 4D vector by a value.
///
/// \param v The multiplied vector.
/// \param scalar What to multiply each component by.
/// \return The result of multiplication.
inline vector4 vector4_multiply(vector4 v, float scalar) {
    return (vector4){v.x * scalar, v.y * scalar, v.z * scalar, v.w * scalar};
}

/// Get the result of dividing each component of the 4D vector by a value.
///
/// \param v The divided vector.
/// \param scalar What to divide each component by.
/// \return The result of division.
inline vector4 vector4_divide(vector4 v, float scalar) {
    return (vector4){v.x / scalar, v.y / scalar, v.z / scalar, v.w / scalar};
}

#endif  // FOOLRENDERER_MATH_VECTOR_H_
