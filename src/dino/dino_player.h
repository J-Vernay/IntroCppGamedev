#include <dino/dino_main.h>
#include <dino/dino_terrain.h>
#pragma once

namespace dino
{

class Player
{
public:

    Player(Vec2 pos, double absTime, jv::input::GamepadIdx gamepadIdx, Terrain* pTerrain);

    void Update(double absTime, float deltaTime);

    void UpdateInput();
    void UpdateDirection(float x, float y);
    void UpdatePosition(float deltaTime);
    void UpdatePlayerState(float deltaTime);
    void UpdateIndexFrame(double absTime);
    void UpdateSpeed(bool isRunning);
    void HandleHit();
    void UpdateHurtState(float deltaTime);

    void Draw() const;

    ~Player();

private:

    struct SpeedPlayerData
    {
        float CurrentSpeed = 30;
        const float SpeedWalking = 30;
        const float SpeedRunning = 60;

        bool IsRunning() const
        {
            return CurrentSpeed == SpeedRunning;
        }   

        SpeedPlayerData()
        {
            CurrentSpeed = SpeedWalking;
        }
    };

    enum PlayerState
    {
        Idle,
        Walking,
        Running,
        Hurt,
    };

    Vec2 m_pos;
    Vec2 m_dir;

    double m_timeStart;

    float m_timerHit = 0;

    uint8_t m_alpha = 0;

    int32_t m_playerColorIndex;
    int32_t m_idxFrame = 0;

    SpeedPlayerData m_speedData;
    PlayerState m_currentPlayerState = PlayerState::Idle;

    bool isMoving() const
    {
        return m_dir.x != 0 || m_dir.y != 0;
    }

    jv::input::GamepadIdx m_gamepadIdx;
    jv::gpu::Texture* m_pTexture;

    Terrain* m_pTerrain;
};

} 