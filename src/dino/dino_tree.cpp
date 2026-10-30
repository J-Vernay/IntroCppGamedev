#include "dino_tree.h"

dino::Tree::Tree(Vec2 pos, int32_t seasonIdx, jv::gpu::Texture* pTexture) 
    : GameObject(pos), SeasonIdx(seasonIdx)
{
    m_pTexture = pTexture;

    SetSortingOrder(2);
}

void dino::Tree::Update(double absTime, float deltaTime)
{
    if (m_disableSelectionTimer > 0)
    {
        m_disableSelectionTimer -= deltaTime;
    }
}

void dino::Tree::Draw() const
{
    float u1 = 48 + 80 * SeasonIdx, u2 = 80 + 80 * SeasonIdx, v1 = 16, v2 = 64;

    jv::util::Color color = Color_WHITE;
    if (m_disableSelectionTimer > 0)
    {
        color.a = 128; // 50% d'opacité
    }

    jv::util::Vec2 pos = GetPos();

    std::vector<jv::gpu::Vertex> vs;
    vs.emplace_back(Vec2{pos.x - 16, pos.y - 48}, Vec2{u1, v1}, color);
    vs.emplace_back(Vec2{pos.x + 16, pos.y - 48}, Vec2{u2, v1}, color);
    vs.emplace_back(Vec2{pos.x - 16, pos.y}, Vec2{u1, v2}, color);
    vs.emplace_back(Vec2{pos.x + 16, pos.y - 48}, Vec2{u2, v1}, color);
    vs.emplace_back(Vec2{pos.x - 16, pos.y}, Vec2{u1, v2}, color);
    vs.emplace_back(Vec2{pos.x + 16, pos.y}, Vec2{u2, v2}, color);

    jv::gpu::VertexBuffer* pVBuf = jv::gpu::CreateVertexBuffer("Tree", vs);
    jv::gpu::Draw(pVBuf, m_pTexture);
    jv::gpu::DestroyVertexBuffer(pVBuf);
}
