// Copyright (c) Caden Ji. All rights reserved.
//
// Licensed under the MIT License. See LICENSE file in the project root for
// license information.

#include "math/math_utility.h"

#include <stdint.h>

extern int32_t int32_max(int32_t a, int32_t b);

extern int32_t int32_min(int32_t a, int32_t b);

extern int32_t int32_clamp(int32_t n, int32_t min, int32_t max);

extern int32_t int32_clamp01(int32_t n);

extern uint32_t uint32_max(uint32_t a, uint32_t b);

extern uint32_t uint32_min(uint32_t a, uint32_t b);

extern uint32_t uint32_clamp(uint32_t n, uint32_t min, uint32_t max);

extern uint32_t uint32_clamp01(uint32_t n);

extern float float_max(float a, float b);

extern float float_min(float a, float b);

extern float float_clamp(float n, float min, float max);

extern float float_clamp01(float n);

extern float float_lerp(float a, float b, float t);

extern uint8_t float_to_uint8(float value);

extern float uint8_to_float(uint8_t value);
