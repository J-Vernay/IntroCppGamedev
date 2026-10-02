
#include <dino/dino_player.h>
#include <dino/dino_terrain.h>
#include <dino/dino_scene.h>
#include <dino/dino_assets.h>
#include <dino/dino_draw_utils.h>
#include <math.h>
#include <algorithm>

dino::Player::Player(jv::input::GamepadIdx gamepadIdx, Vec2 pos, int32_t kind)
{
    m_pos = pos;
    m_kind = kind;
    m_gamepadIdx = gamepadIdx;
}

dino::Player::~Player()
{
}

void dino::Player::_BuildVelocity()
{
    jv::input::Gamepad gamepad;

    // Reset speed and direction
    m_speed = 0;
    m_dir.x = 0, m_dir.y = 0;

    if (jv::input::GetGamepad(m_gamepadIdx, gamepad) && m_hitDuration < 0) // Can't move if hit
    {
        if (gamepad.dpad_up)
        {
            m_dir.y = -1;
            m_lastDir = m_dir;
            m_speed = g_basePlayerSpeed;
        }
        if (gamepad.dpad_down)
        {
            m_dir.y = 1;
            m_lastDir = m_dir;
            m_speed = g_basePlayerSpeed;
        }
        if (gamepad.dpad_left)
        {
            m_dir.x = -1;
            m_lastDir = m_dir;
            m_speed = g_basePlayerSpeed;
        }
        if (gamepad.dpad_right)
        {
            m_dir.x = 1;
            m_lastDir = m_dir;
            m_speed = g_basePlayerSpeed;
        }
        if (gamepad.btn_right)
        {
            m_speed *= 2;
        }
    }
}

void dino::Player::_HandleTerrainCollision(Terrain const& terrain)
{
    m_pos = terrain.ClampPos(m_pos);
}

void dino::Player::_HandleLasso(double absTime)
{
    // Add a point to the lasso
    if (absTime - m_lastLassoPointTime > g_lassoPointsDeltaTime)
    {
        m_lassoPoints.push_back(m_pos);
        m_lassoPointsSpawnTime.push_back(absTime);
        m_lastLassoPointTime = absTime;
    }
    // Remove points older than 2 seconds
    for (int i = m_lassoPoints.size() - 1; i >= 0; i--)
    {
        if (absTime - m_lassoPointsSpawnTime[i] > g_lassoPointsLifeTime)
        {
            m_lassoPoints.erase(m_lassoPoints.begin() + i);
            m_lassoPointsSpawnTime.erase(m_lassoPointsSpawnTime.begin() + i);
        }
    }
}

void dino::Player::OnLassoHit(Player& player_origin, Scene& scene)
{
    m_hitDuration = g_basePlayerStunDuration;
}

void dino::Player::Update(dino::Scene const& scene, double absTime, float deltaTime)
{
    // Update stun duration
    if (m_hitDuration > 0)
    {
        m_hitDuration -= deltaTime;
    }

    _BuildVelocity();
    _Move(scene, deltaTime);
    _HandleLasso(absTime);
 
    // Update animation frame time
    this->m_idxFrame = int32_t(absTime * 8);
}

void dino::Player::Draw() const
{
    // Animation "Idle" by default
    int animationFrameLength = 4;
    float u1 = 0, u2 = 24, v1 = 24 * m_kind, v2 = 24 + 24 * m_kind;

    if (m_hitDuration > 0) // Animation hit
    {
        u1 += 336, u2 += 336;
        animationFrameLength = 3;
    }
    else if (m_speed > g_basePlayerSpeed) // Animation run
    {
        u1 += 432, u2 += 432;
        animationFrameLength = 6;
    }
    else if (m_speed > 0) // Animation walk
    {
        u1 += 96, u2 += 96;
        animationFrameLength = 6;
    }

    u1 += 24 * (m_idxFrame % animationFrameLength);
    u2 += 24 * (m_idxFrame % animationFrameLength);
    
    if (m_lastDir.x < 0) // Mirror sprite vertically if the player is going left
    {
        std::swap(u1, u2);
    }

    jv::util::Color color = Color_WHITE;
    color.a = m_alpha;
    // Player's Sprite
    std::vector<jv::gpu::Vertex> vs;
    vs.emplace_back(Vec2{m_pos.x - 16, m_pos.y - 32}, Vec2{u1, v1}, color);
    vs.emplace_back(Vec2{m_pos.x + 16, m_pos.y - 32}, Vec2{u2, v1}, color);
    vs.emplace_back(Vec2{m_pos.x - 16, m_pos.y}, Vec2{u1, v2}, color);
    vs.emplace_back(Vec2{m_pos.x + 16, m_pos.y - 32}, Vec2{u2, v1}, color);
    vs.emplace_back(Vec2{m_pos.x - 16, m_pos.y}, Vec2{u1, v2}, color);
    vs.emplace_back(Vec2{m_pos.x + 16, m_pos.y}, Vec2{u2, v2}, color);

    jv::gpu::VertexBuffer* pVBuf = jv::gpu::CreateVertexBuffer("Player", vs);
    jv::gpu::Draw(pVBuf, (&dino::AssetsHolder::getInstance())->g_Textures["players"]);
    jv::gpu::DestroyVertexBuffer(pVBuf);

    // Player's lasso
    std::vector<jv::gpu::Vertex> lassoVerticies;

    jv::util::Color const playersColor[4] = {
        Color_BLUE,
        Color_RED,
        Color_YELLOW,
        Color_GREEN
    };
    dino::GenVertices_Polyline(lassoVerticies, m_lassoPoints, g_lassoWidth, playersColor[m_kind]);

    pVBuf = jv::gpu::CreateVertexBuffer("PlayerLasso", lassoVerticies);
    jv::gpu::Draw(pVBuf, (&dino::AssetsHolder::getInstance())->g_Textures["white"]);
    jv::gpu::DestroyVertexBuffer(pVBuf);
}
