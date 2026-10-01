
#include <dino/dino_draw_utils.h>
#include <dino/dino_scene.h>

#include <format>

dino::Scene::Scene()
    : m_Terrain{24, 16},
      m_players
    {Player{m_Terrain.GenerateRandomSpawn(), 0, jv::input::GamepadIdx::Keyboard, 0, &m_Terrain},
          Player{m_Terrain.GenerateRandomSpawn(), 0, jv::input::GamepadIdx::Gamepad1, 1, &m_Terrain},
          Player{m_Terrain.GenerateRandomSpawn(), 0, jv::input::GamepadIdx::Gamepad2, 2, &m_Terrain},
          Player{m_Terrain.GenerateRandomSpawn(), 0, jv::input::GamepadIdx::Gamepad3, 3, &m_Terrain}}
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
        //p.HandlePhysics(m_entitys);
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
        m_animals.emplace_back(spawnPos, absTime, m_pTextureAnimal, &m_Terrain);
    }

    

    for (Animal& animal : m_animals)
    {
        animal.Update(absTime, deltaTime);
        //animal.HandlePhysics(m_entitys);
    }
}

void dino::Scene::Draw() const
{
    m_Terrain.Draw();
    for (const Player& p : m_players)
    {
        p.Draw();
    }

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
