#include <dino/dino_tree.h>

dino::Tree::Tree(Vec2 pos, int32_t kind, jv::gpu::Texture* texture) : Entity(pos)
{
    m_kind = kind;
    m_pTexture = texture;
}

dino::Tree::~Tree()
{
}

void dino::Tree::Update(double absTime, float deltaTime, Terrain& terrain)
{
}

void dino::Tree::Draw() const
{
    // L'animal se déplace visuellement vers la gauche par défaut.
    float u1 = 48 + 80 * m_kind, u2 = 80 + 80 * m_kind, v1 = 16, v2 = 64;

    jv::util::Color color = Color_WHITE;

    std::vector<jv::gpu::Vertex> vs;
    vs.emplace_back(Vec2{m_pos.x - 16, m_pos.y - 48}, Vec2{u1, v1}, color);
    vs.emplace_back(Vec2{m_pos.x + 16, m_pos.y - 48}, Vec2{u2, v1}, color);
    vs.emplace_back(Vec2{m_pos.x - 16, m_pos.y}, Vec2{u1, v2}, color);
    vs.emplace_back(Vec2{m_pos.x + 16, m_pos.y - 48}, Vec2{u2, v1}, color);
    vs.emplace_back(Vec2{m_pos.x - 16, m_pos.y}, Vec2{u1, v2}, color);
    vs.emplace_back(Vec2{m_pos.x + 16, m_pos.y}, Vec2{u2, v2}, color);

    jv::gpu::VertexBuffer* pVBuf = jv::gpu::CreateVertexBuffer("Animal", vs);
    jv::gpu::Draw(pVBuf, m_pTexture);
    jv::gpu::DestroyVertexBuffer(pVBuf);
}

void dino::Tree::OnCaughtInLoop()
{
    m_bShouldDie = true;
}

bool dino::Tree::ShouldStartGame()
{
    return m_bShouldDie;
}

void dino::Tree::OnOutsideTerrain()
{
    
}