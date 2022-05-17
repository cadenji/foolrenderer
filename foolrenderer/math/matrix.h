// Copyright (c) Caden Ji. All rights reserved.
//
// Licensed under the MIT License. See LICENSE.txt in the project root for
// license information.

#ifndef FOOLRENDERER_MATH_MATRIX_H_
#define FOOLRENDERER_MATH_MATRIX_H_

#include <math.h>

#include "math/vector.h"

///
/// \brief 3x3 matrix of floating point values.
///
/// Matrices are row major, elements are accessed with
/// matrix.elements[row_index][column_index].
///
typedef struct matrix3x3 {
    float elements[3][3];
} matrix3x3;

///
/// \brief 4x4 matrix of floating point values.
///
/// Matrices are row major, elements are accessed with
/// matrix.elements[row_index][column_index].
///
typedef struct matrix4x4 {
    float elements[4][4];
} matrix4x4;

///
/// \brief 3x3 identity matrix constant.
///
#define MATRIX3X3_IDENTITY \
    ((const matrix3x3){    \
        {{1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}}})

///
/// \brief 3x3 zero matrix constant, all elements are set to zero.
///
#define MATRIX3X3_ZERO  \
    ((const matrix3x3){ \
        {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}}})

///
/// \brief 4x4 identity matrix constant.
///
#define MATRIX4X4_IDENTITY                        \
    ((const matrix4x4){{{1.0f, 0.0f, 0.0f, 0.0f}, \
                        {0.0f, 1.0f, 0.0f, 0.0f}, \
                        {0.0f, 0.0f, 1.0f, 0.0f}, \
                        {0.0f, 0.0f, 0.0f, 1.0f}}})

///
/// \brief 4x4 zero matrix constant, all elements are set to zero.
///
#define MATRIX4X4_ZERO                            \
    ((const matrix4x4){{{0.0f, 0.0f, 0.0f, 0.0f}, \
                        {0.0f, 0.0f, 0.0f, 0.0f}, \
                        {0.0f, 0.0f, 0.0f, 0.0f}, \
                        {0.0f, 0.0f, 0.0f, 0.0f}}})

inline matrix3x3 matrix3x3_construct(vector3 col_1, vector3 col_2,
                                     vector3 col_3) {
    return (matrix3x3){{{col_1.x, col_2.x, col_3.x},
                        {col_1.y, col_2.y, col_3.y},
                        {col_1.z, col_2.z, col_3.z}}};
}

inline matrix4x4 matrix4x4_construct(vector4 col_1, vector4 col_2,
                                     vector4 col_3, vector4 col_4) {
    return (matrix4x4){{{col_1.x, col_2.x, col_3.x, col_4.x},
                        {col_1.y, col_2.y, col_3.y, col_4.y},
                        {col_1.z, col_2.z, col_3.z, col_4.z},
                        {col_1.w, col_2.w, col_3.w, col_4.w}}};
}

///
/// \brief Constructs a matrix3x3 from the upper-left of matrix4x4.
///
inline matrix3x3 matrix4x4_to_3x3(matrix4x4 m) {
    return (matrix3x3){
        {{m.elements[0][0], m.elements[0][1], m.elements[0][2]},
         {m.elements[1][0], m.elements[1][1], m.elements[1][2]},
         {m.elements[2][0], m.elements[2][1], m.elements[2][2]}}};
}

////////////////////////////////////////////////////////////////////////////////
//
// 3x3 matrix functions.
//
////////////////////////////////////////////////////////////////////////////////

inline matrix3x3 matrix3x3_multiply_scalar(matrix3x3 m, float scalar) {
    for (int row = 0; row < 3; row++) {
        for (int column = 0; column < 3; column++) {
            m.elements[row][column] *= scalar;
        }
    }
    return m;
}

