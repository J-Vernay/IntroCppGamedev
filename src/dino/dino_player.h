#include <dino/dino_main.h>
#include <dino/dino_terrain.h>
#include <dino/dino_entity.h>
#pragma once

namespace dino
{

class Player : public Entity
{
public:

    Player(Vec2 pos, double absTime, jv::input::GamepadIdx gamepadIdx);

    Player(const Player&) = delete;

    void Update(double absTime, float deltaTime) override;

    void UpdateInput();
    void UpdateDirection(float x, float y);
    void UpdatePosition(float deltaTime);
    void UpdatePlayerState(float deltaTime);
    void UpdateIndexFrame(double absTime);
    void UpdateSpeed(bool isRunning);
    
    std::vector<Vec2> UpdateLasso();

    void HandleLassoCollision(int index)
    {
        if (index < 0 || index >= m_lassoVector.size()) return;
        m_lassoVector.erase(m_lassoVector.begin(), m_lassoVector.begin() + index);
    }

    virtual void ResolveTerrainPos(Terrain& terrain) override;

    virtual void CatchByPlayer() override
    {
        HandleHit();
    }

    void HandleHit();
    void UpdateHurtState(float deltaTime);

    void Draw() const override;
    void DrawLasso() const;
    void DrawPlayer() const;

    int GetId() const
    {
        return id;
    }

    Vec2 GetLassoPosByIndex(int index) const
    {
        if (index < 0 || index >= m_lassoVector.size())
            return m_pos;
        return m_lassoVector[index];
    }

    Vec2 GetLassoLastPos() const
    {
        if (m_lassoVector.empty())
            return m_pos;
        return m_lassoVector.back();
    }

    int GetLassoSize() const
    {
        return static_cast<int>(m_lassoVector.size());
    }

    ~Player();

private:

    struct SpeedPlayerData
    {
        float CurrentSpeed = 30;
        const float SpeedWalking = 50;
        const float SpeedRunning = 100;

        bool IsRunning() const
        {
            return CurrentSpeed == SpeedRunning;
        }   

        SpeedPlayerData()
        {
            CurrentSpeed = SpeedWalking;
        }
    };

    jv::util::Color m_playerColor;

    jv::util::Color m_playersColors[4] = {
        Color_BLUE,
        Color_RED,
        Color_YELLOW,
        Color_GREEN
    };

    enum PlayerState
    {
        Idle,
        Walking,
        Running,
        Hurt,
    };

    float m_timerHit = 0;

    int32_t m_playerColorIndex;

    SpeedPlayerData m_speedData;
    PlayerState m_currentPlayerState = PlayerState::Idle;

    bool isMoving() const
    {
        return m_dir.x != 0 || m_dir.y != 0;
    }

    jv::input::GamepadIdx m_gamepadIdx;
    int id;

    std::vector<Vec2> m_lassoVector;
};

} 