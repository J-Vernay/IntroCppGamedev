
#include <dino/dino_draw_utils.h>
#include <dino/dino_terrain.h>
#include <dino/dino_player.h>

#include <random>

dino::Terrain::Terrain(int32_t tileCountX, int32_t tileCountY)
{
    m_pTexture = dino::LoadImageAsset("terrain.bmp");

    Vec2 rdrSize = jv::gpu::GetRenderSize();

    // Océan qui représente tout l'écran.
    m_vertices.emplace_back(Vec2{0, 0}, Vec2{0, 0});
    m_vertices.emplace_back(Vec2{rdrSize.x, 0}, Vec2{16, 0});
    m_vertices.emplace_back(Vec2{0, rdrSize.y}, Vec2{0, 16});
    m_vertices.emplace_back(Vec2{rdrSize.x, 0}, Vec2{16, 0});
    m_vertices.emplace_back(Vec2{0, rdrSize.y}, Vec2{0, 16});
    m_vertices.emplace_back(rdrSize, Vec2{16, 16});

    m_idxVertexOceanEnd = m_vertices.size();

    // Milieu

    m_vertices.emplace_back(Vec2{16, 16}, Vec2{16, 32});
    m_vertices.emplace_back(Vec2{16.f * (tileCountX - 1), 16}, Vec2{32, 32});
    m_vertices.emplace_back(Vec2{16, 16.f * (tileCountY - 1)}, Vec2{16, 48});
    m_vertices.emplace_back(Vec2{16.f * (tileCountX - 1), 16}, Vec2{32, 32});
    m_vertices.emplace_back(Vec2{16, 16.f * (tileCountY - 1)}, Vec2{16, 48});
    m_vertices.emplace_back(Vec2{16.f * (tileCountX - 1), 16.f * (tileCountY - 1)}, Vec2{32, 48});

    m_idxVertexNonAnimatedEnd = m_vertices.size();

    // Coins

    dino::GenVertices_Rect(m_vertices, {0, 0}, {16, 16}, {0, 16});
    dino::GenVertices_Rect(m_vertices, {16.f * (tileCountX - 1), 0}, {16, 16}, {32, 16});
    dino::GenVertices_Rect(m_vertices, {0, 16.f * (tileCountY - 1)}, {16, 16}, {0, 48});
    dino::GenVertices_Rect(
        m_vertices, {16.f * (tileCountX - 1), 16.f * (tileCountY - 1)}, {16, 16}, {32, 48});

    // Frontières

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

    // On centre le terrain de jeu au milieu de l'écran.

    Vec2 terrainSize = {tileCountX * 16.f, tileCountY * 16.f};
    Vec2 terrainPos = {0.5f * (rdrSize.x - terrainSize.x), 0.5f * (rdrSize.y - terrainSize.y)};

    // Tous les vertices, sauf l'océan, sont décalés pour être centrés.
    for (size_t i = m_idxVertexOceanEnd; i < m_vertices.size(); ++i)
    {
        m_vertices[i].pos.x += terrainPos.x;
        m_vertices[i].pos.y += terrainPos.y;

    }

    // On retient les coordonnées du terrain pour la logique de jeu.
    m_spawnOffset = {terrainPos.x + 16.f, terrainPos.y + 16.f};
    m_spawnSize = {terrainSize.x - 32.f, terrainSize.y - 32.f};
}

void dino::Terrain::SetSeason(int32_t idxSeason)
{
    if (idxSeason >= 4)
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

void dino::Terrain::Draw() const
{
    constexpr uint16_t OFFSETS_V[6] = {0, 48, 96, 144, 96, 48};
    uint16_t offsetV = OFFSETS_V[m_idxFrame];

    // On copie les vertices

    std::vector<jv::gpu::Vertex> vertices = m_vertices;

    // La saison s'applique à tous les vertices.
    for (jv::gpu::Vertex& v : vertices)
        v.uv.x += 80 * m_idxSeason;

    // L'animation ne s'applique pas à l'océan ni au milieu.   
    for (size_t i = m_idxVertexNonAnimatedEnd; i < vertices.size(); ++i)
        vertices[i].uv.y += 48 * m_idxFrame;

    jv::gpu::VertexBuffer* pVBuffer = jv::gpu::CreateVertexBuffer("Terrain", vertices);
    jv::gpu::Draw(pVBuffer, m_pTexture);
    jv::gpu::DestroyVertexBuffer(pVBuffer);
}

jv::util::Vec2 dino::Terrain::GenerateRandomSpawn() const
{
    Vec2 pos;
    pos.x = m_spawnOffset.x + jv::util::RandomFloat(0, m_spawnSize.x);
    pos.y = m_spawnOffset.y + jv::util::RandomFloat(0, m_spawnSize.y);
    return pos;
}

jv::util::Vec2 dino::Terrain::ClampPos(Vec2 pos) const
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