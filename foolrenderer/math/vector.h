// Copyright (c) Caden Ji. All rights reserved.
//
// Licensed under the MIT License. See LICENSE file in the project root for
// license information.

#ifndef FOOLRENDERER_MATH_VECTOR_H_
#define FOOLRENDERER_MATH_VECTOR_H_

#include <math.h>

#include "math/math_utility.h"

///
/// \brief A vector in 2D space composed of components (x, y) with floating
///        point value.
///
/// The x, y components can be accessed using u, v respectively. Or access in an
/// array through the member: elements.
///
typedef union vector2 {
    struct {
        float x, y;
    };
    struct {
        float u, v;
    };
    float elements[2];
} vector2;

///
/// \brief A vector in 3D space composed of components (x, y, z) with floating
///        point value.
///
/// The x, y, z components can be accessed using r, g, b respectively. Or access
/// in an array through the member: elements.
///
typedef union vector3 {
    struct {
        float x, y, z;
    };
    struct {
        float r, g, b;
    };
    float elements[3];
} vector3;

///
/// \brief A vector in 4D space composed of components (x, y, z, w) with
///        floating point value.
///
/// The x, y, z, w components can be accessed using r, g, b, a respectively. Or
/// access in an array through the member: elements.
///
typedef union vector4 {
    struct {
        float x, y, z, w;
    };
    struct {
        float r, g, b, a;
    };
    float elements[4];
} vector4;

///
/// \brief Zero vector (0,0) constant.
///
#define VECTOR2_ZERO ((const vector2){{0.0f, 0.0f}})

///
/// \brief One vector (1,1) constant.
///
#define VECTOR2_ONE ((const vector2){{1.0f, 1.0f}})

///
/// \brief Zero vector (0,0,0) constant.
///
#define VECTOR3_ZERO ((const vector3){{0.0f, 0.0f, 0.0f}})

///
/// \brief One vector (1,1,1) constant.
///
#define VECTOR3_ONE ((const vector3){{1.0f, 1.0f, 1.0f}})

///
/// \brief Zero vector (0,0,0,0) constant.
///
#define VECTOR4_ZERO ((const vector4){{0.0f, 0.0f, 0.0f, 0.0f}})

///
/// \brief One vector (1,1,1,1) constant.
///
#define VECTOR4_ONE ((const vector4){{1.0f, 1.0f, 1.0f, 1.0f}})

////////////////////////////////////////////////////////////////////////////////
//
// Construct vector utilities.
//
////////////////////////////////////////////////////////////////////////////////

///
/// \brief Constructs a 2D vector from a 3D vector. The 3D vector provides
///        x and y component values.
///
/// \param v Vector to copy from.
/// \return The constructed 2D vector.
///
inline vector2 vector3_to_2(vector3 v) { return (vector2){{v.x, v.y}}; }

///
/// \brief Constructs a 2D vector from a 3D vector. The 3D vector provides
///        x and y component values.
///
/// \param v Vector to copy from.
/// \return The constructed 2D vector.
///
inline vector2 vector4_to_2(vector4 v) { return (vector2){{v.x, v.y}}; }

///
/// \brief Constructs a 3D vector from a 2D vector and z value.
///
/// \param v The vector provides x and y component values.
/// \param z The z component value.
/// \return The constructed 3D vector.
///
inline vector3 vector2_to_3(vector2 v, float z) {
    return (vector3){{v.x, v.y, z}};
}

///
/// \brief Constructs a 3D vector from a 4D vector. The 4D vector provides
///        x, y and z component values.
///
/// \param v Vector to copy from.
/// \return The constructed 3D vector.
///
inline vector3 vector4_to_3(vector4 v) { return (vector3){{v.x, v.y, v.z}}; }

///
/// \brief Constructs a 4D vector from a 3D vector and w value.
///
/// \param v The vector provides x, y and z component values.
/// \param w The w component value.
/// \return The constructed 4D vector.
///
inline vector4 vector3_to_4(vector3 v, float w) {
    return (vector4){{v.x, v.y, v.z, w}};
}

////////////////////////////////////////////////////////////////////////////////
//
// 2D vector functions.
//
////////////////////////////////////////////////////////////////////////////////

inline vector2 vector2_add(vector2 v1, vector2 v2) {
    return (vector2){{v1.x + v2.x, v1.y + v2.y}};
}

inline vector2 vector2_add_scalar(vector2 v, float scalar) {
    return (vector2){{v.x + scalar, v.y + scalar}};
}

