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

#include "math/vector.h"

/// 4x4 matrix of floating point values.
/// Matrices are row major, elements are accessed with
/// matrix.elements[row_index][column_index].
typedef struct matrix4x4 {
    float elements[4][4];
} matrix4x4;

/// Constant identity 4x4 matrix.
#define MATRIX4X4_IDENTITY                        \
    ((const matrix4x4){{{1.0f, 0.0f, 0.0f, 0.0f}, \
                        {0.0f, 1.0f, 0.0f, 0.0f}, \
                        {0.0f, 0.0f, 1.0f, 0.0f}, \
                        {0.0f, 0.0f, 0.0f, 1.0f}}})

/// Constant zero 4x4 matrix with all elements set to zero.
#define MATRIX4X4_ZERO                            \
    ((const matrix4x4){{{0.0f, 0.0f, 0.0f, 0.0f}, \
                        {0.0f, 0.0f, 0.0f, 0.0f}, \
                        {0.0f, 0.0f, 0.0f, 0.0f}, \
                        {0.0f, 0.0f, 0.0f, 0.0f}}})

/// Converts the 4x4 matrix to string and writes the result to stdout.
///
/// \param m The matrix to be printed.
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

#endif  // FOOLRENDERER_MATH_MATRIX_H_
