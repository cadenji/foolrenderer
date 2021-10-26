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

#include "math/matrix.h"

#include <stdio.h>

void matrix4x4_print(matrix4x4 m) {
    for (int row = 0; row < 4; row++) {
        for (int column = 0; column < 4; column++) {
            printf("%8.3f ", m.elements[row][column]);
        }
        printf("\n");
    }
}

extern matrix4x4 matrix4x4_multiply_scalar(matrix4x4 m, float scalar);

extern vector4 matrix4x4_multiply_vector4(matrix4x4 m, vector4 v);

extern matrix4x4 matrix4x4_multiply(matrix4x4 lift, matrix4x4 right);

extern matrix4x4 matrix4x4_transpose(matrix4x4 m);

extern matrix4x4 matrix4x4_inverse(matrix4x4 m);

extern matrix4x4 matrix4x4_scale(vector3 scaling);

extern matrix4x4 matrix4x4_translate(vector3 translation);

extern matrix4x4 matrix4x4_rotate_x(float angle);

extern matrix4x4 matrix4x4_rotate_y(float angle);

extern matrix4x4 matrix4x4_rotate_z(float angle);
