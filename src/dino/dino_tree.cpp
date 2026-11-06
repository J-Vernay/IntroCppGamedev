#include <dino/dino_tree.h>

dino::Tree::Tree(Vec2 pos, double absTime, int32_t kind, jv::gpu::Texture* texture) : Entity(pos)
{
    m_timeStart = absTime;
    m_kind = kind;
    m_pTexture = texture;
}

dino::Tree::~Tree()
{
}

void dino::Tree::Update(double absTime, float deltaTime, Terrain& terrain)
{
    double aliveTime = absTime - m_timeStart;
    if (aliveTime < InactiveTime)
        m_alpha = uint8_t(UINT8_MAX * (aliveTime / InactiveTime));
}

void dino::Tree::Draw() const
{
    // L'animal se déplace visuellement vers la gauche par défaut.
    float u1 = 48 + 80 * m_kind, u2 = 80 + 80 * m_kind, v1 = 16, v2 = 64;

    jv::util::Color color = Color_WHITE;
    color.a = m_alpha;

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

int32_t dino::Tree::OnCaughtInLoop(float absTime)
{
    double aliveTime = absTime - m_timeStart;
    if (aliveTime < InactiveTime)
        return 0;
    m_bShouldDie = true;

    return 0;
}

void dino::Tree::ResetTime(float absTime)
{
    m_timeStart = absTime;
}

bool dino::Tree::ShouldStartGame()
{
    bool startGame = m_bShouldDie;
    m_bShouldDie = false;
    return startGame;
}

void dino::Tree::OnOutsideTerrain()
{
    
}