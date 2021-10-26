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

#ifndef FOOLRENDERER_MATH_MATRIX_H_
#define FOOLRENDERER_MATH_MATRIX_H_

#include <math.h>

#include "math/math_utility.h"
#include "math/vector.h"

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
    matrix4x4 result = {0};
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
/// The invertible check simply compares the calculated determinant with
/// SMALL_ABSOLUTE_FLOAT to roughly determine whether the matrix is invertible.
/// This simple check works in common cases, but fails when the matrix is a
/// combination of huge and small values that can't be accurately represented by
/// the floating point type.
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

    if (fabsf(determinant) < SMALL_ABSOLUTE_FLOAT) {
        // Roughly thinks the determinant is zero,
        // so the matrix is not invertible.
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
    float cos = cosf(angle);
    float sin = sinf(angle);
    matrix4x4 result = MATRIX4X4_IDENTITY;
    result.elements[1][1] = cos;
    result.elements[1][2] = -sin;
    result.elements[2][1] = sin;
    result.elements[2][2] = cos;
    return result;
}

///
/// \brief Constructs a rotation matrix along the y axis.
///
/// \param angle The angle of rotation along the y axis, in radians.
/// \return The rotation matrix.
///
inline matrix4x4 matrix4x4_rotate_y(float angle) {
    float cos = cosf(angle);
    float sin = sinf(angle);
    matrix4x4 result = MATRIX4X4_IDENTITY;
    result.elements[0][0] = cos;
    result.elements[0][2] = sin;
    result.elements[2][0] = -sin;
    result.elements[2][2] = cos;
    return result;
}

///
/// \brief Constructs a rotation matrix along the z axis.
///
/// \param angle The angle of rotation along the z axis, in radians.
/// \return The rotation matrix.
///
inline matrix4x4 matrix4x4_rotate_z(float angle) {
    float cos = cosf(angle);
    float sin = sinf(angle);
    matrix4x4 result = MATRIX4X4_IDENTITY;
    result.elements[0][0] = cos;
    result.elements[0][1] = -sin;
    result.elements[1][0] = sin;
    result.elements[1][1] = cos;
    return result;
}

#endif  // FOOLRENDERER_MATH_MATRIX_H_
