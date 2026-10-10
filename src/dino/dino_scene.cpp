
#include <dino/dino_draw_utils.h>
#include <dino/dino_scene.h>
#include <algorithm>
#include <format>
#include <dino/dino_geometry.h>

dino::Scene::Scene() : m_Terrain{24, 16}
{
    m_pTextureText = dino::LoadImageAsset("monogram-bitmap.bmp");
    m_pTexture = dino::LoadImageAsset("animals.bmp");
    m_pTextureDinosaurs = dino::LoadImageAsset("dinosaurs.bmp");

    m_Terrain.SetSeason(jv::util::RandomInt32(0, 3));
    Vec2 spawnPos = m_Terrain.GenerateRandomSpawn();

    m_players.emplace_back(m_Terrain.GenerateRandomSpawn(), 0, m_pTextureDinosaurs, 0,
        jv::input::GamepadIdx::Keyboard, Color_BLUE);
    m_players.emplace_back(m_Terrain.GenerateRandomSpawn(), 0, m_pTextureDinosaurs, 1,
        jv::input::GamepadIdx::Gamepad1, Color_RED);
    m_players.emplace_back(m_Terrain.GenerateRandomSpawn(), 0, m_pTextureDinosaurs, 2,
        jv::input::GamepadIdx::Gamepad2, Color_YELLOW);
    m_players.emplace_back(m_Terrain.GenerateRandomSpawn(), 0, m_pTextureDinosaurs, 3,
        jv::input::GamepadIdx::Gamepad3, Color_GREEN);
}

dino::Scene::~Scene()
{
    jv::gpu::DestroyTexture(m_pTexture);
    jv::gpu::DestroyTexture(m_pTextureDinosaurs);
}

void dino::Scene::Update(double absTime, float deltaTime)
{
    m_lastDeltaTime = deltaTime;

    m_Terrain.Update(absTime, deltaTime);

    _UpdateAnimals(absTime, deltaTime);

    _UpdatePlayer(absTime, deltaTime);

    for (Animal& animal : m_animals)
    {
        m_movable.push_back(&animal);
    }

    
    std::sort(m_movable.begin(), m_movable.end(), Movable::OrderByPosY);
}

void dino::Scene::_UpdatePlayer(double absTime, float deltaTime)
{

    for (Player& player : m_players)
    {
        player.Update(absTime, deltaTime);
        player.CheckTerrain(m_Terrain);
    }

    for (int i = 0; i < m_players.size(); ++i)
        for (int j = i + 1; j < m_players.size(); ++j)
            Movable::ResolveCollision(m_players[i], m_players[j]);

    for (int i = 0; i < m_players.size(); ++i)
        for (int j = 0; j < m_animals.size(); ++j)
            Movable::ResolveCollision(m_players[i], m_animals[j]);

    for (size_t p = 0; p < m_players.size(); p++)
    {
        Player& player = m_players[p];
        m_movable.push_back(&player);

        if (player.m_points.size() < 2)
            continue;

        Vec2 last = player.m_points.back();
        Vec2 lastToLast = player.m_points[player.m_points.size() - 2];

        for (size_t i = 0; i < m_players.size(); i++)
        {
            if (i == p)
                continue;

            auto& pts = m_players[i].m_points;

            for (size_t y = 0; y + 1 < pts.size(); y++)
            {
                if (dino::IntersectSegment(lastToLast, last, pts[y], pts[y + 1]))
                {
                    pts.erase(pts.begin(), pts.begin() + y + 1);
                    break;
                }
            }
        }
    }
}

void dino::Scene::_UpdateAnimals(double absTime, float deltaTime)
{
    // Spawner un animal si besoin.

    constexpr double kSpawnTime = 0.3;
    if (absTime - m_animalSpawnTime >= kSpawnTime)
    {
        m_animalSpawnTime = absTime;
        Vec2 spawnPos = m_Terrain.GenerateRandomSpawn();
        m_animals.emplace_back(spawnPos, absTime, m_pTexture);
    }

    for (Animal& animal : m_animals)
    {
        animal.Update(absTime, deltaTime);
        animal.CheckTerrain(m_Terrain);
    }

    for (int i = 0; i < m_animals.size(); ++i)
        for (int j = i + 1; j < m_animals.size(); ++j)
            Movable::ResolveCollision(m_animals[i], m_animals[j]);
}

void dino::Scene::Draw() const
{
    m_Terrain.Draw();

    for (Movable const* entity : m_movable)
        entity->Draw();

    // Nombre de millisecondes qu'il a fallu pour afficher la frame précédente.
    {
        std::string text = std::format("dTime={:04.1f}ms", m_lastDeltaTime * 1000.0);
        std::vector<jv::gpu::Vertex> vs;
        dino::GenVertices_Text(vs, text, Color_WHITE, Color_GREY);
        jv::gpu::VertexBuffer* pVBuf = jv::gpu::CreateVertexBuffer("dTime", vs);
        jv::gpu::Draw(pVBuf, m_pTextureText);
        jv::gpu::DestroyVertexBuffer(pVBuf);
    }
}
