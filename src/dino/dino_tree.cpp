#include <dino/dino_tree.h>
#include <dino/dino_scene.h>
#include <dino/dino_player.h>
#include <dino/dino_assets.h>

dino::Tree::Tree(double absTime, int32_t season)
{
    m_season = season;
    m_spawnTime = absTime;
}

void dino::Tree::Update(Scene& scene, double absTime, float deltaTime)
{
    if (absTime - m_spawnTime > g_spawnDuration)
        m_alpha = 255;
}

void dino::Tree::Draw() const
{
    float u1 = 48 + (48 + 32) * m_season, u2 = 80 + (48 + 32) * m_season, v1 = 16, v2 = 64;

    jv::util::Color color = Color_WHITE;
    color.a = m_alpha;
    std::vector<jv::gpu::Vertex> vs;
    vs.emplace_back(Vec2{m_pos.x - 24, m_pos.y - 64}, Vec2{u1, v1}, color);
    vs.emplace_back(Vec2{m_pos.x + 24, m_pos.y - 64}, Vec2{u2, v1}, color);
    vs.emplace_back(Vec2{m_pos.x - 24, m_pos.y}, Vec2{u1, v2}, color);
    vs.emplace_back(Vec2{m_pos.x + 24, m_pos.y - 64}, Vec2{u2, v1}, color);
    vs.emplace_back(Vec2{m_pos.x - 24, m_pos.y}, Vec2{u1, v2}, color);
    vs.emplace_back(Vec2{m_pos.x + 24, m_pos.y}, Vec2{u2, v2}, color);

    jv::gpu::VertexBuffer* pVBuffer = jv::gpu::CreateVertexBuffer("Terrain", vs);
    jv::gpu::Draw(pVBuffer, (&dino::AssetsHolder::getInstance())->g_Textures["terrain"]);
    jv::gpu::DestroyVertexBuffer(pVBuffer);
}

void dino::Tree::OnLassoHit(Player& player_origin, Scene& scene)
{
    if (m_alpha == 255)
        scene.StartGame(m_season);
}

void dino::Tree::_HandleTerrainCollision(Terrain const& terrain) 
{
    m_pos = terrain.ClampPos(m_pos);
}