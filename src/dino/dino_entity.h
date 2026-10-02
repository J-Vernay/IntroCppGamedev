#pragma once

#include <dino/dino_main.h>
#include <dino/dino_terrain.h>

namespace dino
{

class Entity
{
public:
    static void ResolveCollision(Entity& a, Entity& b);

    /// S'assurer que la position du joueur reste sur le terrain.
    void CheckTerrain(Terrain const& terrain);

protected:
    Vec2 m_pos;

    virtual void _ReactTerrain();
};

}