inline vector2 vector2_subtract(vector2 v1, vector2 v2) {
    return (vector2){{v1.x - v2.x, v1.y - v2.y}};
}

inline vector2 vector2_subtract_scalar(vector2 v, float scalar) {
    return (vector2){{v.x - scalar, v.y - scalar}};
}

inline vector2 vector2_multiply(vector2 v1, vector2 v2) {
    return (vector2){{v1.x * v2.x, v1.y * v2.y}};
}

inline vector2 vector2_multiply_scalar(vector2 v, float scalar) {
    return (vector2){{v.x * scalar, v.y * scalar}};
}

inline vector2 vector2_divide(vector2 v1, vector2 v2) {
    return (vector2){{v1.x / v2.x, v1.y / v2.y}};
}

inline vector2 vector2_divide_scalar(vector2 v, float scalar) {
    return (vector2){{v.x / scalar, v.y / scalar}};
}

inline float vector2_dot(vector2 v1, vector2 v2) {
    return v1.x * v2.x + v1.y * v2.y;
}

///
/// \brief Gets the lenght of a 2D vector.
///
/// If only need to compare lenght of some vectors, using
/// vector2_magnitude_squared() instead. Because this function
/// doesn't perform square root, the execution speed is faster.
///
/// \param v The vector to be calculated.
/// \return The length of the vector.
///
inline float vector2_magnitude(vector2 v) { return sqrtf(vector2_dot(v, v)); }

///
/// \brief Gets the squared lenght of a 2D vector.
///
/// \param v The vector to be calculated.
/// \return The squared length of the vector.
///
inline float vector2_magnitude_squared(vector2 v) { return vector2_dot(v, v); }

///
/// \brief Gets a normalized copy of the 2D vector. Returns a zero vector if the
///        vector magnitude is 0.
///
/// \param v The vector to be normalized.
/// \return Returns a normalized copy if the vector magnitude is not equal 0,
///         otherwise returns zero vector.
///
inline vector2 vector2_normalize(vector2 v) {
    float square_magnitude = vector2_magnitude_squared(v);
    if (square_magnitude == 0.0f) {
        return VECTOR2_ZERO;
    }
    if (fabsf(square_magnitude - 1.0f) < SMALL_ABSOLUTE_FLOAT) {
        return v;
    }
    return vector2_multiply_scalar(v, 1.0f / sqrtf(square_magnitude));
}

inline vector2 vector2_lerp(vector2 a, vector2 b, float t) {
    float x = float_lerp(a.x, b.x, t);
    float y = float_lerp(a.y, b.y, t);
    return (vector2){{x, y}};
}

////////////////////////////////////////////////////////////////////////////////
//
// 3D vector functions.
//
////////////////////////////////////////////////////////////////////////////////

inline vector3 vector3_add(vector3 v1, vector3 v2) {
    return (vector3){{v1.x + v2.x, v1.y + v2.y, v1.z + v2.z}};
}

inline vector3 vector3_add_scalar(vector3 v, float scalar) {
    return (vector3){{v.x + scalar, v.y + scalar, v.z + scalar}};
}

inline vector3 vector3_subtract(vector3 v1, vector3 v2) {
    return (vector3){{v1.x - v2.x, v1.y - v2.y, v1.z - v2.z}};
}

inline vector3 vector3_subtract_scalar(vector3 v, float scalar) {
    return (vector3){{v.x - scalar, v.y - scalar, v.z - scalar}};
}

inline vector3 vector3_multiply(vector3 v1, vector3 v2) {
    return (vector3){{v1.x * v2.x, v1.y * v2.y, v1.z * v2.z}};
}

inline vector3 vector3_multiply_scalar(vector3 v, float scalar) {
    return (vector3){{v.x * scalar, v.y * scalar, v.z * scalar}};
}

inline vector3 vector3_divide(vector3 v1, vector3 v2) {
    return (vector3){{v1.x / v2.x, v1.y / v2.y, v1.z / v2.z}};
}

