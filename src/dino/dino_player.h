#pragma once

#include <dino/dino_main.h>
#include <dino/dino_entity.h>

namespace dino
{
// Représente un joueur.
class Player : public Entity
{
public:
    // Player's lasso is public for scene to handle cutting (and points)
    std::vector<Vec2> m_lassoPoints = {};
    std::vector<double> m_lassoPointsSpawnTime = {};

    // Player's input device
    jv::input::GamepadIdx m_gamepadIdx;

    int m_score = 0;

    /// Initialise le joueur.
    Player(jv::input::GamepadIdx gamepadIdx, Vec2 pos, int32_t kind);

    /// Déplace le jouer et met à jour son animation.
    void Update(Scene& scene, double absTime, float deltaTime) override;

    /// Affiche le joueur
    void Draw() const override;

    // Stun le joueur
    void OnLassoHit(Player& player_origin, Scene& scene) override;

    /// Détruit les ressources associées au joueur.
    ~Player();

protected:
    void _HandleTerrainCollision(Terrain const& terrain) override;

private:
    double m_lastLassoPointTime = DBL_MIN;
    // The last known direction of the player for animations
    Vec2 m_lastDir = {};
    // Time left for the stun
    double m_hitDuration = DBL_MIN;
    // Get player input and calculate direction and speed (taking into account if the player is hit)
    void _BuildVelocity();
    // All computation regarding the laso
    void _HandleLasso(double absTime);

    // Constant Game settings
    const float g_basePlayerSpeed = 60;
    const float g_basePlayerStunDuration = 3;
    const float g_lassoPointsDeltaTime = 0.01f;
    const float g_lassoPointsLifeTime = 5.0f;
    const float g_lassoWidth = 6.0f;
};

} // namespace dino