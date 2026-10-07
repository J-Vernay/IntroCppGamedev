#include "dino_tree.h"
#include "dino_scene.h"

dino::Tree::Tree(Vec2 pos, double absTime, int index, Scene* ptr) : Entity(pos, absTime)
{
    m_pTexture = dino::LoadImageAsset("terrain.bmp");
    this->index = index;
    scenePtr = ptr;
}

dino::Tree::~Tree()
{
    jv::gpu::DestroyTexture(m_pTexture);
}

void dino::Tree::Draw() const
{
    float u1 = 80 * index + 48;
    float u2 = 80 * index + 80;
    float v1 = 64;
    float v2 = 112;

    jv::util::Color color = Color_WHITE;

    std::vector<jv::gpu::Vertex> vs;
    vs.emplace_back(Vec2{m_pos.x - 16, m_pos.y - 32}, Vec2{u1, v1}, color);
    vs.emplace_back(Vec2{m_pos.x + 16, m_pos.y - 32}, Vec2{u2, v1}, color);
    vs.emplace_back(Vec2{m_pos.x - 16, m_pos.y}, Vec2{u1, v2}, color);
    vs.emplace_back(Vec2{m_pos.x + 16, m_pos.y - 32}, Vec2{u2, v1}, color);
    vs.emplace_back(Vec2{m_pos.x - 16, m_pos.y}, Vec2{u1, v2}, color);
    vs.emplace_back(Vec2{m_pos.x + 16, m_pos.y}, Vec2{u2, v2}, color);

    jv::gpu::VertexBuffer* pVBuf = jv::gpu::CreateVertexBuffer("Tree", vs);
    jv::gpu::Draw(pVBuf, m_pTexture);
    jv::gpu::DestroyVertexBuffer(pVBuf);
}

void dino::Tree::CatchByPlayer()
{
    scenePtr->StartGame(index);
}

void dino::Tree::ResolveTerrainPos(Terrain& terrain)
{
    m_pos = terrain.ClampPos(m_pos);
}

void dino::Tree::Update(double absTime, float deltaTime)
{

}
