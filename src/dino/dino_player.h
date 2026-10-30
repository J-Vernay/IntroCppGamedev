#pragma once

#include <dino/dino_main.h>
#include <dino/dino_terrain.h>
#include <dino/dino_entity.h>
#include <deque>

namespace dino
{
struct TrailPoint
{
    Vec2 pos;
    double time;
};
// Représente un joueur.
class Player : public Entity
{
public:
    /// Initialise le joueur avec un type au hasard.
    Player(
        Vec2 pos, int32_t idxPlayer, jv::input::GamepadIdx gamepadIdx, jv::gpu::Texture* pTexture);

    /// Constructeur d'un Player à partir d'un autre Player par copie
    ///
    /// "Player a = b;" ou "Player a{b};" (on construit "a" à partir de "b")
    Player(Player const& other) = delete;

    /// Opérateur d'assignement par copie
    ///
    /// Player a, b;
    /// a = b; // Pas un constructeur, car 'a' et 'b' existe déjà ; c'est une affectation
    Player& operator=(Player const& b) = delete;

    /// Déplace le joueur et met à jour son animation.
    void Update(double absTime, float deltaTime);

    /// Affiche le joueur
    void Draw() const override;

    /// Détruit les ressources associées au joueur.
    ~Player();


    
    // trail
    
    std::deque<TrailPoint> const& GetTrail() const
    {
        return m_trail;
    }
    void ClearTrail()
    {
        m_trail.clear();
    }
    void SetTrail(std::deque<TrailPoint> const& newTrail)
    {
        m_trail = newTrail;
    }

    bool IsActive() const
    {
        return m_bActive;
    }
    void SetActive(bool active)
    {
        m_bActive = active;
    }

    // score
    int GetScore() const
    {
        return m_score;
    }
    void AddScore(int pts)
    {
        m_score += pts;
    }
    void ResetScore()
    {
        m_score = 0;
    }

    jv::util::Color GetColor() const
    {
        constexpr jv::util::Color playerColors[4] = {
            Color_BLUE,
            Color_RED,
            Color_YELLOW,
            Color_GREEN
        };
        return playerColors[m_idxPlayer % 4];
    }

private:
    int32_t m_idxPlayer;
    jv::input::GamepadIdx m_gamepadIdx;
    Vec2 m_dir;
    jv::gpu::Texture* m_pTexture;
    double m_hitTime = 0;
    double m_absTime = 0;
    bool m_bRunning = false;
    bool m_bLeft = false;
    std::deque<TrailPoint> m_trail;
    Vec2 m_lastTrailPos = {-1000, -1000};
    bool m_bActive = false;
    int m_score = 0;
};

} // namespace dino