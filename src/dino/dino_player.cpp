#include <dino/dino_player.h>

#include "dino_draw_utils.h"
#include "dino_geometry.h"

#include <dino/dino_terrain.h>
#include <dino/dino_entity.h>
#include <cmath>
#include <utility>
#include <vector>
#include <algorithm>

dino::Player::Player(Vec2 pos, jv::gpu::Texture* pTexture, jv::input::GamepadIdx gamepadIdx, int32_t colorIndex): Entity{pos, 8.f}
{
    m_pTexture = pTexture;
    m_gamepadIdx = gamepadIdx;
    m_colorIndex = colorIndex;
    m_lassoPoints.push_back({m_pos, m_lassoTime});
}

void dino::Player::Update(double absTime, float deltaTime)
{

    if (!m_isActive)
        return;

    m_hurtTimeRemaining = std::max(0.f, m_hurtTimeRemaining - deltaTime);

    m_dir = {0, 0};
    m_isMoving = false;
    m_isRunning = false;

    jv::input::Gamepad gamepad{};

    bool hasInput = jv::input::GetGamepad(m_gamepadIdx, gamepad);

    bool damageButtonDown = hasInput && gamepad.btn_left;

    bool damageButtonPressed = damageButtonDown && !m_damageButtonWasDown;

    m_damageButtonWasDown = damageButtonDown;

    if (damageButtonPressed)
        TakeDamage();

    if (IsHurt())
    {
        float hurtElapsedTime = 3.f - m_hurtTimeRemaining;

        m_idxFrame = int32_t(hurtElapsedTime * 8) % 3;
        return;
    }

    if (hasInput)
        {
            if (gamepad.dpad_left)
                m_dir.x -= 1;

            if (gamepad.dpad_right)
                m_dir.x += 1;

            if (gamepad.dpad_up)
                m_dir.y -= 1;

            if (gamepad.dpad_down)
                m_dir.y += 1;

            m_isRunning = gamepad.btn_right;
        }

    m_isMoving = m_dir.x != 0 || m_dir.y != 0;
    m_isRunning = m_isRunning && m_isMoving;

    if (m_isMoving)
    {
        float length = std::sqrt(m_dir.x * m_dir.x + m_dir.y * m_dir.y);

        m_dir.x /= length;
        m_dir.y /= length;
    }

    if (m_dir.x < 0)
        m_facingLeft = true;
    else if (m_dir.x > 0)
        m_facingLeft = false;

    float speed = 60.f;

    if (m_isRunning)
        speed *= 2.f;

    m_pos.x += m_dir.x * speed * deltaTime;
    m_pos.y += m_dir.y * speed * deltaTime;

    if (m_isRunning)
        m_idxFrame = int32_t(absTime * 16) % 6;
    else if (m_isMoving)
        m_idxFrame = int32_t(absTime * 8) % 6;
    else
        m_idxFrame = int32_t(absTime * 8) % 4;
}

void dino::Player::Draw() const
{
    if (!m_isActive)
        return;

    float animationStartU = 0;

    if (IsHurt())
        animationStartU = 336;
    else if (m_isRunning)
        animationStartU = 432;
    else if (m_isMoving)
        animationStartU = 96;

    float u1 = animationStartU + 24 * m_idxFrame;
    float u2 = u1 + 24;

    float v1 = 24.f * m_colorIndex;
    float v2 = v1 + 24.f;

    if (m_facingLeft)
        std::swap(u1, u2);

    std::vector<jv::gpu::Vertex> vertices;

    vertices.emplace_back(Vec2{m_pos.x - 12, m_pos.y - 24}, Vec2{u1, v1}, Color_WHITE);

    vertices.emplace_back(Vec2{m_pos.x + 12, m_pos.y - 24}, Vec2{u2, v1}, Color_WHITE);

    vertices.emplace_back(Vec2{m_pos.x - 12, m_pos.y}, Vec2{u1, v2}, Color_WHITE);

    vertices.emplace_back(Vec2{m_pos.x + 12, m_pos.y - 24}, Vec2{u2, v1}, Color_WHITE);

    vertices.emplace_back(Vec2{m_pos.x - 12, m_pos.y}, Vec2{u1, v2}, Color_WHITE);

    vertices.emplace_back(Vec2{m_pos.x + 12, m_pos.y}, Vec2{u2, v2}, Color_WHITE);

    jv::gpu::VertexBuffer* pVBuf = jv::gpu::CreateVertexBuffer("Player", vertices);

    jv::gpu::Draw(pVBuf, m_pTexture);
    jv::gpu::DestroyVertexBuffer(pVBuf);
}

bool dino::Player::IsHurt() const
{
    return m_hurtTimeRemaining > 0.f;
}

void dino::Player::TakeDamage()
{
    if (IsHurt())
        return;

    m_hurtTimeRemaining = 3.f;

    m_dir = {0, 0};
    m_isMoving = false;
    m_isRunning = false;

    m_idxFrame = 0;
}