inline vector3 matrix3x3_multiply_vector3(matrix3x3 m, vector3 v) {
    vector3 result = VECTOR3_ZERO;
    for (int row = 0; row < 3; row++) {
        for (int column = 0; column < 3; column++) {
            result.elements[row] +=
                m.elements[row][column] * v.elements[column];
        }
    }
    return result;
}

inline matrix3x3 matrix3x3_multiply(matrix3x3 lift, matrix3x3 right) {
    matrix3x3 result = MATRIX3X3_ZERO;
    for (int v = 0; v < 3; v++) {
        for (int row = 0; row < 3; row++) {
            for (int column = 0; column < 3; column++) {
                result.elements[row][v] +=
                    lift.elements[row][column] * right.elements[column][v];
            }
        }
    }
    return result;
}

inline matrix3x3 matrix3x3_transpose(matrix3x3 m) {
    matrix3x3 result;
    for (int row = 0; row < 3; row++) {
        for (int column = 0; column < 3; column++) {
            result.elements[column][row] = m.elements[row][column];
        }
    }
    return result;
}

////////////////////////////////////////////////////////////////////////////////
//
// 4x4 matrix functions.
//
////////////////////////////////////////////////////////////////////////////////

///
/// \brief Converts the 4x4 matrix to string and writes the result to stdout.
///
/// \param m The matrix to be printed.
///
void matrix4x4_print(matrix4x4 m);

inline matrix4x4 matrix4x4_multiply_scalar(matrix4x4 m, float scalar) {
    for (int row = 0; row < 4; row++) {
        for (int column = 0; column < 4; column++) {
            m.elements[row][column] *= scalar;
        }
    }
    return m;
}

inline vector4 matrix4x4_multiply_vector4(matrix4x4 m, vector4 v) {
    vector4 result = VECTOR4_ZERO;
    for (int row = 0; row < 4; row++) {
        for (int column = 0; column < 4; column++) {
            result.elements[row] +=
                m.elements[row][column] * v.elements[column];
        }
    }
    return result;
}

inline matrix4x4 matrix4x4_multiply(matrix4x4 lift, matrix4x4 right) {
    matrix4x4 result = MATRIX4X4_ZERO;
    for (int v = 0; v < 4; v++) {
        for (int row = 0; row < 4; row++) {
            for (int column = 0; column < 4; column++) {
                result.elements[row][v] +=
                    lift.elements[row][column] * right.elements[column][v];
            }
        }
    }
    return result;
}

inline matrix4x4 matrix4x4_transpose(matrix4x4 m) {
    matrix4x4 result;
    for (int row = 0; row < 4; row++) {
        for (int column = 0; column < 4; column++) {
            result.elements[column][row] = m.elements[row][column];
        }
    }
    return result;
}

