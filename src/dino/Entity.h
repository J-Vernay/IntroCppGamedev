#pragma once

#include <dino/dino_main.h>
#include <dino/dino_terrain.h>

namespace dino
{

class Entity
{
public:
    virtual ~Entity() = default;

    static void ResolveCollision(Entity& a, Entity& b);

    void CheckTerrain(Terrain const& terrain);

    Vec2 GetPos() const
    {
        return m_pos;
    }

    virtual void Caught(){}

protected:
    Vec2 m_pos = {0, 0};

    virtual void _ReactTerrain();
};

} // namespace dino