#pragma once

#include <dino/dino_main.h>
#include <dino/dino_terrain.h>

namespace dino
{
// Forward declaration
class Scene;
class Player;
class Entity
{
public:
    // Public since scene handle collision displacement
    Vec2 m_pos;
    float m_collisionRadius = 8;

    virtual void Update(Scene& scene, double absTime, float deltaTime) = 0;

    virtual void Draw() const = 0;

    virtual void OnLassoHit(Player& player_origin, Scene& scene) = 0;

protected:
    /// Code to play when entity gets out of the terrain
    virtual void _HandleTerrainCollision(Terrain const& terrain) = 0;
    void _Move(Scene const& scene, float deltaTime);

    int32_t m_idxFrame;
    /// <summary>
    /// Changes the visuals of the entity
    /// Animal: 0-7
    /// Player: 0-3
    /// </summary>
    int32_t m_kind;
    float m_speed = 0;
    Vec2 m_dir = {};
};
}