inline vector3 vector3_divide_scalar(vector3 v, float scalar) {
    return (vector3){{v.x / scalar, v.y / scalar, v.z / scalar}};
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

///
/// \brief Gets the lenght of a 3D vector.
///
/// If only need to compare lenght of some vectors, using
/// vector3_magnitude_squared() instead. Because this function
/// doesn't perform square root, the execution speed is faster.
///
/// \param v The vector to be calculated.
/// \return The length of the vector.
///
inline float vector3_magnitude(vector3 v) { return sqrtf(vector3_dot(v, v)); }

///
/// \brief Gets the squared lenght of a 3D vector.
///
/// \param v The vector to be calculated.
/// \return The squared length of the vector.
///
inline float vector3_magnitude_squared(vector3 v) { return vector3_dot(v, v); }

///
/// \brief Gets a normalized copy of the 3D vector. Returns a zero vector if the
///        vector magnitude is 0.
///
/// \param v The vector to be normalized.
/// \return Returns a normalized copy if the vector magnitude is not equal 0,
///         otherwise returns zero vector.
///
inline vector3 vector3_normalize(vector3 v) {
    float square_magnitude = vector3_magnitude_squared(v);
    if (square_magnitude == 0.0f) {
        return VECTOR3_ZERO;
    }
    if (fabsf(square_magnitude - 1.0f) < SMALL_ABSOLUTE_FLOAT) {
        return v;
    }
    return vector3_multiply_scalar(v, 1.0f / sqrtf(square_magnitude));
}

inline vector3 vector3_lerp(vector3 a, vector3 b, float t) {
    float x = float_lerp(a.x, b.x, t);
    float y = float_lerp(a.y, b.y, t);
    float z = float_lerp(a.z, b.z, t);
    return (vector3){{x, y, z}};
}

////////////////////////////////////////////////////////////////////////////////
//
// 4D vector functions.
//
////////////////////////////////////////////////////////////////////////////////

inline vector4 vector4_add(vector4 v1, vector4 v2) {
    return (vector4){{v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w}};
}

inline vector4 vector4_add_scalar(vector4 v, float scalar) {
    return (vector4){{v.x + scalar, v.y + scalar, v.z + scalar, v.w + scalar}};
}

inline vector4 vector4_subtract(vector4 v1, vector4 v2) {
    return (vector4){{v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w}};
}

inline vector4 vector4_subtract_scalar(vector4 v, float scalar) {
    return (vector4){{v.x - scalar, v.y - scalar, v.z - scalar, v.w - scalar}};
}

inline vector4 vector4_multiply(vector4 v1, vector4 v2) {
    return (vector4){{v1.x * v2.x, v1.y * v2.y, v1.z * v2.z, v1.w * v2.z}};
}

inline vector4 vector4_multiply_scalar(vector4 v, float scalar) {
    return (vector4){{v.x * scalar, v.y * scalar, v.z * scalar, v.w * scalar}};
}

inline vector4 vector4_divide(vector4 v1, vector4 v2) {
    return (vector4){{v1.x / v2.x, v1.y / v2.y, v1.z / v2.z, v1.w / v2.w}};
}

inline vector4 vector4_divide_scalar(vector4 v, float scalar) {
    return (vector4){{v.x / scalar, v.y / scalar, v.z / scalar, v.w / scalar}};
}

inline float vector4_dot(vector4 v1, vector4 v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
}

///
/// \brief Gets the lenght of the 4D vector.
///
/// If only need to compare lenght of some vectors, using
/// vector4_magnitude_squared() instead. Because this function
/// doesn't perform square root, the execution speed is faster.
///
/// \param v The vector to be calculated.
/// \return The length of the vector.
///
inline float vector4_magnitude(vector4 v) { return sqrtf(vector4_dot(v, v)); }

///
/// \brief Gets the squared lenght of the 4D vector.
///
/// \param v The vector to be calculated.
/// \return The squared length of the vector.
///
inline float vector4_magnitude_squared(vector4 v) { return vector4_dot(v, v); }

///
/// \brief Gets a normalized copy of the 4D vector. Returns a zero vector if the
///        vector magnitude is 0.
///
/// \param v The vector to be normalized.
/// \return Returns a normalized copy if the vector magnitude is not equal 0,
///         otherwise returns zero vector.
///
inline vector4 vector4_normalize(vector4 v) {
    float square_magnitude = vector4_magnitude_squared(v);
    if (square_magnitude == 0.0f) {
        return VECTOR4_ZERO;
    }
    if (fabsf(square_magnitude - 1.0f) < SMALL_ABSOLUTE_FLOAT) {
        return v;
    }
    return vector4_multiply_scalar(v, 1.0f / sqrtf(square_magnitude));
}

inline vector4 vector4_lerp(vector4 a, vector4 b, float t) {
    float x = float_lerp(a.x, b.x, t);
    float y = float_lerp(a.y, b.y, t);
    float z = float_lerp(a.z, b.z, t);
    float w = float_lerp(a.w, b.w, t);
    return (vector4){{x, y, z, w}};
}

#endif  // FOOLRENDERER_MATH_VECTOR_H_
