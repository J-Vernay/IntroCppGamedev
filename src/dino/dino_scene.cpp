
#include <dino/dino_draw_utils.h>
#include <dino/dino_scene.h>

#include <format>
#include <iostream>
#include <algorithm>

dino::Scene::Scene() : m_Terrain{24, 16}
{
    m_pTextureText = dino::LoadImageAsset("monogram-bitmap.bmp");
    m_animalTexture = dino::LoadImageAsset("animals.bmp");
    m_playerTexture = dino::LoadImageAsset("dinosaurs.bmp");

    m_Terrain.SetSeason(jv::util::RandomInt32(0, 3));

    Color colors[4] = {Color_BLUE, Color_RED, Color_YELLOW, Color_GREEN};
    for (int i = 0; i < 4; i++) {
        Entity& player = m_players.emplace_back(m_Terrain.GenerateRandomSpawn(), 0, m_playerTexture,
            m_gamepads[i], colors[i], i, &m_Terrain);
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
    m_lastDeltaTime = deltaTime;

    m_Terrain.Update(absTime, deltaTime);

    _UpdatePlayers(absTime, deltaTime);
    //_UpdateAnimals(absTime, deltaTime);
    _UpdateCollisions(absTime, deltaTime);
}

void dino::Scene::_UpdatePlayers(double absTime, float deltaTime)
{
    for (int i = 0; i < m_players.size(); i++)
    {
        m_players[i].Update(absTime, deltaTime);

        m_playerLastMoves.clear();
        
        for (int j = 0; j < m_players.size(); j++)
        {
            if (i == j) continue;

            m_playerLastMoves.push_back(
                std::pair<Vec2, Vec2>(m_players[j].GetLastPos(), m_players[j].GetPos()));
        }

        m_players[i].UpdateTrail(absTime, deltaTime, m_playerLastMoves);
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
        Entity& animal = m_animals.emplace_back(spawnPos, absTime, m_animalTexture, &m_Terrain);
        m_entities.push_back(&animal);
    }

    for (Animal& animal : m_animals)
        animal.Update(absTime, deltaTime);
}

void dino::Scene::_UpdateCollisions(double absTime, float deltaTime)
{
    // Les dinosaures se poussent entre eux.
    for (int i = 0; i < m_players.size(); i++)
    {
        for (int j = i + 1; j < m_players.size(); j++)
        {
            m_players[i].Collide(m_players[j]);
        }
    }

    // Les animaux se poussent entre eux.
    for (int i = 0; i < m_animals.size(); i++)
    {
        for (int j = i + 1; j < m_animals.size(); j++)
        {
            m_animals[i].Collide(m_animals[j]);
        }
    }

    // Les joueurs se poussent avec les animaux.
    for (int i = 0; i < m_players.size(); i++)
    {
        for (int j = 0; j < m_animals.size(); j++)
        {
            m_players[i].Collide(m_animals[j]);
        }
    }
}

void dino::Scene::SortEntities()
{
    std::sort(m_entities.begin(), m_entities.end(),
        [](Entity* a, Entity* b) { return a->GetPos().y < b->GetPos().y; });
}

void dino::Scene::Draw() const
{
    m_Terrain.Draw();

    // Afficher les entités dans l'ordre.
    for (Entity* entity : m_entities)
        entity->Draw();
    for (Player const& player : m_players)
        player.DrawTrail();

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
