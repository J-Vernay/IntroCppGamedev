
#include <dino/dino_draw_utils.h>
#include <dino/dino_scene.h>
#include <dino/dino_player.h>
#include <dino/dino_geometry.h>
#include <string>
#include <format>
#include <algorithm>

dino::Scene::Scene() : m_Terrain{24, 16}
{
    m_pTextureText = dino::LoadImageAsset("monogram-bitmap.bmp");
    m_pTexturePlayer = dino::LoadImageAsset("dinosaurs.bmp");
    m_pTextureAnimal = dino::LoadImageAsset("animals.bmp");

    m_Terrain.SetSeason(jv::util::RandomInt32(0, 3));

   
    m_players.emplace_back(
        m_Terrain.GenerateRandomSpawn(), 0, jv::input::GamepadIdx::Keyboard, m_pTexturePlayer);
    m_players.emplace_back(
        m_Terrain.GenerateRandomSpawn(), 1, jv::input::GamepadIdx::Gamepad1, m_pTexturePlayer);
    m_players.emplace_back(
        m_Terrain.GenerateRandomSpawn(), 2, jv::input::GamepadIdx::Gamepad2, m_pTexturePlayer);
    m_players.emplace_back(
        m_Terrain.GenerateRandomSpawn(), 3, jv::input::GamepadIdx::Gamepad3, m_pTexturePlayer);
}

dino::Scene::~Scene() {
    jv::gpu::DestroyTexture(m_pTextureAnimal);
    jv::gpu::DestroyTexture(m_pTexturePlayer);
}

void dino::Scene::Update(double absTime, float deltaTime)
{
    m_lastDeltaTime = deltaTime;

    for (Player& player : m_players)
        if (player.PollPauseToggle())
            m_bPaused = !m_bPaused;

    if (m_bPaused)
        return;

    m_timer -= deltaTime;

    m_Terrain.Update(absTime, deltaTime);

    _UpdateAnimals(absTime, deltaTime);

    for (Player& player : m_players)
    {
        player.Update(absTime, deltaTime, m_Terrain);
    }

    for (int i = 0; i < m_players.size(); ++i)
        for (int j = i + 1; j < m_players.size(); ++j)
            Entity::ResolveCollision(m_players[i], m_players[j]);

    for (int i = 0; i < m_animals.size(); ++i)
        for (int j = i + 1; j < m_animals.size(); ++j)
            Entity::ResolveCollision(m_animals[i], m_animals[j]);

    for (int i = 0; i < m_players.size(); ++i)
        for (int j = 0; j < m_animals.size(); ++j)
            Entity::ResolveCollision(m_players[i], m_animals[j]);

    LassoCollisionCheck();
}

void dino::Scene::_UpdateAnimals(double absTime, float deltaTime)
{
    // Spawner un animal si besoin.

    constexpr double kSpawnTime = 0.3;
    if (absTime - m_animalSpawnTime >= kSpawnTime)
    {
        m_animalSpawnTime = absTime;
        Vec2 spawnPos = m_Terrain.GenerateRandomSpawn();
        m_animals.emplace_back(spawnPos, m_pTextureAnimal, absTime);
    }

    for (Animal& animal : m_animals)
        animal.Update(absTime, deltaTime, m_Terrain);

    constexpr float kTimerStart = 60.f;
    //ca prend en compte le timer pour faire varier la durée de vie des animaux
    float ratio = std::clamp(m_timer / kTimerStart, 0.f, 1.f);
    //durée de vie des animaux , 15 sec si timer est a fond et 0.5 si a 0
    float lifetime = 0.5f + 15.f * ratio;

    std::erase_if(m_animals, [lifetime](Animal const& a) { return a.IsExpired(lifetime); });
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

    std::string timeText = std::to_string(int(m_timer));
    std::string_view time = timeText;

    std::vector<jv::gpu::Vertex> vsTimer;
    dino::GenVertices_Text(
        vsTimer, time, Color_BLACK, Color_TRANSPARENT, Vec2(m_tmerPos.x, m_tmerPos.y));
    jv::gpu::VertexBuffer* pVBufTimer = jv::gpu::CreateVertexBuffer("Timer", vsTimer);
    jv::gpu::Draw(pVBufTimer, m_pTextureText);
    jv::gpu::DestroyVertexBuffer(pVBufTimer);
}

void dino::Scene::LassoCollisionCheck()
{
    std::vector<Vec2> loop;

    for (Player& player : m_players)
    {
        loop.clear();
        if (!player.FindLassoLoop(loop))
            continue;

        for (Animal& animal : m_animals)
        {
            if (isInside(loop, animal.GetPos()))
                animal.Caught();
        }

        for (Player& other : m_players)
        {
            if (&other == &player)
                continue;
            if (isInside(loop, other.GetPos()))
                other.Caught();
        }

        player.ClearLasso();
    }
    std::erase_if(m_animals, [](Animal const& a) { return a.IsCaught(); });
}

void dino::Scene::PauseGame() const
{
    std::string text = "PAUSE";
    std::vector<jv::gpu::Vertex> vs;

    
    jv::util::Vec2 size = jv::gpu::GetRenderSize();
    dino::GenVertices_Text(vs, text, Color_WHITE, Color_GREY, Vec2(size.x / 2 - 40, size.y / 2));

    jv::gpu::VertexBuffer* pVBuf = jv::gpu::CreateVertexBuffer("Pause", vs);
    jv::gpu::Draw(pVBuf, m_pTextureText);
    jv::gpu::DestroyVertexBuffer(pVBuf);

}