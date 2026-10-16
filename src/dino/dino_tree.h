#pragma once

#include <dino/dino_entity.h>
#include <dino/dino_main.h>
#include <dino/dino_terrain.h>

namespace dino
{

// Représente un animal.
class Tree : public Entity
{
public:
    /// Initialise l'animal avec un type au hasard.
    Tree(Vec2 pos, double absTime, int32_t kind, jv::gpu::Texture* texture);

    /// Déplace l'animal et met à jour son animation.
    void Update(double absTime, float deltaTime, Terrain& terrain) override;

    /// Affiche l'animal
    void Draw() const override;

    int32_t OnCaughtInLoop(float absTime) override;

    void ResetTime(float absTime);

    bool ShouldStartGame();

    /// Détruit les ressources associées à l'animal.
    ~Tree();

private:
    static constexpr float InactiveTime = 5;
    double m_timeStart;
    uint8_t m_alpha = 0;
    int32_t m_kind;
    jv::gpu::Texture* m_pTexture;

protected:
    void OnOutsideTerrain() override;
};

} // namespace dino