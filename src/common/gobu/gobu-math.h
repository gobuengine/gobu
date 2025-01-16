#ifndef GOBU_MATH_H
#define GOBU_MATH_H

#include "gobu-type.h"

go_public float go_math_clamp(float value, float min, float max);
go_public int go_math_random_int(int min, int max);
go_public go_vec2_t go_math_vec2_add(go_vec2_t a, go_vec2_t b);
go_public go_vec2_t go_math_vec2_scale(go_vec2_t a, float scale);
go_public go_vec2_t go_math_vec2_sub(go_vec2_t a, go_vec2_t b);

#endif // GOBU_MATH_H