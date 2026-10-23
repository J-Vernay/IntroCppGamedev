#pragma once

#include <dino/dino_main.h>
#include <dino/dino_terrain.h>
#include <dino/Movable.h>

namespace dino
{

// Représente un animal.
class Animal : public Movable
{
public:
    /// Initialise l'animal avec un type au hasard.
    Animal(Vec2 pPos, double pAbsTime, jv::gpu::Texture* pTexture);
    
    /// Déplace l'animal et met à jour son animation.
    void Update(double pAbsTime, float pDeltaTime);

    /// Affiche l'animal
    void Draw() const override;

private:
    uint8_t m_alpha = 0;
    int32_t m_kind;
    int32_t m_idxFrame;

    //changement de direction lorsque atteint la limite du terrain
    void _ReactTerrain() override;
};

} 