#include <dino/dino_math_utils.h>
#include <math.h>

float dino::math::Distance(jv::util::Vec2 a, jv::util::Vec2 b)
{
    return hypotf(b.y - a.y, b.x - a.x);
}

jv::util::Vec2 dino::math::VectorSubtract(jv::util::Vec2 a, jv::util::Vec2 b)
{
    return {a.x - b.x, a.y - b.y};
}

jv::util::Vec2 dino::math::NormalizeVector(jv::util::Vec2 a)
{
    float length = dino::math::Distance({0, 0}, a);
    return { a.x / length, a.y / length};
}