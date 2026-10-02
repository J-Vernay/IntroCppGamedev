#pragma once

#include <dino/dino_main.h>

namespace dino
{

// Represente un animal.
class Player
{
public:
    Player(Vec2 pos, double absTime, jv::gpu::Texture* pTexture, int kind);

    void Update(double absTime, float deltaTime);

    void Draw() const;

    ~Player();

private:
    Vec2 m_pos;
    double m_timeStart;
    uint8_t m_alpha = 0;
    Vec2 m_dir;
    int32_t m_kind;
    jv::gpu::Texture* m_pTexture;
    jv::input::GamepadIdx m_gamepadIdx;

    double m_absTime;
    bool m_bRunning;
    bool m_bLeft;
    bool m_bStun = false;
    float m_stunTime;
};

} // namespace dino