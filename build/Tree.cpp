#include "Tree.h"


void dino::Tree::HandleCapture()
{
    isAlive = false;
}

dino::Tree::Tree(Vec2 pos, float absTime, int treeType, jv::gpu::Texture* tex)
    : dino::Entity(pos, absTime)
{
    m_pTexture = tex;
    m_pos = pos;
    m_TreeType = treeType;
}

void dino::Tree::Update(double absTime, float deltaTime)
{
}

void dino::Tree::HandleTerrainClamp(dino::Terrain* terrain)
{
}

dino::Tree::~Tree()
{
}

void dino::Tree::Draw() const
{

    float u1 = 48, u2 = 80, v1 = 16, v2 = 64;

    v1 += 48 * m_TreeType;
    v2 += 48 * m_TreeType;

    jv::util::Color color = Color_WHITE;
    color.a = 255;

    std::vector<jv::gpu::Vertex> vs;
    vs.emplace_back(Vec2{ m_pos.x - 16, m_pos.y - 32 }, Vec2{ u1, v1 }, color);
    vs.emplace_back(Vec2{ m_pos.x + 16, m_pos.y - 32 }, Vec2{ u2, v1 }, color);
    vs.emplace_back(Vec2{ m_pos.x - 16, m_pos.y }, Vec2{ u1, v2 }, color);
    vs.emplace_back(Vec2{ m_pos.x + 16, m_pos.y - 32 }, Vec2{ u2, v1 }, color);
    vs.emplace_back(Vec2{ m_pos.x - 16, m_pos.y }, Vec2{ u1, v2 }, color);
    vs.emplace_back(Vec2{ m_pos.x + 16, m_pos.y }, Vec2{ u2, v2 }, color);

    jv::gpu::VertexBuffer* pVBuf = jv::gpu::CreateVertexBuffer("Tree", vs);
    jv::gpu::Draw(pVBuf, m_pTexture);
    jv::gpu::DestroyVertexBuffer(pVBuf);
}
