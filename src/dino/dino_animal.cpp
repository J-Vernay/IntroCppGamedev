
#include <dino/dino_animal.h>
#include <dino/dino_draw_utils.h>
#include <math.h>

dino::Animal::Animal(Vec2 pos, double absTime)
{
    m_pos = pos;
    m_kind = jv::util::RandomInt32(0, 7);
    m_dir = jv::util::RandomRotate({1, 0}, 0, 360);
    m_timeStart = absTime;
    m_pTexture = dino::LoadImageAsset("animals.bmp");
}

void dino::Animal::Shut()
{
    jv::gpu::DestroyTexture(m_pTexture);
    m_pTexture = 0;
}

void dino::Animal::Update(double absTime, float deltaTime)
{
    float speed = 30;
    m_pos.x += m_dir.x * deltaTime * speed;
    m_pos.y += m_dir.y * deltaTime * speed;

    m_idxFrame = int32_t(absTime * 8) % 4;

    double aliveTime = absTime - m_timeStart;
    if (aliveTime < 1)
        m_alpha = uint8_t(UINT8_MAX * aliveTime);
}

void dino::Animal::Draw() const
{
    float u1 = 0, u2 = 32, v1 = 0, v2 = 32;

    if (m_dir.x >= 0)
    {
        // L'animal va vers la droite, et le sprite est orienté vers la gauche -> On inverse la
        // coordonnée U du sprite.
        u1 = 32;
        u2 = 0;
    }

    if (fabsf(m_dir.x) > fabsf(m_dir.y))
    {
        // L'animal se déplace surtout horizontalement.
        v1 = 0;
        v2 = 32;
    }
    else if (m_dir.y >= 0)
    {
        // L'animal va vers le bas.
        v1 = 32;
        v2 = 64;
    }
    else
    {
        // L'animal va vers le haut.
        v1 = 64;
        v2 = 96;
    }

    u1 += 32 * m_idxFrame + 128 * m_kind;
    u2 += 32 * m_idxFrame + 128 * m_kind;

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
