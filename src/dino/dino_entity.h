#pragma once

#include <dino/dino_main.h>
#include <dino/dino_terrain.h>

namespace dino
{
// Forward declaration
class Scene;
class Entity
{
public:
    // Public since scene handle collision displacement
    Vec2 m_pos;
    float m_collisionRadius = 8;

    virtual void Update(Scene& scene, double absTime, float deltaTime) = 0;

    virtual void Draw() const = 0;

protected:
    /// Code to play when entity gets out of the terrain
    virtual void _HandleTerrainCollision(Terrain& terrain) = 0;
    void _Move(Scene& scene, float deltaTime);

    int32_t m_idxFrame;
    int32_t m_kind;
};
}