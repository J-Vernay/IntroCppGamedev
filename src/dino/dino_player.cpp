#include "dino_player.h"

#include <dino/dino_animal.h>
#include <dino/dino_draw_utils.h>
#include <dino/dino_geometry.h>
#include <iostream>
#include <math.h>
#include <windows.h>
#include <string>

dino::Player::Player(Vec2 pos, double absTime, jv::gpu::Texture* texture, int pindxPlayer,
    jv::input::GamepadIdx pgamepadIdx, Color pColor, PlayerState pState)
{
    m_gamepadIdx = pgamepadIdx;
    m_idxPlayer = pindxPlayer;
    m_pos = pos;
    m_dir = jv::util::RandomRotate({1, 0}, 0, 360);
    m_timeStart = absTime;
    m_pTexture = texture;
    m_color = pColor;
    m_currentState = pState;
}

void dino::Player::Update(double absTime, float deltaTime)
{
    float speed = 100;
    m_absTime = absTime;

    jv::input::Gamepad keyboard;
    Vec2 dir = Vec2(0, 0);
    if (jv::input::GetGamepad(m_gamepadIdx, keyboard))
    {
        if (m_currentState == PlayerState::Lobby && keyboard.start && !m_isSpawned)
        {
            m_isSpawned = true;
        }
        else if ((m_currentState == PlayerState::Lobby && keyboard.select && m_isSpawned))
        {
            m_isSpawned = false;
            return;
        }
        else if (!m_isSpawned)
            return;

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

        _Trail(deltaTime);
    }
    else
    {
        m_timerStun -= deltaTime;
    }

    double aliveTime = absTime - m_timeStart;
    if (aliveTime < 1)
        m_alpha = uint8_t(UINT8_MAX * aliveTime);

    m_timer += deltaTime;

    while (m_points.size() > 500)
    {
        m_points.erase(m_points.begin());
    }
}

void dino::Player::TakeDamage()
{
    m_timerStun = 3;
}

bool dino::Player::IsPause()
{

    if (m_currentState == PlayerState::Lobby)
        return false;

    jv::input::Gamepad pad;
    bool startNow = false;

    if (jv::input::GetGamepad(m_gamepadIdx, pad))
        startNow = pad.start;

    bool pressed = startNow && !m_prevStart;
    bool released = !startNow && m_prevStart;

    m_prevStart = startNow;

    if (pressed && m_isSpawned)
    {
        m_IsPaused = !m_IsPaused;
        return true;
    }
    return false;
}

void dino::Player::ChangeState(PlayerState pState)
{
    m_currentState = pState;
}

void dino::Player::_Trail(float deltaTime)
{
    if (!m_points.empty() && m_points.back().x == m_pos.x && m_points.back().y == m_pos.y)
        return;
    m_points.push_back(m_pos);
}

void dino::Player::Draw() const
{
    if (!m_isSpawned)
        return;

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
    
    std::string timeStr = std::to_string(std::lround(m_score));
    std::string_view time = timeStr;

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

std::vector<dino::Vec2> dino::Player::CheckIfLoop()
{
    if (m_points.size() >= 4)
    {
        Vec2 lastPoint = m_points[m_points.size() - 1];
        Vec2 lastToLastPoint = m_points[m_points.size() - 2];

        for (size_t i = 0; i + 3 < m_points.size(); i++)
        {
            if (dino::IntersectSegment(m_points[i], m_points[i + 1], lastToLastPoint, lastPoint))
            {
                std::vector<Vec2> pointBoucle(m_points.begin() + i, m_points.end() - 1);
                m_points.erase(m_points.begin() + i, m_points.end() - 1);
                return pointBoucle;
            }
        }
    }

    return {};
}
