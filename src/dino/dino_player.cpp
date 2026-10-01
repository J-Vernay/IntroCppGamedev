#include <dino/dino_draw_utils.h>
#include <dino/dino_player.h>
#include <math.h>

dino::Player::Player(Vec2 pos, double absTime, jv::gpu::Texture* pTexture)
{
    m_pos = pos;
    m_kind = jv::util::RandomInt32(0, 7);
    m_dir = jv::util::RandomRotate({1, 0}, 0, 360);
    m_timeStart = absTime;
    m_pTexture = pTexture;
}

dino::Player::~Player()
{
}

void dino::Player::Update(double absTime, float deltaTime)
{
    float speed = 100;

    jv::input::Gamepad padInput;

    if (jv::input::GetGamepad(jv::input::GamepadIdx::Keyboard, padInput))
    {
        float dirX = 0, dirY = 0;
        if (padInput.dpad_left)
            dirX -= 1;
        if (padInput.dpad_right)
            dirX += 1;
        if (padInput.dpad_up)
            dirY -= 1;
        if (padInput.dpad_down)
            dirY += 1;
        m_dir = {dirX, dirY};
    }

    m_pos.x += m_dir.x * deltaTime * speed;
    m_pos.y += m_dir.y * deltaTime * speed;

    m_idxFrame = int32_t(absTime * 8) % 4;

    double aliveTime = absTime - m_timeStart;
    if (aliveTime < 1)
        m_alpha = uint8_t(UINT8_MAX * aliveTime);
}

void dino::Player::Draw() const
{
    float u1, u2, v1, v2;

    if (m_dir.x > 0)
    {
        u1 = 32, u2 = 0; // Inversion du sprite sur l'axe X
    }
    else
    {
        u1 = 0, u2 = 32; // Normal sur l'axe X
    }

    if (std::abs(m_dir.y) > std::abs(m_dir.x))
    {
        if (m_dir.y > 0)
        {
            v1 = 32, v2 = 64; // Bas
        }
        else
        {
            v1 = 64, v2 = 96; // Haut
        }
    }
    else
    {
        v1 = 0, v2 = 32; // Horizontal
    }

    u1 += 32 * m_idxFrame + 128 * m_kind;
    u2 += 32 * m_idxFrame + 128 * m_kind;

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