///
/// \brief If the matrix is invertible, gets the inverse of the matrix.
///        Returns a zero matrix otherwise.
///
/// \param m The matrix to be calculated.
/// \return The inverse of the matrix.
///
inline matrix4x4 matrix4x4_inverse(matrix4x4 m) {
    const float a11 = m.elements[0][0], a12 = m.elements[0][1],
                a13 = m.elements[0][2], a14 = m.elements[0][3];
    const float a21 = m.elements[1][0], a22 = m.elements[1][1],
                a23 = m.elements[1][2], a24 = m.elements[1][3];
    const float a31 = m.elements[2][0], a32 = m.elements[2][1],
                a33 = m.elements[2][2], a34 = m.elements[2][3];
    const float a41 = m.elements[3][0], a42 = m.elements[3][1],
                a43 = m.elements[3][2], a44 = m.elements[3][3];

    // Uses the adjugate of the matrix to calculates the inverse.
    matrix4x4 adj;
    adj.elements[0][0] = a22 * a33 * a44 + a23 * a34 * a42 + a24 * a32 * a43 -
                         a24 * a33 * a42 - a23 * a32 * a44 - a22 * a34 * a43;
    adj.elements[0][1] = -a12 * a33 * a44 - a13 * a34 * a42 - a14 * a32 * a43 +
                         a14 * a33 * a42 + a13 * a32 * a44 + a12 * a34 * a43;
    adj.elements[0][2] = a12 * a23 * a44 + a13 * a24 * a42 + a14 * a22 * a43 -
                         a14 * a23 * a42 - a13 * a22 * a44 - a12 * a24 * a43;
    adj.elements[0][3] = -a12 * a23 * a34 - a13 * a24 * a32 - a14 * a22 * a33 +
                         a14 * a23 * a32 + a13 * a22 * a34 + a12 * a24 * a33;

    adj.elements[1][0] = -a21 * a33 * a44 - a23 * a34 * a41 - a24 * a31 * a43 +
                         a24 * a33 * a41 + a23 * a31 * a44 + a21 * a34 * a43;
    adj.elements[1][1] = a11 * a33 * a44 + a13 * a34 * a41 + a14 * a31 * a43 -
                         a14 * a33 * a41 - a13 * a31 * a44 - a11 * a34 * a43;
    adj.elements[1][2] = -a11 * a23 * a44 - a13 * a24 * a41 - a14 * a21 * a43 +
                         a14 * a23 * a41 + a13 * a21 * a44 + a11 * a24 * a43;
    adj.elements[1][3] = a11 * a23 * a34 + a13 * a24 * a31 + a14 * a21 * a33 -
                         a14 * a23 * a31 - a13 * a21 * a34 - a11 * a24 * a33;

    adj.elements[2][0] = a21 * a32 * a44 + a22 * a34 * a41 + a24 * a31 * a42 -
                         a24 * a32 * a41 - a22 * a31 * a44 - a21 * a34 * a42;
    adj.elements[2][1] = -a11 * a32 * a44 - a12 * a34 * a41 - a14 * a31 * a42 +
                         a14 * a32 * a41 + a12 * a31 * a44 + a11 * a34 * a42;
    adj.elements[2][2] = a11 * a22 * a44 + a12 * a24 * a41 + a14 * a21 * a42 -
                         a14 * a22 * a41 - a12 * a21 * a44 - a11 * a24 * a42;
    adj.elements[2][3] = -a11 * a22 * a34 - a12 * a24 * a31 - a14 * a21 * a32 +
                         a14 * a22 * a31 + a12 * a21 * a34 + a11 * a24 * a32;

    adj.elements[3][0] = -a21 * a32 * a43 - a22 * a33 * a41 - a23 * a31 * a42 +
                         a23 * a32 * a41 + a22 * a31 * a43 + a21 * a33 * a42;
    adj.elements[3][1] = a11 * a32 * a43 + a12 * a33 * a41 + a13 * a31 * a42 -
                         a13 * a32 * a41 - a12 * a31 * a43 - a11 * a33 * a42;
    adj.elements[3][2] = -a11 * a22 * a43 - a12 * a23 * a41 - a13 * a21 * a42 +
                         a13 * a22 * a41 + a12 * a21 * a43 + a11 * a23 * a42;
    adj.elements[3][3] = a11 * a22 * a33 + a12 * a23 * a31 + a13 * a21 * a32 -
                         a13 * a22 * a31 - a12 * a21 * a33 - a11 * a23 * a32;

    float determinant = a11 * adj.elements[0][0] + a21 * adj.elements[0][1] +
                        a31 * adj.elements[0][2] + a41 * adj.elements[0][3];

    if (determinant == 0.0f) {
        // The matrix is not invertible.
        return MATRIX4X4_ZERO;
    }
    return matrix4x4_multiply_scalar(adj, 1.0f / determinant);
}

