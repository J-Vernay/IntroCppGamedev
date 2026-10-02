
#include <dino/dino_draw_utils.h>
#include <dino/dino_scene.h>

#include <format>
#include <algorithm>

dino::Scene::Scene()
    : m_Terrain{24, 16},
      m_players
    {Player{m_Terrain.GenerateRandomSpawn(), 0, jv::input::GamepadIdx::Keyboard, 0},
          Player{m_Terrain.GenerateRandomSpawn(), 0, jv::input::GamepadIdx::Gamepad1, 1},
          Player{m_Terrain.GenerateRandomSpawn(), 0, jv::input::GamepadIdx::Gamepad2, 2},
          Player{m_Terrain.GenerateRandomSpawn(), 0, jv::input::GamepadIdx::Gamepad3, 3}}
    {
    m_pTextureText = dino::LoadImageAsset("monogram-bitmap.bmp");
    m_pTextureAnimal = dino::LoadImageAsset("animals.bmp");


    for (Player& p : m_players)
    {
        m_entitys.push_back(&p);
    }

    m_Terrain.SetSeason(jv::util::RandomInt32(0, 3));
}

dino::Scene::~Scene() 
{
    jv::gpu::DestroyTexture(m_pTextureAnimal);
    jv::gpu::DestroyTexture(m_pTextureText);
}

void dino::Scene::Update(double absTime, float deltaTime)
{
    m_lastDeltaTime = deltaTime;

    m_Terrain.Update(absTime, deltaTime);

    _UpdateAnimals(absTime, deltaTime);

    for (Player& p : m_players)
    {
        p.Update(absTime, deltaTime);
        p.HandlePhysics(m_entitys);
    }
    for (Entity* e : m_entitys)
    {
        e->HandleTerrainClamp(&m_Terrain);
    }

    SortEntitys();
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
        m_entitys.push_back(&m_animals.back());
    }

    

    for (Animal& animal : m_animals)
    {
        animal.Update(absTime, deltaTime);
        animal.HandlePhysics(m_entitys);
    }
}

void dino::Scene::Draw() const
{
    m_Terrain.Draw();

    for (const Entity* e : m_entitys)
    {
        e->Draw();
    }

    

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

void dino::Scene::SortEntitys()
{
    std::sort(m_entitys.begin(), m_entitys.end(), HeightDiff);
}

bool dino::Scene::HeightDiff(Entity* firstElt, Entity* secondElt)
{
    Vec2 first = firstElt->GetPosition();
    Vec2 second = secondElt->GetPosition();
    return (first.y < second.y);
}
