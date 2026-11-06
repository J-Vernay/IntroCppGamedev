#include <dino/dino_draw_utils.h>
#include <dino/dino_terrain.h>

#include <random>
//construit le terrain et  limites de déplacement
dino::Terrain::Terrain(int32_t tileCountX, int32_t tileCountY)
{
    m_pTexture = dino::LoadImageAsset("terrain.bmp");

    Vec2 rdrSize = jv::gpu::GetRenderSize();

    m_vertices.emplace_back(Vec2{0, 0}, Vec2{0, 0});
    m_vertices.emplace_back(Vec2{rdrSize.x, 0}, Vec2{16, 0});
    m_vertices.emplace_back(Vec2{0, rdrSize.y}, Vec2{0, 16});
    m_vertices.emplace_back(Vec2{rdrSize.x, 0}, Vec2{16, 0});
    m_vertices.emplace_back(Vec2{0, rdrSize.y}, Vec2{0, 16});
    m_vertices.emplace_back(rdrSize, Vec2{16, 16});

    m_idxVertexOceanEnd = m_vertices.size();

    m_vertices.emplace_back(Vec2{16, 16}, Vec2{16, 32});
    m_vertices.emplace_back(Vec2{16.f * (tileCountX - 1), 16}, Vec2{32, 32});
    m_vertices.emplace_back(Vec2{16, 16.f * (tileCountY - 1)}, Vec2{16, 48});
    m_vertices.emplace_back(Vec2{16.f * (tileCountX - 1), 16}, Vec2{32, 32});
    m_vertices.emplace_back(Vec2{16, 16.f * (tileCountY - 1)}, Vec2{16, 48});
    m_vertices.emplace_back(Vec2{16.f * (tileCountX - 1), 16.f * (tileCountY - 1)}, Vec2{32, 48});

    m_idxVertexNonAnimatedEnd = m_vertices.size();

    dino::GenVertices_Rect(m_vertices, {0, 0}, {16, 16}, {0, 16});
    dino::GenVertices_Rect(m_vertices, {16.f * (tileCountX - 1), 0}, {16, 16}, {32, 16});
    dino::GenVertices_Rect(m_vertices, {0, 16.f * (tileCountY - 1)}, {16, 16}, {0, 48});
    dino::GenVertices_Rect(
        m_vertices, {16.f * (tileCountX - 1), 16.f * (tileCountY - 1)}, {16, 16}, {32, 48});

    for (int32_t i = 1; i < tileCountX - 1; ++i)
    {
        dino::GenVertices_Rect(m_vertices, {16.f * i, 0}, {16, 16}, {16, 16});
        dino::GenVertices_Rect(m_vertices, {16.f * i, 16.f * (tileCountY - 1)}, {16, 16}, {16, 48});
    }

    for (int32_t i = 1; i < tileCountY - 1; ++i)
    {
        dino::GenVertices_Rect(m_vertices, {0, 16.f * i}, {16, 16}, {0, 32});
        dino::GenVertices_Rect(m_vertices, {16.f * (tileCountX - 1), 16.f * i}, {16, 16}, {32, 32});
    }

    Vec2 terrainSize{tileCountX * 16.f, tileCountY * 16.f};
    Vec2 terrainPos{0.5f * (rdrSize.x - terrainSize.x), 0.5f * (rdrSize.y - terrainSize.y)};

    for (size_t i = m_idxVertexOceanEnd; i < m_vertices.size(); ++i)
    {
        m_vertices[i].pos.x += terrainPos.x;
        m_vertices[i].pos.y += terrainPos.y;
    }

    m_spawnOffset = {terrainPos.x + 16.f, terrainPos.y + 16.f};
    m_spawnSize = {terrainSize.x - 32.f, terrainSize.y - 32.f};
}
// Sélectionne la saison du terrain
void dino::Terrain::SetSeason(int32_t idxSeason)
{
    if (idxSeason < 0 || idxSeason >= 4)
        jv::util::Panic("Seulement 4 saisons disponibles");

    m_idxSeason = idxSeason;
}

dino::Terrain::~Terrain()
{
    jv::gpu::DestroyTexture(m_pTexture);
}