///
/// \brief Constructs a scaling matrix.
///
/// \param scaling Specifys the scaling factor on the x, y and z axis.
/// \return The scaling matrix.
///
inline matrix4x4 matrix4x4_scale(vector3 scaling) {
    matrix4x4 result = MATRIX4X4_IDENTITY;
    result.elements[0][0] = scaling.x;
    result.elements[1][1] = scaling.y;
    result.elements[2][2] = scaling.z;
    return result;
}

///
/// \brief Constructs a translation matrix.
///
/// \param translation Specifys the translation on the x, y and z axis.
/// \return The translation matrix.
///
inline matrix4x4 matrix4x4_translate(vector3 translation) {
    matrix4x4 result = MATRIX4X4_IDENTITY;
    result.elements[0][3] = translation.x;
    result.elements[1][3] = translation.y;
    result.elements[2][3] = translation.z;
    return result;
}

///
/// \brief Constructs a rotation matrix along the x axis.
///
/// \param angle The angle of rotation along the x axis, in radians.
/// \return The rotation matrix.
///
inline matrix4x4 matrix4x4_rotate_x(float angle) {
    // For construction a rotation transformation in 3D, refer to:
    // https://www.youtube.com/watch?v=gkyuLPzfDV0
    float c = cosf(angle);
    float s = sinf(angle);
    matrix4x4 result = MATRIX4X4_IDENTITY;
    result.elements[1][1] = c;
    result.elements[1][2] = -s;
    result.elements[2][1] = s;
    result.elements[2][2] = c;
    return result;
}

///
/// \brief Constructs a rotation matrix along the y axis.
///
/// \param angle The angle of rotation along the y axis, in radians.
/// \return The rotation matrix.
///
inline matrix4x4 matrix4x4_rotate_y(float angle) {
    float c = cosf(angle);
    float s = sinf(angle);
    matrix4x4 result = MATRIX4X4_IDENTITY;
    result.elements[0][0] = c;
    result.elements[0][2] = s;
    result.elements[2][0] = -s;
    result.elements[2][2] = c;
    return result;
}

///
/// \brief Constructs a rotation matrix along the z axis.
///
/// \param angle The angle of rotation along the z axis, in radians.
/// \return The rotation matrix.
///
inline matrix4x4 matrix4x4_rotate_z(float angle) {
    float c = cosf(angle);
    float s = sinf(angle);
    matrix4x4 result = MATRIX4X4_IDENTITY;
    result.elements[0][0] = c;
    result.elements[0][1] = -s;
    result.elements[1][0] = s;
    result.elements[1][1] = c;
    return result;
}

///
/// \brief Constructs a rotation matrix about an arbitrary vector.
///
/// \param angle The angle of rotation, in radians.
/// \param about The vector.
/// \return The rotation matrix.
///
inline matrix4x4 matrix4x4_rotate(float angle, vector3 about) {
    if (about.x == 1.0f && about.y == 0.0f && about.z == 0.0f) {
        return matrix4x4_rotate_x(angle);
    }
    if (about.x == 0.0f && about.y == 1.0f && about.z == 0.0f) {
        return matrix4x4_rotate_y(angle);
    }
    if (about.x == 0.0f && about.y == 0.0f && about.z == 1.0f) {
        return matrix4x4_rotate_z(angle);
    }
    // For the derivation of this matrix, refer to the thesis 9.2.4. Rotations
    // about Arbitrary Axes:
    // https://repository.lboro.ac.uk/articles/thesis/Modelling_CPV/9523520
    matrix4x4 result = MATRIX4X4_IDENTITY;
    float c = cosf(angle);
    float s = sinf(angle);
    about = vector3_normalize(about);
    float nc = 1 - c;
    float xy = about.x * about.y;
    float yz = about.y * about.z;
    float zx = about.z * about.x;
    float xs = about.x * s;
    float ys = about.y * s;
    float zs = about.z * s;

    result.elements[0][0] = about.x * about.x * nc + c;
    result.elements[0][1] = xy * nc - zs;
    result.elements[0][2] = zx * nc + ys;

    result.elements[1][0] = xy * nc + zs;
    result.elements[1][1] = about.y * about.y * nc + c;
    result.elements[1][2] = yz * nc - xs;

    result.elements[2][0] = zx * nc - ys;
    result.elements[2][1] = yz * nc + xs;
    result.elements[2][2] = about.z * about.z * nc + c;

    return result;
}

