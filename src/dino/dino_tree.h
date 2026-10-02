#pragma once

#include <dino/dino_entity.h>

namespace dino
{
class Scene;
class Player;
class Tree : public Entity
{
public:
    Tree(double absTime, int32_t season);
    void Update(Scene& scene, double absTime, float deltaTime) override;

    /// Affiche l'arbre
    void Draw() const override;

    // Lance la partie
    void OnLassoHit(Player& player_origin, Scene& scene) override;

protected:
    void _HandleTerrainCollision(Terrain const& terrain) override;

private:
    uint8_t m_alpha = 128;
    int32_t m_season;
    double m_spawnTime;

    const float g_spawnDuration = 5.0f;
};
} // namespace dino