void dino::Terrain::Update(double absTime, float deltaTime)
{
    m_idxFrame = int32_t(absTime * 8) % 4;
}
// affiche le terrain avec sa saison et son animation
void dino::Terrain::Draw() const
{
    std::vector<jv::gpu::Vertex> vertices = m_vertices;

    for (jv::gpu::Vertex& vertex : vertices)
        vertex.uv.x += 80 * m_idxSeason;

    for (size_t i = m_idxVertexNonAnimatedEnd; i < vertices.size(); ++i)
        vertices[i].uv.y += 48 * m_idxFrame;

    jv::gpu::VertexBuffer* pVBuffer = jv::gpu::CreateVertexBuffer("Terrain", vertices);
    jv::gpu::Draw(pVBuffer, m_pTexture);
    jv::gpu::DestroyVertexBuffer(pVBuffer);
}

// Retourne le centre de l'arbre d'une saison
dino::Vec2 dino::Terrain::GetSeasonTreeCenter(int32_t idxSeason) const
{
    if (idxSeason < 0 || idxSeason >= 4)
        return {};

    Vec2 renderSize = jv::gpu::GetRenderSize();
    constexpr float TREE_SIZE = 48;
    constexpr float TREE_SPACING = 16;
    constexpr float GROUP_WIDTH = 4 * TREE_SIZE + 3 * TREE_SPACING;

    float startX = (renderSize.x - GROUP_WIDTH) * 0.5f;
    float treeY = renderSize.y * 0.36f;

    return {startX + idxSeason * (TREE_SIZE + TREE_SPACING) + TREE_SIZE * 0.5f, treeY + 54};
}

// Affiche les quatre arbres du lobby
void dino::Terrain::DrawSeasonTrees(bool dimmed) const
{
    Vec2 renderSize = jv::gpu::GetRenderSize();
    constexpr float TREE_SIZE = 48;
    constexpr float TREE_SPACING = 16;
    constexpr float GROUP_WIDTH = 4 * TREE_SIZE + 3 * TREE_SPACING;

    float startX = (renderSize.x - GROUP_WIDTH) * 0.5f;
    float treeY = renderSize.y * 0.36f;

    Color tint = Color_WHITE;
    if (dimmed)
        tint.a = 110;

    std::vector<jv::gpu::Vertex> vertices;

    for (int32_t season = 0; season < 4; ++season)
    {
        float x1 = startX + season * (TREE_SIZE + TREE_SPACING);
        float x2 = x1 + 48;
        float y1 = treeY;
        float y2 = y1 + 72;

        float u1 = 48.f + 80.f * season;
        float u2 = u1 + 32;
        float v1 = 16;
        float v2 = 64;

        vertices.emplace_back(Vec2{x1, y1}, Vec2{u1, v1}, tint);
        vertices.emplace_back(Vec2{x2, y1}, Vec2{u2, v1}, tint);
        vertices.emplace_back(Vec2{x1, y2}, Vec2{u1, v2}, tint);
        vertices.emplace_back(Vec2{x2, y1}, Vec2{u2, v1}, tint);
        vertices.emplace_back(Vec2{x1, y2}, Vec2{u1, v2}, tint);
        vertices.emplace_back(Vec2{x2, y2}, Vec2{u2, v2}, tint);
    }

    jv::gpu::VertexBuffer* pVBuffer = jv::gpu::CreateVertexBuffer("Season trees", vertices);
    jv::gpu::Draw(pVBuffer, m_pTexture);
    jv::gpu::DestroyVertexBuffer(pVBuffer);
}

dino::Vec2 dino::Terrain::GenerateRandomSpawn() const
{
    Vec2 pos;
    pos.x = m_spawnOffset.x + jv::util::RandomFloat(0, m_spawnSize.x);
    pos.y = m_spawnOffset.y + jv::util::RandomFloat(0, m_spawnSize.y);
    return pos;
}

// Limite une position aux frontières du terrain
dino::Vec2 dino::Terrain::ClampPos(Vec2 pos) const
{
    if (pos.x < m_spawnOffset.x)
        pos.x = m_spawnOffset.x;
    if (pos.x > m_spawnOffset.x + m_spawnSize.x)
        pos.x = m_spawnOffset.x + m_spawnSize.x;
    if (pos.y < m_spawnOffset.y)
        pos.y = m_spawnOffset.y;
    if (pos.y > m_spawnOffset.y + m_spawnSize.y)
        pos.y = m_spawnOffset.y + m_spawnSize.y;

    return pos;
}