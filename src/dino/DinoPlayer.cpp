
#include <dino/DinoPlayer.h>
#include <dino/dino_draw_utils.h>
#include <math.h>

dino::DinoPlayer::DinoPlayer(Vec2 pos, double absTime, jv::gpu::Texture* text)
{
    m_pos = pos;
    m_kind = jv::util::RandomInt32(0, 7);
    m_dir = jv::util::RandomRotate({1, 0}, 0, 360);
    m_timeStart = absTime;
    m_pTexture = text;
}

dino::DinoPlayer::~DinoPlayer()
{
    
}

void dino::DinoPlayer::Update(double absTime, float deltaTime)
{

        float speed = 100;

    jv::input::Gamepad keyboard;
    if (jv::input::GetGamepad(jv::input::GamepadIdx::Keyboard, keyboard))
    {
        float dirx = 0;
        float diry = 0;

        if (keyboard.dpad_up)
            diry -= 1;

        if (keyboard.dpad_down)
            diry += 1;

        if (keyboard.dpad_left)
            dirx -= 1;

        if (keyboard.dpad_right)
            dirx += 1;

        m_dir = {dirx, diry};

        if (keyboard.btn_right)
            speed = speed * 2;

    }



    m_pos.x += m_dir.x * deltaTime * speed;
    m_pos.y += m_dir.y * deltaTime * speed;

    m_idxFrame = int32_t(absTime * 8) % 4;

    double aliveTime = absTime - m_timeStart;
    if (aliveTime < 1)
        m_alpha = uint8_t(UINT8_MAX * aliveTime);
}
void dino::DinoPlayer::Draw() const
{
    float u1 = 0;
    float u2 = 24;
    float v1 = 0;
    float v2 = 24;




    if (m_dir.x > 0.0f)
    {
        u1 = 24;
        u2 = 48;
    }
    else if (m_dir.x < 0.0f)
    {
        u1 = 48;
        u2 = 24;
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
