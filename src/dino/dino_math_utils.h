#pragma once

#include <jv/jv.h>

namespace dino
{
namespace math
{
float Distance(jv::util::Vec2 a, jv::util::Vec2 b);
jv::util::Vec2 VectorSubtract(jv::util::Vec2 a, jv::util::Vec2 b);
jv::util::Vec2 NormalizeVector(jv::util::Vec2 a);
}

}