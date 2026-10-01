#pragma once

#include <dino/dino_main.h>
#include <dino/dino_terrain.h>

namespace dino
{

class Entity
{
public:
    Entity(Terrain* terrain, Vec2 pos);

    /// Déplace l'entité et met à jour son animation.
    virtual void Update(double absTime, float deltaTime) = 0;

    /// Gère les collisions avec une autre entité.
    void Collide(Entity& player);

    // Affiche l'entité.
    virtual void Draw() const = 0;

    Vec2 GetPos() const;
    Vec2 GetLastPos() const;

protected:
    void Move(Vec2 const amount);

    virtual void OnOutsideTerrain() = 0;
    
    Vec2 m_pos;
    Terrain* m_terrain;
    Vec2 m_lastPos;
};

}