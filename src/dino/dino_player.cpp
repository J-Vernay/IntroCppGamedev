
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

        // TEMPORARY
        if (gamepad.btn_left)
        {
            m_hitDuration = g_basePlayerStunDuration;
        }
    }
}

void dino::Player::_Move(dino::Scene& scene, float deltaTime)
{
    m_pos.x += m_dir.x * deltaTime * m_speed;
    m_pos.y += m_dir.y * deltaTime * m_speed;
    
    // Force player inside of terrain
    if (!scene.GetTerrain().IsInside(m_pos))
        _HandleTerrainCollision(scene.GetTerrain());
}

void dino::Player::_HandleTerrainCollision(Terrain& terrain)
{
    m_pos = terrain.ClampPos(m_pos);
}

void dino::Player::Update(dino::Scene& scene, double absTime, float deltaTime)
{
    // Update stun duration
    if (m_hitDuration > 0)
    {
        m_hitDuration -= deltaTime;
    }

    _BuildVelocity();
    _Move(scene, deltaTime);
 
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
}
