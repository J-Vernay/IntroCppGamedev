
#include <dino/dino_draw_utils.h>
#include <dino/dino_scene.h>

#include <format>

dino::Scene::Scene() : m_Terrain{24, 16} 
{
    m_pTextureText = dino::LoadImageAsset("monogram-bitmap.bmp");
    m_pTexture = dino::LoadImageAsset("animals.bmp");
    m_pTextureDinosaurs = dino::LoadImageAsset("dinosaurs.bmp");

    m_Terrain.SetSeason(jv::util::RandomInt32(0, 3));
    Vec2 spawnPos = m_Terrain.GenerateRandomSpawn();

    m_players.emplace_back(
        m_Terrain.GenerateRandomSpawn(), 0, m_pTextureDinosaurs, 0, jv::input::GamepadIdx::Keyboard);
    m_players.emplace_back(m_Terrain.GenerateRandomSpawn(), 0, m_pTextureDinosaurs, 1,
        jv::input::GamepadIdx::Gamepad1);
    m_players.emplace_back(m_Terrain.GenerateRandomSpawn(), 0, m_pTextureDinosaurs, 2,
        jv::input::GamepadIdx::Gamepad2);
    m_players.emplace_back(m_Terrain.GenerateRandomSpawn(), 0, m_pTextureDinosaurs, 3,
        jv::input::GamepadIdx::Gamepad3);
}

dino::Scene::~Scene() 
{
    jv::gpu::DestroyTexture(m_pTexture);
    jv::gpu::DestroyTexture(m_pTextureDinosaurs);
}

void dino::Scene::Update(double absTime, float deltaTime)
{
    m_lastDeltaTime = deltaTime;

    m_Terrain.Update(absTime, deltaTime);

    _UpdateAnimals(absTime, deltaTime);

    for (Player& player : m_players)
    {
        player.Update(absTime, deltaTime);
        player.CheckTerrain(m_Terrain);

        for (Player& player2 : m_players)
        {
        
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
        Vec2 spawnPos = m_Terrain.GenerateRandomSpawn();
        m_animals.emplace_back(spawnPos, absTime, m_pTexture);
    }

    for (Animal& animal : m_animals)
    {
        animal.Update(absTime, deltaTime);
        animal.CheckTerrain(m_Terrain);
    }
        
}

void dino::Scene::Draw() const
{
    m_Terrain.Draw();

    for (Animal const& animal : m_animals)
        animal.Draw();

    for (Player const& player : m_players)
        player.Draw();

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
