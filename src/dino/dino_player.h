#pragma once

#include <dino/dino_main.h>
#include <dino/dino_terrain.h>
#include <dino/dino_entity.h>

namespace dino
{

class Player : public Entity
{
public:

    Player(Vec2 pos, double absTime, jv::input::GamepadIdx gamepadIdx, int color);


    void Update(double absTime, float deltaTime) override;


    void Draw() const override;

    void TakeDamage(double absTime);

    std::vector<Vec2>* GetLassoPoints();

    void HandleTerrainClamp(dino::Terrain* terrain) override;

    void HandleCapture() override;

    void HandleLasso();

    Vec2 GetPos();
    Vec2 GetLastPos();

    void HandleLassoOpponentCollision(int n);
    void HandleLassoColision(int n);


    ~Player();

    Player(const Player&) = delete;

private:
    void UpdateFrameRate(double absTime);
    void UpdateInputs(double absTime, float deltaTime);
    void DrawLasso() const;

    void DetectLassoColision();

    enum playerState
    {
        idle,
        walk,
        run,
        damage
    };

    Color playerColors[4]{Color_BLUE, Color_RED, Color_YELLOW, Color_GREEN

    };

    int m_playerIndex;
    Color m_color;

    Vec2 m_lastPos;

    std::vector<Vec2> m_lassoPoints;
    std::vector<jv::gpu::Vertex> m_lassoVertices;
    playerState m_state;
    double m_lastDamageTime;
    double m_stunDuration;
    int32_t m_kind;
    jv::input::GamepadIdx m_gamepad;
    bool hasTookDamage = false;
};

} // namespace dino