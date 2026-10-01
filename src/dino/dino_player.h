#pragma once

#include <dino/dino_main.h>
#include <dino/dino_terrain.h>
#include <dino/dino_entity.h>

namespace dino
{

class Player : public Entity
{
public:

    Player(Vec2 pos, double absTime, jv::input::GamepadIdx gamepadIdx, int color, dino::Terrain* terrain);


    void Update(double absTime, float deltaTime) override;


    void Draw() const override;

    void TakeDamage(double absTime);


    ~Player();

    Player(const Player&) = delete;

private:
    enum playerState
    {
        idle,
        walk,
        run,
        damage
    };

    playerState m_state;
    double m_lastDamageTime;
    double m_stunDuration;
    int m_color;
    int32_t m_kind;
    jv::input::GamepadIdx m_gamepad;
};

} // namespace dino