
#include <dino/dino_player.h>
#include <dino/dino_draw_utils.h>
#include <dino/dino_geometry.h>
#include <math.h>
#include <algorithm>


dino::Player::Player(Vec2 pos, double absTime, jv::input::GamepadIdx gamepadIdx) : Entity(pos, absTime)
{

    m_gamepadIdx = gamepadIdx;

    id = static_cast<int>(gamepadIdx);
    
    m_playerColorIndex = id;

    m_playerColor = m_playersColors[id];

    m_dir = Vec2{0, 0}; 

    m_timeStart = absTime;

    m_pTexture = dino::LoadImageAsset("dinosaurs.bmp");

}

dino::Player::~Player()
{
    jv::gpu::DestroyTexture(m_pTexture);
}

void dino::Player::Update(double absTime, float deltaTime)
{
    UpdateInput();
    UpdatePosition(deltaTime);
    UpdatePlayerState(deltaTime);
    UpdateIndexFrame(absTime);
    UpdateLasso();
    double aliveTime = absTime - m_timeStart;
    if (aliveTime < 1)
        m_alpha = uint8_t(UINT8_MAX * aliveTime);
}

void dino::Player::UpdateInput()
{
    jv::input::Gamepad gamepad;

    if (jv::input::GetGamepad(m_gamepadIdx, gamepad))
    {

        UpdateDirection(
            gamepad.dpad_right - gamepad.dpad_left,
            gamepad.dpad_down - gamepad.dpad_up);

        UpdateSpeed(gamepad.btn_right == 1);

        if (gamepad.btn_left == 1) HandleHit();
    }
}

void dino::Player::UpdateDirection(float x, float y)
{
    m_dir = Vec2{x, y};
}

void dino::Player::UpdatePosition(float deltaTime)
{
    if (m_currentPlayerState == Hurt) return;

    m_pos.x += m_dir.x * deltaTime * m_speedData.CurrentSpeed;
    m_pos.y += m_dir.y * deltaTime * m_speedData.CurrentSpeed;
}

void dino::Player::UpdatePlayerState(float deltaTime)
{
    if (m_currentPlayerState == Hurt)
    {
        UpdateHurtState(deltaTime);
        return;
    }

    if (isMoving())
    {
        m_currentPlayerState = m_speedData.IsRunning() ? PlayerState::Running : PlayerState::Walking;
    }
    else
    {
        m_currentPlayerState = PlayerState::Idle;
    }
}

void dino::Player::UpdateIndexFrame(double absTime)
{
    if (m_currentPlayerState == Idle)
    {
        m_idxFrame = int32_t(absTime * 8) % 4;
    }
    else if (m_currentPlayerState == Walking)
    {
        m_idxFrame = int32_t(absTime * 8) % 10;
    }
    else if (m_currentPlayerState == Running)
    {
        m_idxFrame = int32_t(absTime * 16) % 7;
    }
    else
    {
        m_idxFrame = int32_t(absTime * 8) % 3;
    }
}

void dino::Player::UpdateSpeed(bool isRunning)
{
    m_speedData.CurrentSpeed = isRunning ? m_speedData.SpeedRunning : m_speedData.SpeedWalking;
}

void dino::Player::UpdateLasso()
{
    if (m_lassoVector.size() > 2)
    {
        for (int i = 0; i < m_lassoVector.size() - 2; i++)
        {
            if (i == m_lassoVector.size() - 1)
                continue;

            bool isIntersecting = IntersectSegment(
                m_pos, m_lassoVector.back(), m_lassoVector[i], m_lassoVector[i + 1]);

            if (isIntersecting)
            {
                m_lassoVector.erase(m_lassoVector.begin() + i, m_lassoVector.end());
                return;
            }
        }
    }

   
    if (m_lassoVector.size() > 120)
    {
        m_lassoVector.erase(m_lassoVector.begin());
    }
    else
    {
        m_lassoVector.push_back(m_pos);
    }
}

void dino::Player::ResolveTerrainPos(Terrain& terrain)
{
    m_pos = terrain.ClampPos(m_pos);
}

void dino::Player::HandleHit()
{
    if (m_currentPlayerState == Hurt) return;

    m_currentPlayerState = Hurt;
    m_timerHit = 3;
}

void dino::Player::UpdateHurtState(float deltaTime)
{
    if (m_timerHit <= 0)
    {
        m_currentPlayerState = Idle;
    }
    else
    {
        m_timerHit -= deltaTime;
    }
}

void dino::Player::Draw() const
{
    DrawLasso();
    DrawPlayer();
}

void dino::Player::DrawLasso() const
{
    std::vector<jv::gpu::Vertex> vs;
    GenVertices_Polyline(vs, m_lassoVector, 4, m_playerColor);
    jv::gpu::VertexBuffer* pVBuf = jv::gpu::CreateVertexBuffer("Lasso", vs);
    jv::gpu::Draw(pVBuf, nullptr);
    jv::gpu::DestroyVertexBuffer(pVBuf);
}

void dino::Player::DrawPlayer() const
{
    float u1 = 0;
    float u2 = 24;
    float v1 = 0 + m_playerColorIndex * 24;
    float v2 = 24 + m_playerColorIndex * 24;

    float decal = 0;

    if (m_currentPlayerState == Hurt)
    {
        decal = 14;
    }

    if (isMoving() && m_currentPlayerState != Hurt)
    {
        if (m_currentPlayerState == PlayerState::Walking)
        {
            decal = 4;
        }
        else if (m_currentPlayerState == PlayerState::Running)
        {
            decal = 17;
        }
    }

    if (m_dir.x < 0)
    {
        float tempU = u2;
        u2 = u1;
        u1 = tempU;
    }

    u1 += 24 * m_idxFrame + decal * 24;
    u2 += 24 * m_idxFrame + decal * 24;

    jv::util::Color color = Color_WHITE;

    color.a = 255;

    std::vector<jv::gpu::Vertex> vs;
    vs.emplace_back(Vec2{m_pos.x - 16, m_pos.y - 32}, Vec2{u1, v1}, color);
    vs.emplace_back(Vec2{m_pos.x + 16, m_pos.y - 32}, Vec2{u2, v1}, color);
    vs.emplace_back(Vec2{m_pos.x - 16, m_pos.y}, Vec2{u1, v2}, color);
    vs.emplace_back(Vec2{m_pos.x + 16, m_pos.y - 32}, Vec2{u2, v1}, color);
    vs.emplace_back(Vec2{m_pos.x - 16, m_pos.y}, Vec2{u1, v2}, color);
    vs.emplace_back(Vec2{m_pos.x + 16, m_pos.y}, Vec2{u2, v2}, color);

    jv::gpu::VertexBuffer* pVBuf = jv::gpu::CreateVertexBuffer("Player", vs);
    jv::gpu::Draw(pVBuf, m_pTexture);
    jv::gpu::DestroyVertexBuffer(pVBuf);
}

