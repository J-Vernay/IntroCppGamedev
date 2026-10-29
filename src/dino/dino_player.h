#pragma once

#include <dino/Entity.h>
#include <dino/dino_main.h>
#include <dino/dino_terrain.h>

#include <vector>

namespace dino
{

// Représente un joueur.
class Player : public Entity
{
public:
    Player(
        Vec2 pos, int32_t idxPlayer, jv::input::GamepadIdx gamepadIdx, jv::gpu::Texture* pTexture);

    Player(Player const& other) = delete;
    Player& operator=(Player const& other) = delete;

    void Update(double absTime, float deltaTime);
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