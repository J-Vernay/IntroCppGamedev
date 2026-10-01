
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

    // COORDONNEES HORIZONTALES
    if (m_dir.x > 0)
        std::swap(u1, u2);

    u1 += 32 * m_idxFrame + 128 * m_kind;
    u2 += 32 * m_idxFrame + 128 * m_kind;

    // COORDONNEES VERTICALES
    if (std::abs(m_dir.y) > std::abs(m_dir.x))
    {
        float pxOffset = (m_dir.y > 0) ? 32.0f : 64.0f;
        v1 += pxOffset;
        v2 += pxOffset;
    }

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
