
#include <dino/dino_draw_utils.h>
#include <dino/dino_scene.h>
#include <dino/dino_geometry.h>
#include <format>
#include <vector>
#include <algorithm>

dino::Scene::Scene(): m_Terrain{24, 16}
    
{
    m_pTextureText = dino::LoadImageAsset("monogram-bitmap.bmp");
    m_pTextureAnimal = dino::LoadImageAsset("animals.bmp");
    m_pTexturePlayer = dino::LoadImageAsset("dinosaurs.bmp");

    m_Terrain.SetSeason(jv::util::RandomInt32(0, 3));

    for (int i = 0; i < 4; i++)
    {
        Vec2 spawnPos = m_Terrain.GenerateRandomSpawn();

        m_players.emplace_back(spawnPos, 0.0, jv::input::GamepadIdx(i),m_pTexturePlayer, m_pTextureText);

        m_entities.emplace_back(&m_players[i]);
    }

    SetupGame();
}

dino::Scene::~Scene() 
{
    jv::gpu::DestroyTexture(m_pTextureText);
    jv::gpu::DestroyTexture(m_pTextureAnimal);
    jv::gpu::DestroyTexture(m_pTexturePlayer);
}

void dino::Scene::Update(double absTime, float deltaTime)
{
    if (m_isGameRunning)
    {
        UpdateChrono(deltaTime);
        SpawnAnimals(absTime, deltaTime);

        for (Player& p : m_players)
        {
            p.RequestPause(m_isPaused, deltaTime);
        }
    }
    else
    {
        for (Player& p : m_players)
        {
            p.UpdateLobbyState();
        }
    }
    
    if (m_isPaused) return;

    m_lastDeltaTime = deltaTime;

    m_Terrain.Update(absTime, deltaTime);

    UpdateEntiy(absTime, deltaTime);

    UpdateEntiyCollision();

    UpdateEntityOrderInLayer();

    LassoCollisionCheck();

    UpdatePlayerLassoCollision();

    RebuildEntityList();
}

void dino::Scene::SpawnAnimals(double absTime, float deltaTime)
{
    constexpr double kSpawnTime = 0.5;

    float ratio = (m_chrono / 10.0f);

    if (absTime - m_animalSpawnTime >= kSpawnTime * ratio)
    {
        m_animalSpawnTime = absTime;

        Vec2 spawnPos = m_Terrain.GenerateRandomSpawn();
        
        m_animals.emplace_back(spawnPos, absTime, m_pTextureAnimal);
        m_entities.emplace_back(&m_animals.back());
    }
}

void dino::Scene::LassoCollisionCheck()
{
    for (int i = 0; i < m_players.size(); i++)
        for (int j = 0; j < m_players.size(); j++)
        {
            if (m_players[i].GetId() == m_players[j].GetId())
                continue;

            Vec2 lastA = {m_players[i].GetX(), m_players[i].GetY()};
            Vec2 lastB = m_players[i].GetLassoLastPos();

            if (m_players[i].GetLassoSize() >= 2)
            {
                lastB = m_players[i].GetLassoPosByIndex(m_players[i].GetLassoSize() - 2);
            }

            for (int k = 0; k < m_players[j].GetLassoSize() - 1; k++)
            {
                bool isIntersecting = IntersectSegment(lastA, lastB,
                    m_players[j].GetLassoPosByIndex(k), m_players[j].GetLassoPosByIndex(k + 1));

                if (isIntersecting)
                {
                    m_players[j].HandleLassoCollision(k);
                }
            }
        }
}

void dino::Scene::UpdateEntiy(double absTime, float deltaTime)
{
    for (Entity*& entity : m_entities)
    {
        entity->Update(absTime, deltaTime);
        entity->ResolveTerrainPos(m_Terrain);
    }
}

void dino::Scene::UpdateEntiyCollision()
{
    for (size_t i = 0; i < m_entities.size(); i++)
        for (size_t j = i + 1; j < m_entities.size(); j++)
        {
            if (m_entities[i] == m_entities[j])
                continue;
            m_entities[i]->ResolvePhysicConflict(*m_entities[j]);
        }
}

