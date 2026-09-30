
#include <dino/dino_player.h>
#include <dino/dino_draw_utils.h>
#include <math.h>

dino::Player::Player(Vec2 pos, double absTime, jv::gpu::Texture* texture,
    jv::input::GamepadIdx gamepadIdx, int32_t color, Terrain* terrain)
    : Entity(terrain, pos)
{
    m_terrain = terrain;
    m_state = IDLE;
    m_color = color;
    m_gamepadIdx = gamepadIdx;
    m_dir = jv::util::RandomRotate({1, 0}, 0, 360);
    m_timeStart = absTime;
    m_pTexture = texture;
}

dino::Player::~Player()
{
    // jv::gpu::DestroyTexture(m_pTexture);
}

void dino::Player::Update(double absTime, float deltaTime)
{
    float speed = 50;
    bool running = false;

    // Entrées joueur.
    m_dir = {0, 0};
    jv::input::Gamepad input;
    if (jv::input::GetGamepad(m_gamepadIdx, input))
    {
        if (input.btn_left)
        {
            m_state = HURT;
            m_hurtTime = absTime;
        }

        // Direction de mouvement.
        if (input.dpad_up)
            m_dir.y += -1;
        if (input.dpad_down)
            m_dir.y += 1;
        if (input.dpad_left)
            m_dir.x += -1;
        if (input.dpad_right)
            m_dir.x += 1;
        
        // Logique de normalisation simplifiée.
        if (abs(m_dir.x) > 0 && abs(m_dir.y) > 0)
        {
            m_dir.x *= 0.7;
            m_dir.y *= 0.7;
        }

        // Courir.
        if (input.btn_right)
        {
            speed *= 2;
            running = true;
        }
    }

    if (m_state != HURT)
    {
        m_pos.x += m_dir.x * deltaTime * speed;
        m_pos.y += m_dir.y * deltaTime * speed;
        m_pos = m_terrain->ClampPos(m_pos);

        // Mettre à jour la direction du sprite, droite ou gauche.
        if (m_dir.x > 0)
        {
            m_facingLeft = false;
        }
        else if (m_dir.x < 0)
        {
            m_facingLeft = true;
        }

        // L'action que le joueur réalise en ce moment (cela dictera son animation).
        m_state = (m_dir.x == 0 && m_dir.y == 0) ? IDLE : (running ? RUN : WALK);
    }
    else
    {
        // L'immobilisation des dégâts dure 3 secondes.
        if (absTime - m_hurtTime > 3.)
        {
            m_state = IDLE;
        }
    }

    int frameRate = 0;
    int frameCount = 0;

    switch (m_state)
    {
    case IDLE:
        frameRate = 8;
        frameCount = 4;
        break;
    case WALK:
        frameRate = 8;
        frameCount = 6;
        break;
    case RUN:
        frameRate = 16;
        frameCount = 6;
        break;
    case HURT:
        frameRate = 8;
        frameCount = 3;
        break;
    default:
        break;
    }

    m_idxFrame = int32_t(absTime * frameRate) % frameCount;
}

void dino::Player::Draw() const
{
    // Le joueur se déplace visuellement vers la droite par défaut.
    float u1 = 0, u2 = 24;
    float v1 = m_color * 24;
    float v2 = m_color * 24 + 24;

    int32_t uActionOffset = 0;
    
    switch (m_state)
    {
    case IDLE:
        uActionOffset = 0;
        break;
    case WALK:
        uActionOffset = 96;
        break;
    case RUN:
        uActionOffset = 432;
        break;
    case HURT:
        uActionOffset = 336;
        break;
    default:
        break;
    }

    u1 += 24 * m_idxFrame + uActionOffset;
    u2 += 24 * m_idxFrame + uActionOffset;

    jv::util::Color color = Color_WHITE;
    color.a = m_alpha;

    if (m_facingLeft) // Le joueur regarde vers la gauche. Inversion de U.
    {
        float temp = u1;
        u1 = u2;
        u2 = temp;
    }

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

void dino::Player::OnOutsideTerrain()
{

}