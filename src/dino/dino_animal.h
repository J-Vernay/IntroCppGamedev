#pragma once

#include <dino/dino_main.h>
#include <dino/dino_terrain.h>
#include <dino/dino_entity.h>

namespace dino
{

class Animal : public Entity
{
public:
    Animal(Vec2 pos, double absTime, jv::gpu::Texture* tex);
    
    void Update(double absTime, float deltaTime) override;

    void Draw() const override;

    ~Animal();

    Animal(const Animal&) = delete;

    void HandleTerrainClamp(dino::Terrain* terrain) override;

private:
    int32_t m_kind;
};

} // namespace dino