void dino::Player::UpdateLasso(float deltaTime)
{
    m_closedLoop.clear();

    if (!m_isActive || deltaTime <= 0.f)
        return;


    m_lassoTime += deltaTime;

    m_lassoPoints.push_back({m_pos, m_lassoTime});

    constexpr double lassoDuration = 2.0;
    double oldestAllowedTime = m_lassoTime - lassoDuration;

    auto firstToRemove = std::remove_if(m_lassoPoints.begin(), m_lassoPoints.end(),[oldestAllowedTime](const LassoPoint& point) { return point.time < oldestAllowedTime; });

    m_lassoPoints.erase(firstToRemove, m_lassoPoints.end());

    _RemoveLassoLoop();
}

void dino::Player::DrawLasso() const
{
    if (!m_isActive || m_lassoPoints.size() < 2)
        return;

    std::vector<Vec2> positions;
    positions.reserve(m_lassoPoints.size());

    for (const LassoPoint& point : m_lassoPoints)
        positions.push_back(point.pos);

    const Color playerColors[4] = {Color_BLUE, Color_RED, Color_YELLOW, Color_GREEN};

    std::vector<jv::gpu::Vertex> vertices;

    dino::GenVertices_Polyline(vertices, positions, 2.f, playerColors[m_colorIndex]);

    if (vertices.empty())
        return;

    jv::gpu::VertexBuffer* pVBuf = jv::gpu::CreateVertexBuffer("Lasso", vertices);

    jv::gpu::Draw(pVBuf, nullptr);
    jv::gpu::DestroyVertexBuffer(pVBuf);
}

void dino::Player::Join(jv::input::GamepadIdx device)
{
    m_gamepadIdx = device;
    m_isActive = true;

    ResetState(true);
}

void dino::Player::Leave()
{
    m_isActive = false;

    ResetState(false);
}

bool dino::Player::CutLasso(Vec2 from, Vec2 to)
{
    if (!m_isActive || m_lassoPoints.size() < 2)
        return false;

    for (std::size_t end = m_lassoPoints.size() - 1; end > 0; --end)
    {
        const std::size_t i = end - 1;

        Vec2 intersection;
        float t = 0.f;
        float u = 0.f;

        if (!FindSegmentIntersection(
                from, to, m_lassoPoints[i].pos, m_lassoPoints[end].pos, intersection, t, u))
        {
            continue;
        }

        if (t <= 0.000001f) continue;

        const double intersectionTime =m_lassoPoints[i].time + u * (m_lassoPoints[end].time - m_lassoPoints[i].time);

        m_lassoPoints.erase(m_lassoPoints.begin(), m_lassoPoints.begin() + static_cast<std::ptrdiff_t>(i));

        m_lassoPoints.front() = {intersection, intersectionTime};

        return true;
    }

    return false;
}

void dino::Player::OnCaptured()
{
    TakeDamage();
}


void dino::Player::ResetState(bool resetScore)
{
    m_dir = {0.f, 0.f};
    m_isMoving = false;
    m_isRunning = false;

    m_hurtTimeRemaining = 0.f;
    m_idxFrame = 0;

    m_lassoTime = 0.0;

    m_closedLoop.clear();
    m_lassoPoints.clear();
    m_lassoPoints.push_back({m_pos, m_lassoTime});

    if (resetScore)
    {
        m_score = 0;
    }
}


void dino::Player::_RemoveLassoLoop()
{
    if (m_lassoPoints.size() < 4)
        return;

    std::size_t last = m_lassoPoints.size() - 1;

    LassoPoint endPoint = m_lassoPoints[last];
    Vec2 start = m_lassoPoints[last - 1].pos;

    for (std::size_t i = 0; i + 2 < last; ++i)
    {
        Vec2 intersection;
        float t = 0.f;
        float u = 0.f;

        if (!FindSegmentIntersection(start, endPoint.pos, m_lassoPoints[i].pos,m_lassoPoints[i + 1].pos, intersection, t, u))
        {
            continue;
        }

        if (t <= 0.000001f)
            continue;

        std::vector<Vec2> loop;
        loop.push_back(intersection);

        for (std::size_t j = i + 1; j < last; ++j)
            loop.push_back(m_lassoPoints[j].pos);

        loop.push_back(intersection);
        double twiceArea = 0.0;

        for (std::size_t j = 0; j + 1 < loop.size(); ++j)
        {
            twiceArea += double(loop[j].x) * loop[j + 1].y - double(loop[j + 1].x) * loop[j].y;
        }

        if (std::abs(twiceArea) < 0.0001)
            continue;

        double intersectionTime =m_lassoPoints[i].time + u * (m_lassoPoints[i + 1].time - m_lassoPoints[i].time);

        m_closedLoop = std::move(loop);

        m_lassoPoints.resize(i + 1);
        m_lassoPoints.push_back({intersection, intersectionTime});
        m_lassoPoints.push_back(endPoint);

        return;
    }


}