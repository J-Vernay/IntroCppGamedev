#include <math.h>
#pragma once

namespace dino
{

class Entity
{
public:
    Entity(Vec2 pos, double absTime, Terrain* pTerrain) : m_pos(pos), m_timeStart(absTime), m_pTerrain(pTerrain) {};
    
    virtual void Update(double absTime, float deltaTime) = 0;
    virtual void Draw() const = 0;
    virtual ~Entity() {};

    Entity(const Entity&) = delete;

protected:
    Vec2 m_pos = {0, 0};
    Vec2 m_dir = {0, 0};

    double m_timeStart = 0;

    uint8_t m_alpha = 0;

    int32_t m_idxFrame = 0;

    jv::gpu::Texture* m_pTexture;

    Terrain* m_pTerrain;
};
} // namespace dino