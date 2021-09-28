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

/// A vector in 2D space composed of components (x, y) with float value.
struct vector2 {
    float x, y;
};

/// A vector in 3D space composed of components (x, y, z) with float value.
struct vector3 {
    float x, y, z;
};

/// A vector in 4D space composed of components (x, y, z, w) with float value.
struct vector4 {
    float x, y, z, w;
};

/// Constant zero vector (0,0).
#define VECTOR2_ZERO ((const struct vector2){0.0f, 0.0f})

/// Constant one vector (1,1).
#define VECTOR2_ONE ((const struct vector2){1.0f, 1.0f})

/// Constant zero vector (0,0,0).
#define VECTOR3_ZERO ((const struct vector3){0.0f, 0.0f, 0.0f})

/// Constant one vector (1,1,1).
#define VECTOR3_ONE ((const struct vector3){1.0f, 1.0f, 1.0f})

/// Constant zero vector (0,0,0,0).
#define VECTOR4_ZERO ((const struct vector4){0.0f, 0.0f, 0.0f, 0.0f})

// Constant one vector (1,1,1,1).
#define VECTOR4_ONE ((const struct vector4){1.0f, 1.0f, 1.0f, 1.0f})

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
inline struct vector2 vector3_to_2(struct vector3 v) {
    return (struct vector2){v.x, v.y};
}

/// Construct a 3D vector from a 2D vector and z value.
///
/// \param v The vector provides x and y component values.
/// \param z The z component value.
/// \return The constructed 3D vector.
inline struct vector3 vector2_to_3(struct vector2 v, float z) {
    return (struct vector3){v.x, v.y, z};
}

/// Construct a 3D vector from a 4D vector.
///
/// \param v The vector provides x, y and z component values.
/// \return The constructed 3D vector.
inline struct vector3 vecotr4_to_3(struct vector4 v) {
    return (struct vector3){v.x, v.y, v.z};
}

/// Construct a 4D vector from a 3D vector and w value.
///
/// \param v The vector provides x, y and z component values.
/// \param w The w component value.
/// \return The constructed 4D vector.
inline struct vector4 vector3_to_4(struct vector3 v, float w) {
    return (struct vector4){v.x, v.y, v.z, w};
}

////////////////////////////////////////////////////////////////////////////////
//
// 2D vector functions.
//
////////////////////////////////////////////////////////////////////////////////

inline struct vector2 vector2_add(struct vector2 v1, struct vector2 v2) {
    return (struct vector2){v1.x + v2.x, v1.y + v2.y};
}

inline struct vector2 vector2_subtract(struct vector2 v1, struct vector2 v2) {
    return (struct vector2){v1.x - v2.x, v1.y - v2.y};
}

/// Get the result of multiplying each component of the 2D vector by a value.
///
/// \param v The multiplied vector.
/// \param scalar What to multiply each component by.
/// \return The result of multiplication.
inline struct vector2 vector2_multiply(struct vector2 v, float scalar) {
    return (struct vector2){v.x * scalar, v.y * scalar};
}

/// Get the result of dividing each component of the 2D vector by a value.
///
/// \param v The divided vector.
/// \param scalar What to divide each component by.
/// \return The result of division.
inline struct vector2 vector2_divide(struct vector2 v, float scalar) {
    return (struct vector2){v.x / scalar, v.y / scalar};
}

inline float vector2_dot(struct vector2 v1, struct vector2 v2) {
    return v1.x * v2.x + v1.y * v2.y;
}

/// Get the lenght of a 2D vector.
///
/// If only need to compare lenght of some vectors, using
/// vector2_magnitude_squared(struct vector2) instead. Because this function
/// doesn't perform square root, the execution speed is faster.
inline float vector2_magnitude(struct vector2 v) {
    return sqrtf(vector2_dot(v, v));
}

/// Get the squared lenght of a 2D vector.
///
/// See also vector2_magnitude(struct vector2).
inline float vector2_magnitude_squared(struct vector2 v) {
    return vector2_dot(v, v);
}

inline struct vector2 vector2_normalize(struct vector2 v) {
    return vector2_divide(v, vector2_magnitude(v));
}

////////////////////////////////////////////////////////////////////////////////
//
// 3D vector functions.
//
////////////////////////////////////////////////////////////////////////////////

inline struct vector3 vector3_add(struct vector3 v1, struct vector3 v2) {
    return (struct vector3){v1.x + v2.x, v1.y + v2.y, v1.z + v2.z};
}

inline struct vector3 vector3_subtract(struct vector3 v1, struct vector3 v2) {
    return (struct vector3){v1.x - v2.x, v1.y - v2.y, v1.z - v2.z};
}

/// Get the result of multiplying each component of the 3D vector by a value.
///
/// \param v The multiplied vector.
/// \param scalar What to multiply each component by.
/// \return The result of multiplication.
inline struct vector3 vector3_multiply(struct vector3 v, float scalar) {
    return (struct vector3){v.x * scalar, v.y * scalar, v.z * scalar};
}

/// Get the result of dividing each component of the 3D vector by a value.
///
/// \param v The divided vector.
/// \param scalar What to divide each component by.
/// \return The result of division.
inline struct vector3 vector3_divide(struct vector3 v, float scalar) {
    return (struct vector3){v.x / scalar, v.y / scalar, v.z / scalar};
}

inline float vector3_dot(struct vector3 v1, struct vector3 v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

inline struct vector3 vector3_cross(struct vector3 v1, struct vector3 v2) {
    struct vector3 v_out;
    v_out.x = v1.y * v2.z - v1.z * v2.y;
    v_out.y = v1.z * v2.x - v1.x * v2.z;
    v_out.z = v1.x * v2.y - v1.y * v2.x;
    return v_out;
}

/// Get the lenght of a 3D vector.
///
/// If only need to compare lenght of some vectors, using
/// vector3_magnitude_squared(struct vector3) instead. Because this function
/// doesn't perform square root, the execution speed is faster.
inline float vector3_magnitude(struct vector3 v) {
    return sqrtf(vector3_dot(v, v));
}

/// Get the squared lenght of a 3D vector.
///
/// See also vector3_magnitude(struct vector3).
inline float vector3_magnitude_squared(struct vector3 v) {
    return vector3_dot(v, v);
}

inline struct vector3 vector3_normalize(struct vector3 v) {
    return vector3_divide(v, vector3_magnitude(v));
}

////////////////////////////////////////////////////////////////////////////////
//
// 4D vector functions.
//
////////////////////////////////////////////////////////////////////////////////

inline struct vector4 vector4_add(struct vector4 v1, struct vector4 v2) {
    return (struct vector4){v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w};
}

inline struct vector4 vector4_subtract(struct vector4 v1, struct vector4 v2) {
    return (struct vector4){v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w};
}

/// Get the result of multiplying each component of the 4D vector by a value.
///
/// \param v The multiplied vector.
/// \param scalar What to multiply each component by.
/// \return The result of multiplication.
inline struct vector4 vector4_multiply(struct vector4 v, float scalar) {
    return (struct vector4){v.x * scalar, v.y * scalar, v.z * scalar,
                            v.w * scalar};
}

/// Get the result of dividing each component of the 4D vector by a value.
///
/// \param v The divided vector.
/// \param scalar What to divide each component by.
/// \return The result of division.
inline struct vector4 vector4_divide(struct vector4 v, float scalar) {
    return (struct vector4){v.x / scalar, v.y / scalar, v.z / scalar,
                            v.w / scalar};
}

#endif  // FOOLRENDERER_MATH_VECTOR_H_
