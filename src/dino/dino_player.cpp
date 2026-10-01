#include "dino_player.h"

#include <dino/dino_animal.h>
#include <dino/dino_draw_utils.h>
#include <math.h>

dino::Player::Player(Vec2 pos, double absTime, jv::gpu::Texture* texture)
{
    m_pos = pos;
    m_kind = jv::util::RandomInt32(0, 7);
    m_timeStart = absTime;
    m_pTexture = texture;
    m_dir = jv::util::RandomRotate({1, 0}, 0, 360);
}

dino::Player::~Player()
{
    jv::gpu::DestroyTexture(m_pTexture);
}

void dino::Player::Update(double absTime, float deltaTime)
{
    float speed = 100;
    m_absTime = absTime;

    jv::input::Gamepad keyboard;
    if (jv::input::GetGamepad(jv::input::GamepadIdx::Keyboard, keyboard))
    {
        Vec2 dir = Vec2(0, 0);
        if (keyboard.dpad_up)
            dir.y -= 1;
        if (keyboard.dpad_down)
            dir.y += 1;
        if (keyboard.dpad_right)
            dir.x += 1;
        if (keyboard.dpad_left)
            dir.x -= 1;

        m_dir = dir;

        if (keyboard.btn_right)
        {
            m_bRunning = true;
            speed *= 2;
        }
        else
        {
            m_bRunning = false;
        }
            
        // m_pos = Vec2((dir.x + m_pos.x) * 1 * deltaTime, (dir.y+ m_pos.y) * 1 * deltaTime);
    }

    m_pos.x += m_dir.x * speed * deltaTime;
    m_pos.y += m_dir.y * speed * deltaTime;

    m_idxFrame = int32_t(absTime * 8) % 4;

    double aliveTime = absTime - m_timeStart;
    if (aliveTime < 1)
        m_alpha = uint8_t(UINT8_MAX * aliveTime);
}

void dino::Player::Draw() const
{
    float u1 = 0, u2 = 24, v1 = 0, v2 = 24;
    float m_idxFrame = int32_t(m_absTime * 8) % 4;

    if (m_dir.x != 0.0f)
    {
        u1 = 24;
        u2 = 48;
        m_idxFrame = int32_t(m_absTime * 8) % 6;
    }
    if (m_bRunning)
    {
        u1 = 432;
        u2 = 432 + 24;
        m_idxFrame = int32_t(m_absTime * 16) % 6;

    }

    if (m_dir.x < 0)
    {
        std::swap(u1, u2);
    }

    u1 += 24 * m_idxFrame;
    u2 += 24 * m_idxFrame;

    jv::util::Color color = Color_WHITE;
    color.a = m_alpha;

    std::vector<jv::gpu::Vertex> vs;
    vs.emplace_back(Vec2{m_pos.x - 16, m_pos.y - 32}, Vec2{u1, v1}, color);
    vs.emplace_back(Vec2{m_pos.x + 16, m_pos.y - 32}, Vec2{u2, v1}, color);
    vs.emplace_back(Vec2{m_pos.x - 16, m_pos.y}, Vec2{u1, v2}, color);
    vs.emplace_back(Vec2{m_pos.x + 16, m_pos.y - 32}, Vec2{u2, v1}, color);
    vs.emplace_back(Vec2{m_pos.x - 16, m_pos.y}, Vec2{u1, v2}, color);
    vs.emplace_back(Vec2{m_pos.x + 16, m_pos.y}, Vec2{u2, v2}, color);

    jv::gpu::VertexBuffer* pVBuf = jv::gpu::CreateVertexBuffer("Animal", vs);
    jv::gpu::Draw(pVBuf, m_pTexture);
    jv::gpu::DestroyVertexBuffer(pVBuf);
}
