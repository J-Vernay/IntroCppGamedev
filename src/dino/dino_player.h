#include <dino/dino_main.h>
#include <dino/dino_terrain.h>
#include <dino/dino_entity.h>
#pragma once

namespace dino
{

class Player : public Entity
{
public:

    Player(Vec2 pos, double absTime, jv::input::GamepadIdx gamepadIdx,
        jv::gpu::Texture* m_pTextureText, jv::gpu::Texture* pMainTex);

    void Update(double absTime, float deltaTime) override;

    void UpdateInput();
    void UpdateDirection(float x, float y);
    void UpdatePosition(float deltaTime);
    void UpdatePlayerState(float deltaTime);
    void UpdateLobbyState();
    void UpdateIndexFrame(double absTime);
    void UpdateSpeed(bool isRunning);

    void RequestPause(bool& gamePaused,float deltaTime);
    
    std::vector<Vec2> UpdateLasso();

    void HandleLassoCollision(int index)
    {
        if (index < 0 || index >= m_lassoVector.size()) return;
        m_lassoVector.erase(m_lassoVector.begin(), m_lassoVector.begin() + index);
    }

    virtual void ResolveTerrainPos(Terrain& terrain) override;

    void AddPoint(int points)
    {
        m_score += points;
    }

    void ResetPoint()
    {
        m_score = 0;
    }

    virtual void CatchByPlayer() override
    {
        if (m_currentPlayerState == Lobby)
            return;
        HandleHit();
    }

    bool IsInLobby()
    {
        return m_currentPlayerState == Lobby;
    }

    void HandleHit();
    void UpdateHurtState(float deltaTime);

    void Draw() const override;
    void DrawLasso() const;
    void DrawPlayer() const;
    void DrawScore() const;

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
        float SpeedWalking = 50;
        float SpeedRunning = 100;

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
        Lobby,
        Idle,
        Walking,
        Running,
        Hurt,
    };

    float m_timerHit = 0;
    float m_timerPause = 0.0f;

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

    int m_score = 0;

    jv::gpu::Texture* m_pTextureText;
};

} 