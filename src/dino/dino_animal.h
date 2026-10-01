#pragma once

#include <dino/dino_main.h>
#include <dino/dino_terrain.h>
#include <dino/dino_entity.h>

namespace dino
{

class Animal : Entity
{
public:
    Animal(Vec2 pos, double absTime, jv::gpu::Texture* tex, dino::Terrain* terrain);
    
    void Update(double absTime, float deltaTime) override;

    void Draw() const override;

    ~Animal();

    Animal(const Animal&) = delete;

private:
    int32_t m_kind;
};

} // namespace dino