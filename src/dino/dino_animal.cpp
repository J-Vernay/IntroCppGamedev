
#include <dino/dino_animal.h>
#include <dino/dino_draw_utils.h>
#include <math.h>

dino::Animal::Animal(Vec2 pos, double absTime, jv::gpu::Texture* texture, Terrain* terrain)
    : Entity(terrain, pos)
{
    m_kind = jv::util::RandomInt32(0, 7);
    m_dir = jv::util::RandomRotate({1, 0}, 0, 360);
    m_timeStart = absTime;
    m_pTexture = texture;
    m_terrain = terrain;
}

dino::Animal::~Animal()
{
}

void dino::Animal::Update(double absTime, float deltaTime)
{
    float speed = 30;
    Vec2 displacement = Vec2{m_dir.x * deltaTime * speed, m_dir.y * deltaTime * speed};
    Move(displacement);

    m_idxFrame = int32_t(absTime * 8) % 4;

    double aliveTime = absTime - m_timeStart;
    if (aliveTime < 1)
        m_alpha = uint8_t(UINT8_MAX * aliveTime);
}

void dino::Animal::Draw() const
{
    // L'animal se déplace visuellement vers la gauche par défaut.
    float u1 = 0, u2 = 32, v1 = 0, v2 = 32;

    u1 += 32 * m_idxFrame + 128 * m_kind;
    u2 += 32 * m_idxFrame + 128 * m_kind;

    jv::util::Color color = Color_WHITE;
    color.a = m_alpha;

    if (abs(m_dir.y) > abs(m_dir.x)) // L'animal se déplace principalement verticalement.
    {
        if (m_dir.y > 0) // L'animal se déplace vers le haut.
        {
            v1 = 32;
            v2 = 64;
        }
        else // L'animal se déplace vers le bas.
        {
            v1 = 64;
            v2 = 96;
        }
    }
    else if (m_dir.x > 0) // L'animal se déplace vers la droite.
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

void dino::Animal::OnOutsideTerrain()
{
    m_dir = jv::util::RandomRotate({1, 0}, 0, 360);
}