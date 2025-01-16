#include "gobu-math.h"

go_public float go_math_clamp(float value, float min, float max)
{
    if (value < min)
        return min;
    if (value > max)
        return max;
    return value;
}

go_public int go_math_random_int(int min, int max)
{
    return min + rand() % (max - min + 1);
}

go_public go_vec2_t go_math_vec2_add(go_vec2_t a, go_vec2_t b)
{
    return (go_vec2_t){a.x + b.x, a.y + b.y};
}

go_public go_vec2_t go_math_vec2_scale(go_vec2_t a, float scale)
{
    return (go_vec2_t){a.x * scale, a.y * scale};
}

go_public go_vec2_t go_math_vec2_sub(go_vec2_t a, go_vec2_t b)
{
    return (go_vec2_t){a.x - b.x, a.y - b.y};
}
