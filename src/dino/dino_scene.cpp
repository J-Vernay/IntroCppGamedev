
#include <dino/dino_draw_utils.h>
#include <dino/dino_scene.h>

#include <format>

dino::Scene::Scene() : m_Terrain{24, 16}
{
    m_pTextureText = dino::LoadImageAsset("monogram-bitmap.bmp");

    m_Terrain.SetSeason(jv::util::RandomInt32(0, 3));
}

dino::Scene::~Scene() {}

void dino::Scene::Update(double absTime, float deltaTime)
{
    m_lastDeltaTime = deltaTime;

    m_Terrain.Update(absTime, deltaTime);

    _UpdateAnimals(absTime, deltaTime);
}

void dino::Scene::_UpdateAnimals(double absTime, float deltaTime)
{
    // Spawner un animal si besoin.

    constexpr double kSpawnTime = 0.3;
    if (absTime - m_animalSpawnTime >= kSpawnTime)
    {
        m_animalSpawnTime = absTime;
        Vec2 spawnPos = m_Terrain.GenerateRandomSpawn();
        m_animals.emplace_back(spawnPos, absTime);
    }

    for (Animal& animal : m_animals)
        animal.Update(absTime, deltaTime);
}

void dino::Scene::Draw() const
{
    m_Terrain.Draw();

    for (Animal const& animal : m_animals)
        animal.Draw();

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
