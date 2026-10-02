
#include <dino/dino_draw_utils.h>
#include <dino/dino_scene.h>
#include <dino/dino_geometry.h>

#include <format>
#include <iostream>
#include <algorithm>
#include <deque>

dino::Scene::Scene() : m_terrain{24, 16}
{
    m_pTextureText = dino::LoadImageAsset("monogram-bitmap.bmp");
    m_animalTexture = dino::LoadImageAsset("animals.bmp");
    m_playerTexture = dino::LoadImageAsset("dinosaurs.bmp");

    m_terrain.SetSeason(jv::util::RandomInt32(0, 3));

    Color colors[4] = {Color_BLUE, Color_RED, Color_YELLOW, Color_GREEN};
    for (int i = 0; i < 4; i++) {
        Entity& player = m_players.emplace_back(m_terrain.GenerateRandomSpawn(), 0, m_playerTexture,
            m_gamepads[i], colors[i], i);
        m_entities.push_back(&player);
    }
}

dino::Scene::~Scene()
{
    jv::gpu::DestroyTexture(m_pTextureText);
    jv::gpu::DestroyTexture(m_animalTexture);
    jv::gpu::DestroyTexture(m_playerTexture);

}

void dino::Scene::Update(double absTime, float deltaTime)
{
    _CheckPause();

    if (m_bPause)
        return;

    m_lastDeltaTime = deltaTime;

    m_terrain.Update(absTime, deltaTime);
    m_timer -= deltaTime;

    _UpdatePlayers(absTime, deltaTime);
    _UpdateAnimals(absTime, deltaTime);
    _UpdateCollisions(absTime, deltaTime);
}

void dino::Scene::_CheckPause()
{
    for (Player& player : m_players)
    {
        if (player.Pauses())
        {
            m_bPause = !m_bPause;
        }
    }
}

void dino::Scene::_UpdatePlayers(double absTime, float deltaTime)
{
    for (int i = 0; i < m_players.size(); i++)
    {
        m_players[i].Update(absTime, deltaTime, m_terrain);

        m_playerLastMoves.clear();
        
        for (int j = 0; j < m_players.size(); j++)
        {
            if (i == j) continue;

            m_playerLastMoves.push_back(
                std::pair<Vec2, Vec2>(m_players[j].GetLastPos(), m_players[j].GetPos()));
        }

        m_players[i].UpdateTrail(absTime, deltaTime, m_playerLastMoves);
        
        std::pair<int32_t, int32_t> loop = m_players[i].CheckLoop();
        if (loop.first != -1)
        {
            _OnPlayerLoop(&m_players[i], loop.first, loop.second);
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
        Vec2 spawnPos = m_terrain.GenerateRandomSpawn();
        Entity& animal = m_animals.emplace_back(spawnPos, absTime, m_animalTexture);
        m_entities.push_back(&animal);
    }

    bool animalDied = false;

    for (Animal& animal : m_animals)
    {
        animal.Update(absTime, deltaTime, m_terrain);
        if (animal.m_bShouldDie)
            animalDied = true;
    }

    std::erase_if(m_animals, [](Animal a) { return a.m_bShouldDie; });

    if (animalDied)
    {
        m_entities.clear();
        for (Player& player : m_players)
            m_entities.push_back(&player);
        for (Animal& animal : m_animals)
            m_entities.push_back(&animal);
    }
}

void dino::Scene::_UpdateCollisions(double absTime, float deltaTime)
{
    // Les dinosaures se poussent entre eux.
    for (int i = 0; i < m_players.size(); i++)
    {
        for (int j = i + 1; j < m_players.size(); j++)
        {
            m_players[i].Collide(m_players[j], m_terrain);
        }
    }

    // Les animaux se poussent entre eux.
    for (int i = 0; i < m_animals.size(); i++)
    {
        for (int j = i + 1; j < m_animals.size(); j++)
        {
            m_animals[i].Collide(m_animals[j], m_terrain);
        }
    }

    // Les joueurs se poussent avec les animaux.
    for (int i = 0; i < m_players.size(); i++)
    {
        for (int j = 0; j < m_animals.size(); j++)
        {
            m_players[i].Collide(m_animals[j], m_terrain);
        }
    }
}

void dino::Scene::_DrawTimer() const
{
    std::string text = std::format("{:04.2f}", m_timer);
    std::vector<jv::gpu::Vertex> vs;

    float renderScale = 3;
    Vec2 renderSize = jv::gpu::GetRenderSize();
    Vec2 textSize = GenVertices_Text(vs, text, Color_WHITE, Color_GREY, {(renderSize.x / 2) / renderScale, 0});
    for (jv::gpu::Vertex& vertex : vs)
    {
        vertex.pos = {vertex.pos.x - textSize.x / 2, vertex.pos.y};
    }
    jv::gpu::VertexBuffer* pVBuf = jv::gpu::CreateVertexBuffer("Timer", vs);
    jv::gpu::Draw(pVBuf, m_pTextureText, {{0, 0}, 0, {renderScale, renderScale}});
    jv::gpu::DestroyVertexBuffer(pVBuf);
}

void dino::Scene::SortEntities()
{
    std::sort(m_entities.begin(), m_entities.end(),
        [](Entity* a, Entity* b) { return a->GetPos().y < b->GetPos().y; });
}

void dino::Scene::Draw() const
{
    m_terrain.Draw();

    // Afficher les lassos.
    for (Player const& player : m_players)
        player.DrawTrail();

    // Afficher les entités dans l'ordre.
    for (Entity* entity : m_entities)
        entity->Draw();

    _DrawTimer();

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

void dino::Scene::_OnPlayerLoop(Player* pPlayer, int32_t start, int32_t end)
{
    int minLoopSize = 10;

    if (abs(end - start) > minLoopSize)
    {
        PointList pastPositions = pPlayer->GetTrail();

        for (Entity* entity : m_entities)
        {
            if (entity == pPlayer)
                continue;

            Vec2 entityPos = entity->GetPos();

            int32_t segmentHitCount = 0;
            int32_t segmentHitCount2 = 0;

            for (int i = start; i <= end; i++)
            {
                Vec2 loopSegmentA = pastPositions[i == start ? end : i - 1];
                Vec2 loopSegmentB = pastPositions[i];

                // Raycast outside of terrain.
                if (IntersectSegment(entityPos, {}, loopSegmentA, loopSegmentB))
                {
                    segmentHitCount++;
                }
            }

            // Odd segment hit count = in loop.
            if (segmentHitCount % 2 == 1)
            {
                entity->OnCaughtInLoop();
            }
        }
    }

    pPlayer->CutLoop(start, end);
}