///
/// \brief Constructs a view matrix.
///
/// The view matrix is used to transform the world space vertices to the view
/// space.
///
/// \param from The position of the camera point.
/// \param to The position of the target point.
/// \param up The direction of the up vector.
/// \return The view matrix.
///
inline matrix4x4 matrix4x4_look_at(vector3 from, vector3 to, vector3 up) {
    // For the derivation of this matrix, refer to:
    // http://www.songho.ca/opengl/gl_camera.html#lookat
    //
    // In foolrenderer, world space and view space are right-handed coordinate
    // systems (matches OpenGL convention), so the direction of z_axis is
    // opposite to the direction in which the camera points to the target.
    vector3 z_axis = vector3_normalize(vector3_subtract(from, to));
    vector3 x_axis = vector3_normalize(vector3_cross(up, z_axis));
    vector3 y_axis = vector3_cross(z_axis, x_axis);
    matrix4x4 result = MATRIX4X4_IDENTITY;

    result.elements[0][0] = x_axis.x;
    result.elements[0][1] = x_axis.y;
    result.elements[0][2] = x_axis.z;

    result.elements[1][0] = y_axis.x;
    result.elements[1][1] = y_axis.y;
    result.elements[1][2] = y_axis.z;

    result.elements[2][0] = z_axis.x;
    result.elements[2][1] = z_axis.y;
    result.elements[2][2] = z_axis.z;

    result.elements[0][3] = -vector3_dot(x_axis, from);
    result.elements[1][3] = -vector3_dot(y_axis, from);
    result.elements[2][3] = -vector3_dot(z_axis, from);

    return result;
}

///
/// \brief Constructs a perspective projection matrix, follow OpenGL convention.
///
/// \param fov The vertical field of view in radians.
/// \param aspect The aspect ration (width divided by height).
/// \param near The distance to the near depth clipping plane.
/// \param far The distance to the far depth clipping plane.
/// \return The perspective projection matrix.
///
inline matrix4x4 matrix4x4_perspective(float fov, float aspect, float near,
                                       float far) {
    // For the derivation of this matrix, refer to:
    // http://www.songho.ca/opengl/gl_projectionmatrix.html
    matrix4x4 result = MATRIX4X4_ZERO;
    float fn = far - near;
    result.elements[1][1] = 1.0f / tanf(fov / 2.0f);
    result.elements[0][0] = result.elements[1][1] / aspect;
    result.elements[2][2] = (-far - near) / fn;
    result.elements[2][3] = (-2.0f * far * near) / fn;
    result.elements[3][2] = -1.0f;
    return result;
}

///
/// \brief Constructs an orthogonal projection matrix, follow OpenGL convention.
///
/// \param right Coordinate of the right clipping plane .
/// \param top Coordinates of the top clipping plane.
/// \param near The distance to the near depth clipping plane.
/// \param far The distance to the far depth clipping plane.
/// \return The orthogonal projection matrix.
///
inline matrix4x4 matrix4x4_orthographic(float right, float top, float near,
                                        float far) {
    // For the derivation of this matrix, refer to:
    // http://www.songho.ca/opengl/gl_projectionmatrix.html
    matrix4x4 result = MATRIX4X4_IDENTITY;
    float fn = far - near;
    result.elements[0][0] = 1.0f / right;
    result.elements[1][1] = 1.0f / top;
    result.elements[2][2] = -2.0f / fn;
    result.elements[2][3] = (-near - far) / fn;
    return result;
}

#endif  // FOOLRENDERER_MATH_MATRIX_H_
