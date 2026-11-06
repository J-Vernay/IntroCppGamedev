#pragma once

#include "dino_entity.h"

#include <dino/dino_main.h>

namespace dino
{
class Terrain;


class Player : public Entity
{
public:
    Player(Vec2 pos, jv::gpu::Texture* pTexture, jv::input::GamepadIdx gamepadIdx, int32_t colorIndex);

    Player(const Player&) = delete;
    Player& operator=(const Player&) = delete;

    void Update(double absTime, float deltaTime) override;
    void Draw() const override;

    ~Player() override = default;

    void TakeDamage();
    bool IsHurt() const;

    void UpdateLasso(float deltaTime);
    void DrawLasso() const;

    bool CutLasso(Vec2 from, Vec2 to);

    const std::vector<Vec2>& GetClosedLoop() const{return m_closedLoop;}

    void OnCaptured() override;

    void AddScore(int points)
    {
        m_score += points;
    }

    int GetScore() const
    {
        return m_score;
    }

    int32_t GetColorIndex() const
    {
        return m_colorIndex;
    }



    void ResetState(bool resetScore);

    bool IsActive() const
    {
        return m_isActive;
    }

    jv::input::GamepadIdx GetGamepadIdx() const
    {
        return m_gamepadIdx;
    }

    void Join(jv::input::GamepadIdx device);
    void Leave();

private:
    Vec2 m_dir{0, 0};

    bool m_isMoving = false;
    bool m_isRunning = false;
    bool m_facingLeft = false;

    int32_t m_idxFrame = 0;

    jv::gpu::Texture* m_pTexture = nullptr;

    float m_hurtTimeRemaining = 0.f;
    bool m_damageButtonWasDown = false;

    jv::input::GamepadIdx m_gamepadIdx;
    int32_t m_colorIndex = 0;

    struct LassoPoint
    {
        Vec2 pos;
        double time;
    };

    std::vector<LassoPoint> m_lassoPoints;
    double m_lassoTime = 0.0;

    void _RemoveLassoLoop();

    std::vector<Vec2> m_closedLoop;

    int m_score = 0;


    bool m_isActive = false;
};



} 