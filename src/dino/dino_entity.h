#include <dino/dino_animal.h>
#include <dino/dino_draw_utils.h>
#include <math.h>
#pragma once

namespace dino
{

class Entity
{
public:
    Entity(Vec2 pos, double absTime, jv::gpu::Texture* texturePtr, Terrain* pTerrain);
    
    void Update(double absTime, float deltaTime);
    void Draw() const;
    ~Entity();
    Entity(const Entity&) = delete;

private:
    Vec2 m_pos;
    Vec2 m_dir;

    double m_timeStart;

    uint8_t m_alpha = 0;

    int32_t m_idxFrame = 0;

    jv::gpu::Texture* m_pTexture;

    Terrain* m_pTerrain;
};
} // namespace dino