
#include <algorithm>
#include <dino/dino_draw_utils.h>
#include <dino/dino_geometry.h>
#include <dino/dino_scene.h>
#include <format>
#include <iostream>
#include <string>

dino::Scene::Scene() : m_Terrain{24, 16}
{
    m_pTextureText = dino::LoadImageAsset("monogram-bitmap.bmp");
    m_pTexture = dino::LoadImageAsset("animals.bmp");
    m_pTextureTree = dino::LoadImageAsset("terrain.bmp");
    m_pTextureDinosaurs = dino::LoadImageAsset("dinosaurs.bmp");

    m_Terrain.SetSeason(jv::util::RandomInt32(0, 3));
    Vec2 spawnPos = m_Terrain.GenerateRandomSpawn();

    PlayerState state = PlayerState::Lobby;

    m_players.emplace_back(m_Terrain.GenerateRandomSpawn(), 0, m_pTextureDinosaurs, 0,
        jv::input::GamepadIdx::Keyboard, Color_BLUE, state);
    m_players.emplace_back(m_Terrain.GenerateRandomSpawn(), 0, m_pTextureDinosaurs, 1,
        jv::input::GamepadIdx::Gamepad1, Color_RED, state);
    m_players.emplace_back(m_Terrain.GenerateRandomSpawn(), 0, m_pTextureDinosaurs, 2,
        jv::input::GamepadIdx::Gamepad2, Color_YELLOW, state);
    m_players.emplace_back(m_Terrain.GenerateRandomSpawn(), 0, m_pTextureDinosaurs, 3,
        jv::input::GamepadIdx::Gamepad3, Color_GREEN, state);

    m_trees.emplace_back(dino_tree(Vec2(480/2, 360/2), m_pTextureTree, 0));
}

dino::Scene::~Scene()
{
    jv::gpu::DestroyTexture(m_pTexture);
    jv::gpu::DestroyTexture(m_pTextureDinosaurs);
}

void dino::Scene::Update(double absTime, float deltaTime)
{
    m_lastDeltaTime = deltaTime;
    m_movable.clear();

    _UpdatePlayer(absTime, deltaTime);

    for (Player& player : m_players)
    {
        if (player.IsPause())
        {
            m_isPaused = !m_isPaused;
        }
    }

    if (m_isPaused|| m_currentState == PlayerState::Lobby)
            return;

    for (Animal& animal : m_animals)
    {
        m_movable.push_back(&animal);
    }

    m_Terrain.Update(absTime, deltaTime);

    _UpdateAnimals(absTime, deltaTime);


    std::sort(m_movable.begin(), m_movable.end(), Movable::OrderByPosY);

    m_timer = std::clamp(m_timer -deltaTime, 0.0f, 60.0f);
}

void dino::Scene::_UpdatePlayer(double absTime, float deltaTime)
{

    for (size_t i = 0; i < m_players.size(); i++)
    {
        Player& player = m_players[i];
        
        player.Update(absTime, deltaTime);

        if (!player.m_isSpawned)
            continue;

        m_movable.push_back(&player);

        player.CheckTerrain(m_Terrain);
        CheckLoop(player);

        for (int j = i + 1; j < m_players.size(); ++j)
            Movable::ResolveCollision(player, m_players[j]);

        for (int j = 0; j < m_animals.size(); ++j)
            Movable::ResolveCollision(player, m_animals[j]);


        if (player.m_points.size() < 2)
            continue;

        Vec2 last = player.m_points.back();
        Vec2 lastToLast = player.m_points[player.m_points.size() - 2];

        for (size_t y = 0; y < m_players.size(); y++)
        {
            if (y == i)
                continue;

            auto& pts = m_players[y].m_points;

            for (size_t x = 0; x + 1 < pts.size(); x++)
            {
                if (dino::IntersectSegment(lastToLast, last, pts[x], pts[x + 1]))
                {
                    pts.erase(pts.begin(), pts.begin() + x + 1);
                    break;
                }
            }
        }
    }
}

void dino::Scene::CheckLoop(Player& player)
{
    Vec2 vecZero = Vec2(0, 0);

    std::vector<dino::Vec2> temp_point = player.CheckIfLoop();
    int size = temp_point.size();
    if (temp_point.size() > 0)
    {
        for (size_t i = m_movable.size(); i >0 ; i--)
        {
            int counter = 0;
            for (size_t y = 0; y < temp_point.size() - 1; y++)
            {
                if (dino::IntersectSegment(
                        temp_point[y], temp_point[y + 1], m_movable[i-1]->GetPos(), vecZero))
                {
                    ++counter;
                }
            }

            if (counter % 2 == 0)
                continue;

            Movable* target = m_movable[i-1];
            if (target == &player)
                continue;

            std::erase_if(m_animals, [&](const Animal& a) { return &a == target; });

            for (Player& playerCop : m_players)
            {
                if (target == &playerCop)
                {
                    playerCop.TakeDamage();
                }
            }
            std::erase(m_movable, target);
        }
    }
}

void dino::Scene::_UpdateAnimals(double absTime, float deltaTime)
{

    constexpr double kMaxSpawnInterval = 3.0; 
    constexpr double kMinSpawnInterval = 0.3; 

    double progress = 1.0 - std::clamp(m_timer / 60.0, 0.0, 1.0);

    double spawnInterval = kMaxSpawnInterval + (kMinSpawnInterval - kMaxSpawnInterval) * progress;

    if (absTime - m_animalSpawnTime >= spawnInterval)
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

    for (dino_tree const& tree : m_trees)
        tree.Draw();   

    
    //Convertit le timer en string en arrondissant
    std::string timeStr = std::to_string(std::lround(m_timer));
    std::string_view time = timeStr;
    
    //creer le visuel du timer
    std::vector<jv::gpu::Vertex> vsTimer;
    dino::GenVertices_Text(
        vsTimer, time, Color_BLACK, Color_TRANSPARENT, Vec2(m_whereTimer.x, m_whereTimer.y));
    jv::gpu::VertexBuffer* pVBufTimer = jv::gpu::CreateVertexBuffer("Timer", vsTimer);
    jv::gpu::Draw(pVBufTimer, m_pTextureText);
    jv::gpu::DestroyVertexBuffer(pVBufTimer);

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
