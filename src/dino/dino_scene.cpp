
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
    m_treeTexture = dino::LoadImageAsset("terrain.bmp");

    m_terrain.SetSeason(jv::util::RandomInt32(0, 3));

    Color colors[4] = {Color_BLUE, Color_RED, Color_YELLOW, Color_GREEN};
    for (int i = 0; i < 4; i++)
    {
        Entity& player = m_players.emplace_back(m_terrain.GenerateRandomSpawn(), 0, m_playerTexture,
            m_gamepads[i], colors[i], i);
    }

    for (int i = 0; i < 4; i++)
    {
        Tree& tree = m_trees.emplace_back(m_terrain.GenerateRandomSpawn(), i, m_treeTexture);
        m_entities.push_back(&tree);
    }
}

dino::Scene::~Scene()
{
    jv::gpu::DestroyTexture(m_pTextureText);
    jv::gpu::DestroyTexture(m_animalTexture);
    jv::gpu::DestroyTexture(m_playerTexture);
    jv::gpu::DestroyTexture(m_treeTexture);

}

void dino::Scene::Update(double absTime, float deltaTime)
{
    _CheckJoinLeave();
    _CheckPause();

    if (m_bPause)
        return;

    m_lastDeltaTime = deltaTime;

    m_terrain.Update(absTime, deltaTime);
    if (m_bGameStarted) m_timer -= deltaTime;

    _UpdatePlayers(absTime, deltaTime);
    if (!m_bGameStarted)
        _CheckGameStart();
    _UpdateCollisions(absTime, deltaTime);
    _UpdateAnimals(absTime, deltaTime);
}

void dino::Scene::_StartGame(int32_t idxSeason)
{
    m_bGameStarted = true;
    m_terrain.SetSeason(idxSeason);
    _RefreshEntities();
}

void dino::Scene::_CheckPause()
{
    // Les joueurs ne peuvent pas faire pause dans le lobby.
    if (!m_bGameStarted)
        return;

    for (Player* player : m_activePlayers)
    {
        if (player->Pauses())
        {
            m_bPause = !m_bPause;
        }
    }
}

void dino::Scene::_CheckJoinLeave()
{
    // Ne pas vérifier si le jeu a déjà démarré.
    if (m_bGameStarted)
        return;

    for (Player& player : m_players)
    {
        if (player.CheckJoin())
        {
            m_activePlayers.push_back(&player);
            m_entities.push_back(&player);
        }
        if (player.CheckLeave())
        {
            int leavingPlayerIndex = -1;
            for (int i = 0; i < m_activePlayers.size(); i++)
            {
                if (&player == m_activePlayers[i])
                {
                    leavingPlayerIndex = i;
                }
            }
            m_activePlayers.erase(m_activePlayers.begin() + leavingPlayerIndex);
            _RefreshEntities();
        }
    }
}

void dino::Scene::_UpdatePlayers(double absTime, float deltaTime)
{
    for (int i = 0; i < m_activePlayers.size(); i++)
    {
        m_activePlayers[i]->Update(absTime, deltaTime, m_terrain);

        m_playerLastMoves.clear();
        
        for (int j = 0; j < m_activePlayers.size(); j++)
        {
            if (i == j) continue;

            m_playerLastMoves.push_back(
                std::pair<Vec2, Vec2>(m_activePlayers[j]->GetLastPos(), m_activePlayers[j]->GetPos()));
        }

        m_activePlayers[i]->UpdateTrail(absTime, deltaTime, m_playerLastMoves);
        
        std::pair<int32_t, int32_t> loop = m_activePlayers[i]->CheckLoop();
        if (loop.first != -1)
        {
            _OnPlayerLoop(m_activePlayers[i], loop.first, loop.second);
        }
    }
}

void dino::Scene::_UpdateAnimals(double absTime, float deltaTime)
{
    // Aucun animal n'apparaît si le jeu n'a pas commencé.
    if (!m_bGameStarted)
        return;

    // Spawner un animal si besoin.
    constexpr double kStartSpawnTime = 1;
    constexpr double kEndSpawnTime = 0.1;
    float t = 1 - m_timer / GameTime;
    float lerpSpawnTime = (kEndSpawnTime - kStartSpawnTime) * t + kStartSpawnTime;

    if (absTime - m_animalSpawnTime >= lerpSpawnTime)
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
        _RefreshEntities();
    }
}

void dino::Scene::_UpdateCollisions(double absTime, float deltaTime)
{
    // Les dinosaures se poussent entre eux.
    for (int i = 0; i < m_activePlayers.size(); i++)
    {
        for (int j = i + 1; j < m_activePlayers.size(); j++)
        {
            m_activePlayers[i]->Collide(*m_activePlayers[j], m_terrain);
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
    for (int i = 0; i < m_activePlayers.size(); i++)
    {
        for (int j = 0; j < m_animals.size(); j++)
        {
            m_activePlayers[i]->Collide(m_animals[j], m_terrain);
        }
    }

    // Les joueurs et les arbres se poussent entre eux.
    if (!m_bGameStarted)
    {
        for (int i = 0; i < m_activePlayers.size(); i++)
        {
            for (int j = 0; j < m_trees.size(); j++)
            {
                m_activePlayers[i]->Collide(m_trees[j], m_terrain);
            }
        }
    }
}

void dino::Scene::_CheckGameStart()
{
    for (int i = 0; i < m_trees.size(); i++)
    {
        if (m_trees[i].ShouldStartGame())
        {
            _StartGame(i);
            return;
        }
    }
}

void dino::Scene::_RefreshEntities()
{
    m_entities.clear();
    for (Player* player : m_activePlayers)
        m_entities.push_back(player);
    for (Animal& animal : m_animals)
        m_entities.push_back(&animal);
    if (!m_bGameStarted)
        for (Tree& tree : m_trees)
            m_entities.push_back(&tree);
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
    for (Player const* player : m_activePlayers)
        player->DrawTrail();

    // Afficher les entités dans l'ordre.
    for (Entity* entity : m_entities)
        entity->Draw();

    if (m_bGameStarted)
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
