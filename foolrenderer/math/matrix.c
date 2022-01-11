// Copyright (c) Caden Ji. All rights reserved.
//
// Licensed under the MIT License. See LICENSE.txt in the project root for
// license information.

#include "math/matrix.h"

#include <stdio.h>

extern matrix3x3 matrix4x4_to_3x3(matrix4x4 m);

extern matrix3x3 matrix3x3_multiply_scalar(matrix3x3 m, float scalar);

extern vector3 matrix3x3_multiply_vector3(matrix3x3 m, vector3 v);

extern matrix3x3 matrix3x3_multiply(matrix3x3 lift, matrix3x3 right);

extern matrix3x3 matrix3x3_transpose(matrix3x3 m);

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

extern matrix4x4 matrix4x4_rotate(float angle, vector3 about);

extern matrix4x4 matrix4x4_look_at(vector3 eye, vector3 target, vector3 up);

extern matrix4x4 matrix4x4_perspective(float fov, float aspect, float near,
                                       float far);

extern matrix4x4 matrix4x4_orthographic(float right, float top, float near,
                                        float far);
