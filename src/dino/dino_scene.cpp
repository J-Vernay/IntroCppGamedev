
#include <dino/dino_draw_utils.h>
#include <dino/dino_scene.h>
#include <dino/Entity.h>
#include <algorithm>

#include <format>

dino::Scene::Scene() : m_Terrain{24, 16}
{
    m_pTextureText = dino::LoadImageAsset("monogram-bitmap.bmp");
    m_pTextureAnimal = dino::LoadImageAsset("animals.bmp");
    m_pTexturePlayer = dino::LoadImageAsset("dinosaurs.bmp");

    m_Terrain.SetSeason(jv::util::RandomInt32(0, 3));

    m_players.emplace_back(m_Terrain.GenerateRandomSpawn(), 0, jv::input::GamepadIdx::Keyboard, m_pTexturePlayer);
    m_players.emplace_back(m_Terrain.GenerateRandomSpawn(), 1, jv::input::GamepadIdx::Gamepad1, m_pTexturePlayer);
    m_players.emplace_back(m_Terrain.GenerateRandomSpawn(), 2, jv::input::GamepadIdx::Gamepad2, m_pTexturePlayer);
    m_players.emplace_back(m_Terrain.GenerateRandomSpawn(), 3, jv::input::GamepadIdx::Gamepad3, m_pTexturePlayer);
}

dino::Scene::~Scene()
{
    jv::gpu::DestroyTexture(m_pTexturePlayer);
    jv::gpu::DestroyTexture(m_pTextureAnimal);
}

void dino::Scene::Update(double absTime, float deltaTime)
{
    m_lastDeltaTime = deltaTime;

    m_Terrain.Update(absTime, deltaTime);

    _UpdateAnimals(absTime, deltaTime);

    for (Player& player : m_players)
        player.Update(absTime, deltaTime);

    for (Player& player : m_players)
    {
        player.CheckTerrain(m_Terrain);
    }

    for (int i = 0; i < m_players.size(); ++i)
        for (int j = i + 1; j < m_players.size(); ++j)
            Entity::ResolveCollision(m_players[i], m_players[j]);


    for (Animal& animal : m_animals)
    {
        animal.CheckTerrain(m_Terrain);

        m_entities.resize(0);
        for (Player& player : m_players)
            m_entities.push_back(&player);
        for (Animal& animal : m_animals)
            m_entities.push_back(&animal);

        std::sort(m_entities.begin(), m_entities.end(), Entity::OrderByPosY);
    }

    for (int i = 0; i < m_animals.size(); ++i)
        for (int j = i + 1; j < m_animals.size(); ++j)
            Entity::ResolveCollision(m_animals[i], m_animals[j]);

     for (int i = 0; i < m_players.size(); ++i)
        for (int j = i + 1; j < m_animals.size(); ++j)
            Entity::ResolveCollision(m_players[i], m_animals[j]);
}

void dino::Scene::_UpdateAnimals(double absTime, float deltaTime)
{
    // Spawner un animal si besoin.

    constexpr double kSpawnTime = 0.3;
    if (absTime - m_animalSpawnTime >= kSpawnTime)
    {
        m_animalSpawnTime = absTime;
        Vec2 spawnPos = m_Terrain.GenerateRandomSpawn();
        m_animals.emplace_back(spawnPos, absTime, m_pTextureAnimal);
    }

    for (Animal& animal : m_animals)
        animal.Update(absTime, deltaTime);
}

void dino::Scene::Draw() const
{
    m_Terrain.Draw();

    for (Entity const* entity : m_entities)
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
