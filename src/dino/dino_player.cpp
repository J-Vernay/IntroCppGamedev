#include "dino_player.h"

#include <dino/dino_animal.h>
#include <dino/dino_draw_utils.h>
#include <dino/dino_geometry.h>
#include <iostream>
#include <windows.h>
#include <math.h>

dino::Player::Player(Vec2 pos, double absTime, jv::gpu::Texture* texture, int pindxPlayer,
    jv::input::GamepadIdx pgamepadIdx, Color pColor)
{
    m_gamepadIdx = pgamepadIdx;
    m_idxPlayer = pindxPlayer;
    m_pos = pos;
    m_dir = jv::util::RandomRotate({1, 0}, 0, 360);
    m_timeStart = absTime;
    m_pTexture = texture;
    m_color = pColor;
}

void dino::Player::Update(double absTime, float deltaTime)
{
    float speed = 100;
    m_absTime = absTime;

    jv::input::Gamepad keyboard;
    Vec2 dir = Vec2(0, 0);
    if (jv::input::GetGamepad(m_gamepadIdx, keyboard))
    {
        if (keyboard.dpad_up)
            dir.y -= 1;
        if (keyboard.dpad_down)
            dir.y += 1;
        if (keyboard.dpad_right)
            dir.x += 1;
        if (keyboard.dpad_left)
            dir.x -= 1;

        if (keyboard.btn_right)
        {
            m_bRunning = true;
            speed *= 2;
        }
        else
        {
            m_bRunning = false;
        }
        if (keyboard.btn_left)
        {
            m_timerStun = 3;
        }
    }

    m_dir = dir;

    if (m_timerStun <= 0)
    {
        m_pos.x += dir.x * speed * deltaTime;
        m_pos.y += dir.y * speed * deltaTime;

       Trail(deltaTime);
    }
    else
    {
        m_timerStun -= deltaTime;
    }

    double aliveTime = absTime - m_timeStart;
    if (aliveTime < 1)
        m_alpha = uint8_t(UINT8_MAX * aliveTime);
}

void dino::Player::Trail(float deltaTime)
{
    if(!m_points.empty() && m_points.back().x == m_pos.x && m_points.back().y == m_pos.y) return;

    if (m_points.size() >= 3)
    {
        Vec2 lastPoint = m_points.back();

        for (size_t i = 0; i + 2 < m_points.size(); i++)
        {
            if (dino::IntersectSegment(m_points[i], m_points[i + 1], lastPoint, m_pos))
            {
                m_points.erase(m_points.begin() + i, m_points.end() - 1);
                break;
            }
        }
    }

    m_points.push_back(m_pos);

    m_timerTrail += deltaTime;
    if (m_points.size() >= 120)
    {
        OutputDebugStringA("hello");
        m_points.erase(m_points.begin());
    }
}

void dino::Player::Draw() const
{
    float u1, u2, v1 = 0, v2 = 24;

    if (m_timerStun > 0)
    {
        int32_t idxFrame = int32_t(m_absTime * 8) % 3;
        u1 = 336 + 24 * idxFrame;
        u2 = 336 + 24 + 24 * idxFrame;
    }
    else if (m_bRunning)
    {
        int32_t idxFrame = int32_t(m_absTime * 16) % 6;
        u1 = 432 + 24 * idxFrame;
        u2 = 432 + 24 + 24 * idxFrame;
    }
    else if (m_dir.x != 0 || m_dir.y != 0)
    {
        int32_t idxFrame = int32_t(m_absTime * 8) % 6;
        u1 = 96 + 24 * idxFrame;
        u2 = 96 + 24 + 24 * idxFrame;
    }
    else
    {
        int32_t idxFrame = int32_t(m_absTime * 8) % 4;
        u1 = 0 + 24 * idxFrame;
        u2 = 24 + 24 * idxFrame;
    }

    std::vector<jv::gpu::Vertex> vsTrail;
    dino::GenVertices_Polyline(vsTrail, m_points, 10, m_color);
    jv::gpu::VertexBuffer* pVBufTrail = jv::gpu::CreateVertexBuffer("Trail", vsTrail);
    jv::gpu::Draw(pVBufTrail, NULL);
    jv::gpu::DestroyVertexBuffer(pVBufTrail);

    if (m_dir.x < 0)
        std::swap(u1, u2);

    v1 += 24 * m_idxPlayer;
    v2 += 24 * m_idxPlayer;

    std::vector<jv::gpu::Vertex> vs;
    vs.emplace_back(Vec2{m_pos.x - 16, m_pos.y - 32}, Vec2{u1, v1});
    vs.emplace_back(Vec2{m_pos.x + 16, m_pos.y - 32}, Vec2{u2, v1});
    vs.emplace_back(Vec2{m_pos.x - 16, m_pos.y}, Vec2{u1, v2});
    vs.emplace_back(Vec2{m_pos.x + 16, m_pos.y - 32}, Vec2{u2, v1});
    vs.emplace_back(Vec2{m_pos.x - 16, m_pos.y}, Vec2{u1, v2});
    vs.emplace_back(Vec2{m_pos.x + 16, m_pos.y}, Vec2{u2, v2});

    jv::gpu::VertexBuffer* pVBuf = jv::gpu::CreateVertexBuffer("Player", vs);
    jv::gpu::Draw(pVBuf, m_pTexture);
    jv::gpu::DestroyVertexBuffer(pVBuf);
}
