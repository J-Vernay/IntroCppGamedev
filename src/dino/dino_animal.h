#pragma once

#include <dino/dino_main.h>
#include <dino/dino_terrain.h>
#include <dino/dino_entity.h>

namespace dino
{

class Animal : public Entity
{
public:

    Animal(Vec2 pos, double absTime,jv::gpu::Texture* textureAnimalPtr);
    
    void Update(double absTime, float deltaTime) override;

    void Draw() const override;

    virtual void ResolveTerrainPos(Terrain& terrain) override;

    ~Animal();

    Animal(const Animal&) = delete;

private:

    int32_t m_kind;
};

} 