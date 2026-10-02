#pragma once

#include <dino/dino_main.h>
#include <dino/dino_terrain.h>

class Scene;

namespace dino
{

class Entity
{
public:
    Entity(Vec2 pos);

    /// Déplace l'entité et met à jour son animation.
    virtual void Update(double absTime, float deltaTime, Terrain& terrain) = 0;

    /// Gère les collisions avec une autre entité.
    void Collide(Entity& player, Terrain& terrain);

    // Affiche l'entité.
    virtual void Draw() const = 0;

    Vec2 GetPos() const;
    Vec2 GetLastPos() const;
    virtual void OnCaughtInLoop() = 0;
    bool m_bCaughtInLoop = false;
    bool m_bShouldDie = false;

protected:
    void Move(Vec2 const amount, Terrain& terrain);

    virtual void OnOutsideTerrain() = 0;
    
    Vec2 m_pos;
    Vec2 m_lastPos;
};

}