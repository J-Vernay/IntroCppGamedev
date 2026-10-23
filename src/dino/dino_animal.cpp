
#include <dino/dino_animal.h>

#include <algorithm>
#include <dino/dino_draw_utils.h>
#include <dino/dino_terrain.h>
#include <math.h>

dino::Animal::Animal(Vec2 pos, double absTime, jv::gpu::Texture* pTexture) : Entity{pos, 8.f}
{
    m_kind = jv::util::RandomInt32(0, 7);
    m_dir = jv::util::RandomRotate({1, 0}, 0, 360);
    m_timeStart = absTime;
    m_pTexture = pTexture;
}


void dino::Animal::Update(double absTime, float deltaTime)
{
    float speed = 30;
    m_pos.x += m_dir.x * deltaTime * speed;
    m_pos.y += m_dir.y * deltaTime * speed;

    m_idxFrame = int32_t(absTime * 8) % 4;

    const double aliveTime = absTime - m_timeStart;
    const double opacity = std::clamp(aliveTime, 0.0, 1.0);

    m_alpha = static_cast<uint8_t>(255.0 * opacity);
}

void dino::Animal::OnCaptured()
{
    MarkForRemoval();
}

void dino::Animal::OnTerrainCollision(Vec2 correction)
{
    m_dir = jv::util::RandomRotate({1, 0}, 0, 360);

    if (correction.x > 0.f)
        m_dir.x = std::abs(m_dir.x);
    else if (correction.x < 0.f)
        m_dir.x = -std::abs(m_dir.x);

    if (correction.y > 0.f)
        m_dir.y = std::abs(m_dir.y);
    else if (correction.y < 0.f)
        m_dir.y = -std::abs(m_dir.y);
}

void dino::Animal::Draw() const
{
    float u1 = 32 * m_idxFrame + 128 * m_kind;
    float u2 = u1 + 32;

    float v1 = 0;

    if (std::abs(m_dir.y) > std::abs(m_dir.x))
    {
        if (m_dir.y > 0)
            v1 = 32;
        else
            v1 = 64;
    }
    else
    {
        if (m_dir.x > 0)
            std::swap(u1, u2);
    }

    float v2 = v1 + 32;

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


