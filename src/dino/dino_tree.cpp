#include "dino_tree.h"

dino::dino_tree::dino_tree(Vec2 pPos, jv::gpu::Texture* pTexture, int pIndex, double pAbsTime)
{
    m_pos = pPos;
    m_pTexture = pTexture;
    index = pIndex;
    m_timeStart = pAbsTime;
}

void dino::dino_tree::Update(double absTime, float deltaTime)
{
    m_aliveTime = absTime - m_timeStart;
    if (m_aliveTime < 5)
        m_alpha = uint8_t(UINT8_MAX * m_aliveTime/5);
}

bool dino::dino_tree::IsActive()
{
    return m_aliveTime > 5;
}

void dino::dino_tree::Draw() const
{
    float u1 = 48, u2 = 80, v1 = 16, v2 = 64;

    u1 += (48+32) * index;
    u2 += (48 + 32) * index;

    jv::util::Color color = jv::util::Color({Color_WHITE.r, Color_WHITE.g, Color_WHITE.b, m_alpha});

    std::vector<jv::gpu::Vertex> vs;
    vs.emplace_back(Vec2{m_pos.x - 16, m_pos.y - 48}, Vec2{u1, v1}, color);
    vs.emplace_back(Vec2{m_pos.x + 16, m_pos.y - 48}, Vec2{u2, v1}, color);
    vs.emplace_back(Vec2{m_pos.x - 16, m_pos.y}, Vec2{u1, v2}, color);
    vs.emplace_back(Vec2{m_pos.x + 16, m_pos.y - 48}, Vec2{u2, v1}, color);
    vs.emplace_back(Vec2{m_pos.x - 16, m_pos.y}, Vec2{u1, v2}, color);
    vs.emplace_back(Vec2{m_pos.x + 16, m_pos.y}, Vec2{u2, v2}, color);

    jv::gpu::VertexBuffer* pVBuf = jv::gpu::CreateVertexBuffer("Tree", vs);
    jv::gpu::Draw(pVBuf, m_pTexture);
    jv::gpu::DestroyVertexBuffer(pVBuf);
}
