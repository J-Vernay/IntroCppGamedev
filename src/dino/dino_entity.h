#pragma once

#include <dino/dino_main.h>
#include <dino/dino_terrain.h>

namespace dino
{

class Entity
{
public:
    Entity(Terrain* terrain, Vec2 pos);

    /// Gère les collisions avec une autre entité.
    void Collide(Entity& player);

    void Push(Vec2 const amount);

    Vec2 GetPos() const;

protected:
    Vec2 m_pos;
    Terrain* m_terrain;

    virtual void OnOutsideTerrain() = 0;
};

}