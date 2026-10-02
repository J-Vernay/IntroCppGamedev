#include <dino/dino_draw_utils.h>
#include <dino/dino_player.h>
#include <dino/dino_terrain.h>
#include <math.h>

dino::Player::Player(
    Vec2 pos, int32_t idxPlayer, jv::input::GamepadIdx gamepadIdx, jv::gpu::Texture* pTexture)
{
    m_pos = pos;
    m_idxPlayer = idxPlayer;
    m_gamepadIdx = gamepadIdx;
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

        //sprint

        m_bRunning = padInput.btn_right;

        if (m_bRunning)
            speed *= 2;
        else
            speed /= 2;

        if (padInput.btn_left) // 'A/Q' sur le clavier
            m_hitTime = 3;

        m_dir = {dirX, dirY};
    }

    if (m_hitTime <= 0) // Animation de dégâts fini / pas active
    {
        m_pos.x += m_dir.x * deltaTime * speed;
        m_pos.y += m_dir.y * deltaTime * speed;
    }
    else
    {
        m_hitTime -= deltaTime; // On avance le temps de l'anim
    }

    m_absTime = absTime;

    if (m_dir.x != 0)
        m_bLeft = m_dir.x < 0;
}

void dino::Player::Draw() const
{
    float u1, u2, v1 = 0, v2 = 24;

    if (m_hitTime > 0)
    {
        // Degat
        int32_t idxFrame = int32_t(m_absTime * 8) % 3;
        u1 = 336 + 24 * idxFrame;
        u2 = 336 + 24 + 24 * idxFrame;
    }

    if (m_bRunning)
    {
        // Course
        int32_t idxFrame = int32_t(m_absTime * 16) % 6;
        u1 = 432 + 24 * idxFrame;
        u2 = 432 + 24 + 24 * idxFrame;
    }
    else if (m_dir.x != 0 || m_dir.y != 0)
    {
        // Marche
        int32_t idxFrame = int32_t(m_absTime * 8) % 6;
        u1 = 96 + 24 * idxFrame;
        u2 = 96 + 24 + 24 * idxFrame;
    }
    else
    {
        // Immobile
        int32_t idxFrame = int32_t(m_absTime * 8) % 4;
        u1 = 0 + 24 * idxFrame;
        u2 = 24 + 24 * idxFrame;
    }

    if (m_bLeft)
        std::swap(u1, u2);

    #if 0

    v1 = 0, v2 = 24;

    u1 += 32 * m_idxFrame + 128 * m_kind;
    u2 += 32 * m_idxFrame + 128 * m_kind;

#endif

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

void dino::Player::DetectBounds(Terrain const& terrain)
{
    m_pos = terrain.ClampPos(m_pos);
}