void dino::Scene::UpdateEntityOrderInLayer()
{
    std::sort(m_entities.begin(), m_entities.end(),
        [](Entity* const& a, Entity* const& b) { return a->GetY() < b->GetY(); });
}

void dino::Scene::UpdatePlayerLassoCollision()
{
    for (Player& player : m_players)
    {
        std::vector<dino::Vec2> result = player.UpdateLasso();

        if (result.size() <= 0)
            continue;

        int point = 0;

        for (Animal& a : m_animals)
        {
            if (dino::isInside(result, {a.GetX(), a.GetY()}))
                point += 10;
        }

        player.AddPoint(point);

        for (Entity* entity : m_entities)
        {
            if (entity == &player)
                continue;

            Vec2 entityPos = {entity->GetX(), entity->GetY()};

            if (dino::isInside(result, entityPos))
            {
                entity->CatchByPlayer();
                
            }
        }
    }
}

void dino::Scene::RebuildEntityList()
{
    std::erase_if(m_animals, [](Animal& animal) { return !animal.IsAlive(); });

    m_entities.clear();

    for (Player& player : m_players)
        m_entities.push_back(&player);

    for (Animal& animal : m_animals)
        m_entities.push_back(&animal);

    for (Tree& tree : m_trees)
        m_entities.push_back(&tree);
}

void dino::Scene::UpdateChrono(float deltaTime)
{
    if (m_chrono > 0)
    {
        m_chrono -= deltaTime;
    }
    else
    {
        m_chrono = 0;
        StopGame();
    }
}

void dino::Scene::DrawChrono() const
{
    std::string text = std::format("Time {:04.1f}s", m_chrono);
    std::vector<jv::gpu::Vertex> vs;
    dino::GenVertices_Text(vs, text, Color_WHITE, Color_BLACK, Vec2{200, 0});
    jv::gpu::VertexBuffer* pVBuf = jv::gpu::CreateVertexBuffer("Chrono", vs);
    jv::gpu::Draw(pVBuf, m_pTextureText);
    jv::gpu::DestroyVertexBuffer(pVBuf);
}


void dino::Scene::Draw() const
{
    m_Terrain.Draw();
    DrawChrono();
    for (Entity* const& entity : m_entities)
        entity->Draw();


    std::string text = std::format("dTime={:04.1f}ms", m_lastDeltaTime * 1000.0);
    std::vector<jv::gpu::Vertex> vs;
    dino::GenVertices_Text(vs, text, Color_WHITE, Color_GREY);
    jv::gpu::VertexBuffer* pVBuf = jv::gpu::CreateVertexBuffer("dTime", vs);
    jv::gpu::Draw(pVBuf, m_pTextureText);
    jv::gpu::DestroyVertexBuffer(pVBuf);
}

void dino::Scene::StartGame(int index)
{
    if (m_isGameRunning) return;

    m_isGameRunning = true;

    m_trees.clear();

    std::erase_if(m_players, [](Player& player) { return player.IsInLobby(); });

    m_Terrain.SetSeason(index);

    for (Player& p : m_players )
    {
        p.ResetPoint();
    }
}

void dino::Scene::StopGame()
{
    m_isGameRunning = false;

    m_animals.clear();
    RebuildEntityList();

    SetupGame();
}

void dino::Scene::SetupGame()
{
    for (int i = 0; i < 4; i++)
    {
        if (i < m_players.size()) continue;
            
        Vec2 spawnPos = m_Terrain.GenerateRandomSpawn();

        m_players.emplace_back(
            spawnPos, 0.0, jv::input::GamepadIdx(i), m_pTexturePlayer, m_pTextureText);

        m_entities.emplace_back(&m_players[i]);
    }


    for (int i = 0; i < 4; i++)
    {
        Vec2 spawnPos = m_Terrain.GenerateRandomSpawn();

        m_trees.emplace_back(spawnPos, 0.0, i, this);

        m_entities.emplace_back(&m_trees[i]);
    }

    m_chrono = 10.0f;
    m_isGameRunning = false;
}

