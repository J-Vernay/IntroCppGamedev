
#include <dino/dino_draw_utils.h>
#include <dino/dino_scene.h>
#include <format>

dino::Scene::Scene(): m_Terrain{24, 16}
    
{
    m_pTextureText = dino::LoadImageAsset("monogram-bitmap.bmp");
    m_pTextureAnimal = dino::LoadImageAsset("animals.bmp");

    m_Terrain.SetSeason(jv::util::RandomInt32(0, 3));

    for (int i = 0; i < 4; i++)
    {
        Vec2 spawnPos = m_Terrain.GenerateRandomSpawn();
        
        m_players.emplace_back(spawnPos, 0.0, jv::input::GamepadIdx(i));
        m_entities.emplace_back(&m_players[i]);
    }
}

dino::Scene::~Scene() 
{
    jv::gpu::DestroyTexture(m_pTextureText);
    jv::gpu::DestroyTexture(m_pTextureAnimal);
}

void dino::Scene::Update(double absTime, float deltaTime)
{
    m_lastDeltaTime = deltaTime;

    m_Terrain.Update(absTime, deltaTime);

    SpawnAnimals(absTime, deltaTime);

    for (Entity*& entity : m_entities)
    {
        entity->Update(absTime, deltaTime);
        entity->ResolveTerrainPos(m_Terrain);
    }

    for (size_t i = 0; i < m_entities.size(); i++)
        for (size_t j = i + 1; j < m_entities.size(); j++)
        {
            m_entities[i]->ResolvePhysicConflict(*m_entities[j]);
        }
}

void dino::Scene::SpawnAnimals(double absTime, float deltaTime)
{

    constexpr double kSpawnTime = 0.3;
    if (absTime - m_animalSpawnTime >= kSpawnTime)
    {
        m_animalSpawnTime = absTime;

        Vec2 spawnPos = m_Terrain.GenerateRandomSpawn();
        
        m_animals.emplace_back(spawnPos, absTime, m_pTextureAnimal);
        m_entities.emplace_back(&m_animals.back());
    }
}


void dino::Scene::Draw() const
{
    m_Terrain.Draw();

    for (Entity* const& entity : m_entities)
        entity->Draw();

    {
        std::string text = std::format("dTime={:04.1f}ms", m_lastDeltaTime * 1000.0);
        std::vector<jv::gpu::Vertex> vs;
        dino::GenVertices_Text(vs, text, Color_WHITE, Color_GREY);
        jv::gpu::VertexBuffer* pVBuf = jv::gpu::CreateVertexBuffer("dTime", vs);
        jv::gpu::Draw(pVBuf, m_pTextureText);
        jv::gpu::DestroyVertexBuffer(pVBuf);
    }
}
