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

#include "math/vector.h"

extern struct vector2 vector3_to_2(struct vector3 v);

extern struct vector3 vector2_to_3(struct vector2 v, float z);

extern struct vector3 vecotr4_to_3(struct vector4 v);

extern struct vector4 vector3_to_4(struct vector3 v, float w);

extern struct vector2 vector2_add(struct vector2 v1, struct vector2 v2);

extern struct vector2 vector2_subtract(struct vector2 v1, struct vector2 v2);

extern struct vector2 vector2_multiply(struct vector2 v, float scalar);

extern struct vector2 vector2_divide(struct vector2 v, float scalar);

extern float vector2_dot(struct vector2 v1, struct vector2 v2);

extern float vector2_magnitude(struct vector2 v);

extern float vector2_magnitude_squared(struct vector2 v);

extern struct vector2 vector2_normalize(struct vector2 v);

extern struct vector3 vector3_add(struct vector3 v1, struct vector3 v2);

extern struct vector3 vector3_subtract(struct vector3 v1, struct vector3 v2);

extern struct vector3 vector3_multiply(struct vector3 v, float scalar);

extern struct vector3 vector3_divide(struct vector3 v, float scalar);

extern float vector3_dot(struct vector3 v1, struct vector3 v2);

extern struct vector3 vector3_cross(struct vector3 v1, struct vector3 v2);

extern float vector3_magnitude(struct vector3 v);

extern float vector3_magnitude_squared(struct vector3 v);

extern struct vector3 vector3_normalize(struct vector3 v);

extern struct vector4 vector4_add(struct vector4 v1, struct vector4 v2);

extern struct vector4 vector4_subtract(struct vector4 v1, struct vector4 v2);

extern struct vector4 vector4_multiply(struct vector4 v, float scalar);

extern struct vector4 vector4_divide(struct vector4 v, float scalar);
