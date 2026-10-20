#include "dino_tree.h"

dino::dino_tree::dino_tree(Vec2 pPos, jv::gpu::Texture* pTexture, int pIndex)
{
    m_pos = pPos;
    m_pTexture = pTexture;
    index = pIndex;
}

void dino::dino_tree::Draw() const
{
    float u1 = 48, u2 = 80, v1 = 16, v2 = 64;

    u1 += (48+32) * index;
    u2 += (48 + 32) * index;

    jv::util::Color color = Color_WHITE;

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
