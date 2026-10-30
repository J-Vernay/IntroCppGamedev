#pragma once
#include "jv/jv.h"

namespace jv::physics
{
    using Vec2 = jv::util::Vec2;

class IRestrictedSpace
{
public:
    virtual ~IRestrictedSpace() = default;

    /// Transforme la position 'pos' pour s'assurer qu'elle reste dans cet espace
    virtual Vec2 ClampPos(Vec2 pos) const = 0;

    /// Génère une position aléatoire dans cet espace
    virtual Vec2 GenerateRandomSpawn() const = 0;
};
} // namespace jv::physics