
#pragma once

#include <dino/Entity.h>
#include <dino/dino_main.h>
#include <dino/dino_terrain.h>
#include <dino/Entity.h>

#include <vector>

namespace dino
{

// Représente un joueur.
class Player : public Entity
{
public:
    Player(
        Vec2 pos, int32_t idxPlayer, jv::input::GamepadIdx gamepadIdx, jv::gpu::Texture* pTexture);

    /// Constructeur d'un Player à partir d'un autre Player par copie
    ///
    /// "Player a = b;" ou "Player a{b};" (on construit "a" à partir de "b")
    Player(Player const& other) = delete;
    Player& operator=(Player const& other) = delete;

    /// Opérateur d'assignement par copie
    ///
    /// Player a, b;
    /// a = b; // Pas un constructeur, car 'a' et 'b' existe déjà ; c'est une affectation
    Player& operator=(Player const& b) = delete;

    /// Déplace le joueur et met à jour son animation.
    void Update(double absTime, float deltaTime);

    /// Affiche le joueur
    void Draw() const override;

    void Join(Vec2 pos);
    void Leave();
    bool IsConnected() const
    {
        return m_isConnected;
    }

    void AddScore(int32_t points)
    {
        m_score += points;
    }
    int32_t GetScore() const
    {
        return m_score;
    }

    void ClearLasso();
    std::vector<Vec2> const& GetLassoPoints() const
    {
        return m_lassoPoints;
    }
    std::vector<Vec2> TakeCompletedLoop();

    ~Player();

private:
    int32_t m_idxPlayer = 0;
    jv::input::GamepadIdx m_gamepadIdx;
    Vec2 m_dir{};
    jv::gpu::Texture* m_pTexture = nullptr;
    double m_absTime = 0;
    double m_hitTime = 0;
    bool m_bRunning = false;
    bool m_goingLeft = false;
    bool m_isConnected = false;
    int32_t m_score = 0;

    std::vector<Vec2> m_lassoPoints;
    std::vector<double> m_lassoTimes;
    std::vector<Vec2> m_completedLoop;

    void _UpdateLasso(double absTime);
};

} // namespace dino