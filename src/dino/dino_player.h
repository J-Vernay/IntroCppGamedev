#pragma once

#include <dino/dino_main.h>
#include <dino/dino_entity.h>

namespace dino
{
// Représente un joueur.
class Player : public Entity
{
public:
    /// Initialise le joueur.
    Player(jv::input::GamepadIdx gamepadIdx, Vec2 pos, int32_t kind);

    /// Déplace le jouer et met à jour son animation.
    void Update(Scene& scene, double absTime, float deltaTime) override;

    /// Affiche le joueur
    void Draw() const override;

    /// Détruit les ressources associées au joueur.
    ~Player();

protected:
    void _HandleTerrainCollision(Terrain& terrain) override;

private:
    jv::input::GamepadIdx m_gamepadIdx;
    uint8_t m_alpha = UINT8_MAX;
    Vec2 m_lastDir = {};

    double m_hitDuration = DBL_MIN;
    // Get player input and calculate direction and speed (taking into account if the player is hit)
    void _BuildVelocity();

    const float g_basePlayerSpeed = 30;
    const float g_basePlayerStunDuration = 3;
};

} // namespace dino