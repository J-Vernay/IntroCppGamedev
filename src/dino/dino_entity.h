#pragma once

#include <dino/dino_main.h>
#include <dino/dino_terrain.h>

namespace dino
{
class Entity
{
public:
    Entity(Vec2 pos, double absTime) : m_pos(pos), m_timeStart(absTime) {};

    virtual void Update(double absTime, float deltaTime) = 0;
    virtual void Draw() const = 0;

    virtual ~Entity() {};

    Entity(const Entity&) = delete;

    Vec2 GetPosition()
    {
        return m_pos;
    };

    void HandlePhysics(std::vector<Entity*> entitys);

protected:
    Vec2 m_pos;
    double m_timeStart;
    uint8_t m_alpha = 0;
    Vec2 m_dir;
    int32_t m_idxFrame;
    jv::gpu::Texture* m_pTexture;
    dino::Terrain* m_pTerrain;
};
} // namespace dino