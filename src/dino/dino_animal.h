#pragma once

#include <dino/dino_main.h>
#include <dino/dino_terrain.h>
#include <dino/dino_entity.h>

namespace dino
{

// Représente un animal.
class Animal : public Entity
{
public:
    /// Initialise l'animal avec un type au hasard.
    Animal(Vec2 pos, double absTime, jv::gpu::Texture* texture);
    
    /// Déplace l'animal et met à jour son animation.
    void Update(double absTime, float deltaTime, Terrain& terrain) override;

    /// Affiche l'animal
    void Draw() const override;

    void OnCaughtInLoop() override;

    /// Détruit les ressources associées à l'animal.
    ~Animal();

private:
    double m_timeStart;
    uint8_t m_alpha = 0;
    Vec2 m_dir;
    int32_t m_kind;
    int32_t m_idxFrame = 0;
    jv::gpu::Texture* m_pTexture;

protected:
    void OnOutsideTerrain() override;
};

} // namespace dino