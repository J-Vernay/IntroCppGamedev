#include <dino/dino_draw_utils.h>
#include <dino/dino_players.h>
#include <math.h>

dino::Player::Player(Vec2 pos, double absTime, jv::gpu::Texture* pTexture)
{
    m_pos = pos;
    m_kind = jv::util::RandomInt32(0, 7);
    m_timeStart = absTime;
    m_pTexture = pTexture;
    m_gamepadIdx = jv::input::GamepadIdx::Keyboard;
}

dino::Player::~Player()
{
    jv::gpu::DestroyTexture(m_pTexture);
}

void dino::Player::Update(double absTime, float deltaTime)
{
    jv::input::Gamepad input;
    float speed = 100;
    if (jv::input::GetGamepad(m_gamepadIdx, input)){
        Vec2 dir = Vec2(0,0);
        if (input.dpad_up)
            dir.y -= 1;
        if (input.dpad_down)
            dir.y += 1;
        if (input.dpad_right)
            dir.x += 1;
        if (input.dpad_left)
            dir.x -= 1;
        m_dir = {dir.x, dir.y};
        if (input.btn_right)
        {
            speed *= 2;
        }
        
    }
    if (m_dir.x != 0)
    {

        m_bLeft = m_dir.x < 0;
    }
    m_bRunning = input.btn_right;
    m_pos.x += m_dir.x * deltaTime * speed;
    m_pos.y += m_dir.y * deltaTime * speed;

    m_absTime = absTime;
    

    double aliveTime = absTime - m_timeStart;
    if (aliveTime < 1)
        m_alpha = uint8_t(UINT8_MAX * aliveTime);
}

void dino::Player::Draw() const
{
    
    float u1 = 0, u2 = 24, v1 = 0, v2 = 24;
    int32_t m_idxFrame;
    if (m_bRunning)
    {
        m_idxFrame = int32_t(m_absTime * 16) % 6;
        u1 = 432 + 24 * m_idxFrame, u2 = 432 + 24 + 24 * m_idxFrame;
    }
    else if (m_dir.x != 0 || m_dir.y != 0)
    {
        m_idxFrame = int32_t(m_absTime * 8) % 6;
        u1 = 96 + 24 * m_idxFrame, u2 = 96 + 24 + 24 * m_idxFrame;
    }
    else
    {
        m_idxFrame = int32_t(m_absTime * 8) % 4;
        u1 = 0 + 24 * m_idxFrame, u2 = 24 + 24 * m_idxFrame;
    }
    if ( m_bLeft)
    {
        std::swap(u1, u2);
    }

    jv::util::Color color = Color_WHITE;
    color.a = m_alpha;
    
    std::vector<jv::gpu::Vertex> vs;
    vs.emplace_back(Vec2{m_pos.x - 24, m_pos.y - 48}, Vec2{u1, v1}, color);
    vs.emplace_back(Vec2{m_pos.x + 24, m_pos.y - 48}, Vec2{u2, v1}, color);
    vs.emplace_back(Vec2{m_pos.x - 24, m_pos.y}, Vec2{u1, v2}, color);
    vs.emplace_back(Vec2{m_pos.x + 24, m_pos.y - 48}, Vec2{u2, v1}, color);
    vs.emplace_back(Vec2{m_pos.x - 24, m_pos.y}, Vec2{u1, v2}, color);
    vs.emplace_back(Vec2{m_pos.x + 24, m_pos.y}, Vec2{u2, v2}, color);

    jv::gpu::VertexBuffer* pVBuf = jv::gpu::CreateVertexBuffer("Animal", vs);
    jv::gpu::Draw(pVBuf, m_pTexture);
    jv::gpu::DestroyVertexBuffer(pVBuf);
}
