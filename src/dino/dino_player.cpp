
#include <dino/dino_draw_utils.h>
#include <dino/dino_player.h>
#include <dino/dino_geometry.h>
#include <math.h>

dino::Player::Player(Vec2 pos, double absTime, jv::input::GamepadIdx  gamepadIdx, int color) : dino::Entity(pos, absTime)
{
    m_pos = pos;
    m_lastPos = pos;
    m_idxFrame = 0;
    m_kind = 1;
    m_dir = jv::util::RandomRotate({1, 0}, 0, 360);
    m_timeStart = absTime;
    m_pTexture = dino::LoadImageAsset("dinosaurs.bmp");
    m_state = idle;
    m_stunDuration = 1;
    m_gamepad = gamepadIdx;
    m_lastDamageTime = -50;
    m_playerIndex = (int)gamepadIdx;
    m_color = playerColors[m_playerIndex];
}

dino::Player::~Player()
{
    // jv::gpu::DestroyTexture(m_pTexture);
}

void dino::Player::Update(double absTime, float deltaTime)
{
    UpdateInputs(absTime, deltaTime);
    UpdateFrameRate(absTime);

    m_lassoPoints.push_back(m_pos);
    if (m_lassoPoints.size() > 120)
    {
        m_lassoPoints.erase(m_lassoPoints.begin());
    }

    DetectLassoColision();

    double aliveTime = absTime - m_timeStart;
    if (aliveTime < 1)
        m_alpha = uint8_t(UINT8_MAX * aliveTime);
}

void dino::Player::HandleTerrainClamp(dino::Terrain* terrain)
{
    m_pos = terrain->ClampPos(m_pos);
}

void dino::Player::UpdateFrameRate(double absTime)
{
    if (m_state == run)
    {
        m_idxFrame = int32_t(absTime * 16) % 6;
    }
    else if (m_state == walk)
    {
        m_idxFrame = int32_t(absTime * 8) % 6;
    }
    else if (m_state == idle)
    {
        m_idxFrame = int32_t(absTime * 8) % 4;
    }
    else if (m_state == damage)
    {
        m_idxFrame = int32_t(absTime * 8) % 3;
    }
}

void dino::Player::UpdateInputs(double absTime, float deltaTime)
{
    float speed = 30;

    jv::input::Gamepad gamepad;

    if (absTime - m_lastDamageTime > m_stunDuration)
    {
        m_state = idle;
    }

    if (jv::input::GetGamepad(m_gamepad, gamepad))
    {
        if (gamepad.btn_left)
        {
            TakeDamage(absTime);
            return;
        }
        if (m_state == damage)
        {
            return;
        }

        m_dir = Vec2(gamepad.dpad_right - gamepad.dpad_left, gamepad.dpad_down - gamepad.dpad_up);
        if (gamepad.btn_right)
        {
            speed *= 2;
            m_state = run;
        }
        else
        {
            m_state = walk;
        }
    }
    if (m_dir.x == 0 && m_dir.y == 0)
    {
        m_state = idle;
    }
    m_lastPos = m_pos;
    m_pos.x += m_dir.x * deltaTime * speed;
    m_pos.y += m_dir.y * deltaTime * speed;
}

void dino::Player::DetectLassoColision()
{
    if (m_lastPos.x == m_pos.x && m_lastPos.y == m_pos.y)
        return;
    for (int i = 0; i < m_lassoPoints.size() - 1; i++)
    {
        if (dino::IntersectSegment(m_lastPos, m_pos, m_lassoPoints.at(i), m_lassoPoints.at(i + 1))) {
            HandleLassoColision(i);
            return;
        }
    }
}

void dino::Player::HandleLassoColision(int n)
{
    m_lassoPoints.erase(m_lassoPoints.end() - n, m_lassoPoints.end());
}



void dino::Player::TakeDamage(double absTime)
{
    m_state = damage;
    m_lastDamageTime = absTime;
    m_idxFrame = int32_t(absTime * 8) % 3;
}

void dino::Player::Draw() const
{
    DrawLasso();

    float u1 = 0, u2 = 24, v1 = 0 + 24 * m_playerIndex, v2 = 24 + 24 * m_playerIndex;

    float decal = 0;

    if (m_state == damage)
    {
        decal = 14;
    }
    else if (m_state == run)
    {
        decal = 17;
    }
    else if (m_state == walk)
    {
        decal = 4;
    }
    if (m_dir.x < 0)
    {
        int temp = u1;
        u1 = u2;
        u2 = temp;
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

void dino::Player::DrawLasso() const
{
    std::vector<jv::gpu::Vertex> vs;

    GenVertices_Polyline(vs, m_lassoPoints, 4, m_color);

    jv::gpu::VertexBuffer* pVBuf = jv::gpu::CreateVertexBuffer("lasso", vs);
    jv::gpu::Draw(pVBuf, nullptr);
    jv::gpu::DestroyVertexBuffer(pVBuf);
}