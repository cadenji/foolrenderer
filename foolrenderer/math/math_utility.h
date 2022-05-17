// Copyright (c) Caden Ji. All rights reserved.
//
// Licensed under the MIT License. See LICENSE.txt in the project root for
// license information.

#ifndef FOOLRENDERER_MATH_MATH_UTILITY_H_
#define FOOLRENDERER_MATH_MATH_UTILITY_H_

#include <stdint.h>

#define SMALL_ABSOLUTE_FLOAT (1.e-8f)
#define PI (3.1415926535897932f)

inline int32_t int32_max(int32_t a, int32_t b) { return a > b ? a : b; }

inline int32_t int32_min(int32_t a, int32_t b) { return a < b ? a : b; }

inline int32_t int32_clamp(int32_t n, int32_t min, int32_t max) {
    return int32_min(int32_max(n, min), max);
}

inline int32_t int32_clamp01(int32_t n) { return int32_clamp(n, 0, 1); }

inline uint32_t uint32_max(uint32_t a, uint32_t b) { return a > b ? a : b; }

inline uint32_t uint32_min(uint32_t a, uint32_t b) { return a < b ? a : b; }

inline uint32_t uint32_clamp(uint32_t n, uint32_t min, uint32_t max) {
    return uint32_min(uint32_max(n, min), max);
}

inline uint32_t uint32_clamp01(uint32_t n) { return uint32_clamp(n, 0, 1); }

inline float float_max(float a, float b) { return a > b ? a : b; }

inline float float_min(float a, float b) { return a < b ? a : b; }

inline float float_clamp(float n, float min, float max) {
    return float_min(float_max(n, min), max);
}

inline float float_clamp01(float n) { return float_clamp(n, 0.0f, 1.0f); }

inline uint8_t float_to_uint8(float value) { return value * 0xFF; }

inline float uint8_to_float(uint8_t value) { return value / 255.0f; }

#endif  // FOOLRENDERER_MATH_MATH_UTILITY_H_
