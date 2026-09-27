#include <jv/jv.h>

#include <numbers>
#include <random>

static std::random_device g_randomDevice;
static std::mt19937 g_rng(g_randomDevice());

uint32_t jv::util::RandomUint32(uint32_t min, uint32_t max) noexcept
{
    std::uniform_int_distribution<uint32_t> distribution(min, max);
    return distribution(g_rng);
}

int32_t jv::util::RandomInt32(int32_t min, int32_t max) noexcept
{
    std::uniform_int_distribution<int32_t> distribution(min, max);
    return distribution(g_rng);
}

float jv::util::RandomFloat(float min, float max) noexcept
{
    std::uniform_real_distribution<float> distribution(min, max);
    return distribution(g_rng);
}

jv::util::Vec2 jv::util::RandomRotate(Vec2 vec, float angleMin, float angleMax) noexcept
{
    std::uniform_real_distribution<float> distribution(angleMin, angleMax);
    float angleDeg = distribution(g_rng);
    float angleRad = angleDeg * std::numbers::pi_v<float> / 180;
    float cos = cosf(angleRad);
    float sin = sinf(angleRad);
    Vec2 res;
    res.x = cos * vec.x - sin * vec.y;
    res.y = sin * vec.x + cos * vec.y;
    return res;
}
