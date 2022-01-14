// Copyright (c) Caden Ji. All rights reserved.
//
// Licensed under the MIT License. See LICENSE.txt in the project root for
// license information.

#ifndef FOOLRENDERER_MATH_MATH_UTILITY_H_
#define FOOLRENDERER_MATH_MATH_UTILITY_H_

#include <stdint.h>

#define SMALL_ABSOLUTE_FLOAT (1.e-8f)

#define PI (3.1415926535897932f)
#define HALF_PI (1.57079632679f)
#define INVERSE_PI (0.31830988618f)

inline int32_t max_int32(int32_t a, int32_t b) { return a > b ? a : b; }

inline int32_t min_int32(int32_t a, int32_t b) { return a < b ? a : b; }

inline int32_t clamp_int32(int32_t n, int32_t min, int32_t max) {
    return min_int32(max_int32(n, min), max);
}

inline int32_t clamp01_int32(int32_t n) { return clamp_int32(n, 0, 1); }

inline uint32_t max_uint32(uint32_t a, uint32_t b) { return a > b ? a : b; }

inline uint32_t min_uint32(uint32_t a, uint32_t b) { return a < b ? a : b; }

inline uint32_t clamp_uint32(uint32_t n, uint32_t min, uint32_t max) {
    return min_uint32(max_uint32(n, min), max);
}

inline uint32_t clamp01_uint32(uint32_t n) { return clamp_uint32(n, 0, 1); }

inline float max_float(float a, float b) { return a > b ? a : b; }

inline float min_float(float a, float b) { return a < b ? a : b; }

inline float clamp_float(float n, float min, float max) {
    return min_float(max_float(n, min), max);
}

inline float clamp01_float(float n) { return clamp_float(n, 0.0f, 1.0f); }

#endif  // FOOLRENDERER_MATH_MATH_UTILITY_H_
