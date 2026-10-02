
#include <dino/dino_player.h>
#include <dino/dino_draw_utils.h>
#include <math.h>

dino::Player::Player(Vec2 pos, double absTime, jv::gpu::Texture* pTexture)
{
    m_pos = pos;
    //m_dir = jv::util::RandomRotate({1, 0}, 0, 360);
    m_timeStart = absTime;
    m_pTexture = pTexture;
}

dino::Player::~Player()
{
}

void dino::Player::Update(double absTime, float deltaTime)
{
    float speed = 100;

    // Calcul de la direction du joueur, en utilisant les flèches du clavier.
    jv::input::Gamepad keyboard;
    if (jv::input::GetGamepad(jv::input::GamepadIdx::Keyboard, keyboard))
    {
        float dirx = 0, diry = 0;
        if (keyboard.dpad_up)
            diry -= 1;
        if (keyboard.dpad_down)
            diry += 1;
        if (keyboard.dpad_left)
            dirx -= 1;
        if (keyboard.dpad_right)
            dirx += 1;
        m_dir = {dirx, diry};

        m_bRunning = keyboard.btn_right; // 'D' sur le clavier (ZQSD)
        if (m_bRunning)
            speed *= 2;

        if (keyboard.btn_left) // 'A/Q' sur le clavier
            m_hitTime = 3;
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


    double aliveTime = absTime - m_timeStart;
    if (aliveTime < 1)
        m_alpha = uint8_t(UINT8_MAX * aliveTime);
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
    else if (m_bRunning)
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
#endif

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
    jv::gpu::Draw(pVBuf, m_pTexture);
    jv::gpu::DestroyVertexBuffer(pVBuf);
}
