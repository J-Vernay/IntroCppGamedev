#pragma once

#include <dino/dino_main.h>
#include <dino/dino_terrain.h>

namespace dino
{

class Entity
{
public:
    static void ResolveCollision(Entity& a, Entity& b);

    void CheckTerrain(Terrain const& terrain);

protected:
    Vec2 m_pos;

      virtual void _ReactTerrain();
};

} // namespace dino