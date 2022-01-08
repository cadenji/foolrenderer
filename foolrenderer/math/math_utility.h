// Copyright (c) Caden Ji. All rights reserved.
//
// Licensed under the MIT License. See LICENSE.txt in the project root for
// license information.

#ifndef FOOLRENDERER_MATH_MATH_UTILITY_H_
#define FOOLRENDERER_MATH_MATH_UTILITY_H_

#define SMALL_ABSOLUTE_FLOAT (1.e-8f)

#define PI (3.1415926535897932f)
#define HALF_PI (1.57079632679f)
#define INVERSE_PI (0.31830988618f)

inline int max_int(int a, int b) { return a > b ? a : b; }

inline int min_int(int a, int b) { return a < b ? a : b; }

inline float max_float(float a, float b) { return a > b ? a : b; }

inline float min_float(float a, float b) { return a < b ? a : b; }

inline int clamp_int(int n, int min, int max) {
    int t = n < min ? min : n;
    return t > max ? max : t;
}

inline float clamp_float(float n, float min, float max) {
    float t = n < min ? min : n;
    return t > max ? max : t;
}

inline int clamp01_int(int n) { return clamp_int(n, 0, 1); }

inline float clamp01_float(float n) { return clamp_float(n, 0.0f, 1.0f); }

#endif  // FOOLRENDERER_MATH_MATH_UTILITY_H_
