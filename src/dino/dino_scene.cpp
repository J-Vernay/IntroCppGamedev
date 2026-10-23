
#include <dino/dino_draw_utils.h>
#include <dino/dino_scene.h>

#include "dino_geometry.h"

#include <format>
#include <algorithm>

dino::Scene::Scene() : m_Terrain{24, 16}
{
    m_pTextureText = dino::LoadImageAsset("monogram-bitmap.bmp");
    m_pTextureAnimals = dino::LoadImageAsset("animals.bmp");
    m_pTexturePlayers = dino::LoadImageAsset("dinosaurs.bmp");

    m_Terrain.SetSeason(jv::util::RandomInt32(0, 3));

    Vec2 renderSize = jv::gpu::GetRenderSize();

    Vec2 center{renderSize.x * 0.5f, renderSize.y * 0.5f};

    const jv::input::GamepadIdx devices[4] = {jv::input::GamepadIdx::Keyboard,
        jv::input::GamepadIdx::Gamepad1, jv::input::GamepadIdx::Gamepad2,
        jv::input::GamepadIdx::Gamepad3};

    const Vec2 offsets[4] = {{-32.f, -24.f}, {32.f, -24.f}, {-32.f, 24.f}, {32.f, 24.f}};

    for (int32_t i = 0; i < 4; ++i)
    {
        Vec2 playerPos{center.x + offsets[i].x, center.y + offsets[i].y};

        m_players.emplace_back(playerPos, m_pTexturePlayers, devices[i], i);
    }
}

dino::Scene::~Scene()
{
    m_players.clear();
    m_animals.clear();

    jv::gpu::DestroyTexture(m_pTexturePlayers);
    jv::gpu::DestroyTexture(m_pTextureAnimals);
    jv::gpu::DestroyTexture(m_pTextureText);
}

void dino::Scene::Update(double absTime, float deltaTime)
{
    m_lastDeltaTime = deltaTime;

    const bool launchRequested = _UpdateInputs();

    if (m_isPaused || deltaTime <= 0.f)
        return;

    if (m_gameState == GameState::Lobby)
    {
        m_lobbyDelayRemaining =
            std::max(0.0, m_lobbyDelayRemaining - static_cast<double>(deltaTime));

        if (launchRequested && m_lobbyDelayRemaining <= 0.0)
        {
            _StartRound();
            return;
        }
    }

    const double step = m_gameState == GameState::Playing
    ? std::min(static_cast<double>(deltaTime), m_roundTimeRemaining)
    : static_cast<double>(deltaTime);

    const float gameDeltaTime = static_cast<float>(step);

    m_simulationTime += step;

    if (m_gameState == GameState::Playing)
    {
        m_roundTimeRemaining = std::max(0.0, m_roundTimeRemaining - step);
    }

    std::vector<Vec2> previousPositions;
    previousPositions.reserve(m_players.size());

    for (const Player& player : m_players)
    {
        previousPositions.push_back(player.GetPosition());
    }

    m_Terrain.Update(m_simulationTime, gameDeltaTime);

    if (m_gameState == GameState::Playing)
    {
        _UpdateAnimals(m_simulationTime, gameDeltaTime);
    }

    for (Player& player : m_players)
    {
        player.Update(m_simulationTime, gameDeltaTime);
        player.ClampToTerrain(m_Terrain);
    }

    _ResolveCollisions();

    for (Player& player : m_players)
    {
        player.UpdateLasso(gameDeltaTime);
    }

    _CutPlayerLassos(previousPositions);

    if (m_gameState == GameState::Playing)
    {
        _CheckClosedLoops();

        if (m_roundTimeRemaining <= 0.0)
        {
            _EnterLobby();
        }
    }
}

void dino::Scene::_UpdateAnimals(double absTime, float deltaTime)
{
    const double progress = std::clamp(1.0 - m_roundTimeRemaining / kRoundDuration, 0.0, 1.0);

    constexpr double startInterval = 1.0;
    constexpr double endInterval = 0.15;

    const double spawnInterval = startInterval + (endInterval - startInterval) * progress;

    m_animalSpawnAccumulator += deltaTime;

    while (m_animalSpawnAccumulator >= spawnInterval)
    {
        m_animalSpawnAccumulator -= spawnInterval;

        const Vec2 spawnPos = m_Terrain.GenerateRandomSpawn();

        m_animals.emplace_back(spawnPos, absTime, m_pTextureAnimals);
    }

    for (Animal& animal : m_animals)
    {
        animal.Update(absTime, deltaTime);
        animal.ClampToTerrain(m_Terrain);
    }
}

void dino::Scene::Draw() const
{
    m_Terrain.Draw();

    for (const Player& player : m_players)
        player.DrawLasso();

    std::vector<const Entity*> entities;
    entities.reserve(m_players.size() + m_animals.size());

    for (const Animal& animal : m_animals)
        entities.push_back(&animal);

    for (const Player& player : m_players)
        entities.push_back(&player);

    std::stable_sort(entities.begin(), entities.end(),
        [](const Entity* a, const Entity* b) { return a->GetPosition().y < b->GetPosition().y; });

    for (const Entity* entity : entities)
        entity->Draw();

    // Affichage du temps de la mise à jour précédente.
    {
       
        std::string text = std::format("dTime={:04.1f}ms | Captures: {}", m_lastDeltaTime * 1000.0, m_lastLoopAnimalCount);

        std::vector<jv::gpu::Vertex> vertices;

        dino::GenVertices_Text(vertices, text, Color_WHITE, Color_GREY, {8.f, jv::gpu::GetRenderSize().y - 16.f});

        jv::gpu::VertexBuffer* pVBuf = jv::gpu::CreateVertexBuffer("dTime", vertices);

        jv::gpu::Draw(pVBuf, m_pTextureText);
        jv::gpu::DestroyVertexBuffer(pVBuf);
    }

    _DrawScores();
    _DrawRoundTimer();
    _DrawPauseMessage();


}

void dino::Scene::_StartRound()
{

    const bool hasPlayer = std::any_of(
        m_players.begin(), m_players.end(), [](const Player& player) { return player.IsActive(); });

    if (!hasPlayer)
        return;

    m_animals.clear();

    m_animalSpawnAccumulator = 0.0;
    m_lastLoopAnimalCount = 0;

    m_roundTimeRemaining = kRoundDuration;
    m_lobbyDelayRemaining = 0.0;

    m_isPaused = false;

    for (Player& player : m_players)
    {
        player.ResetState(true);
    }

    m_gameState = GameState::Playing;
}

void dino::Scene::_EnterLobby()
{
    m_gameState = GameState::Lobby;

    m_isPaused = false;
    m_lobbyDelayRemaining = 5.0;

    m_animals.clear();

    m_animalSpawnAccumulator = 0.0;
    m_lastLoopAnimalCount = 0;

    for (Player& player : m_players)
    {
        player.ResetState(false);
    }
}

void dino::Scene::_ResolveCollisions()
{
    std::vector<Entity*> entities;
    entities.reserve(m_players.size() + m_animals.size());

    for (Player& player : m_players)
    {
        if (player.IsActive())
        {
            entities.push_back(&player);
        }
    }

    for (Animal& animal : m_animals)
        entities.push_back(&animal);

    constexpr int solverIterations = 4;

    for (int iteration = 0; iteration < solverIterations; ++iteration)
    {
        for (std::size_t i = 0; i < entities.size(); ++i)
        {
            for (std::size_t j = i + 1; j < entities.size(); ++j)
            {
                Entity& entityA = *entities[i];
                Entity& entityB = *entities[j];

                Vec2 posA = entityA.GetPosition();
                Vec2 posB = entityB.GetPosition();

                if (ResolveCircleCollision(posA, entityA.GetRadius(), posB, entityB.GetRadius()))
                {
                    entityA.SetPosition(posA);
                    entityB.SetPosition(posB);
                }
            }
        }

        for (Entity* entity : entities)
            entity->ClampToTerrain(m_Terrain);
    }
}

void dino::Scene::_CutPlayerLassos(const std::vector<Vec2>& previousPositions)
{
    for (std::size_t i = 0; i < m_players.size(); ++i)
    {
        if (!m_players[i].IsActive())
            continue;

        const Vec2 from = previousPositions[i];
        const Vec2 to = m_players[i].GetPosition();

        for (std::size_t j = 0; j < m_players.size(); ++j)
        {
            if (i == j || !m_players[j].IsActive())
                continue;

            m_players[j].CutLasso(from, to);
        }
    }
}

void dino::Scene::_CheckClosedLoops()
{
    std::vector<Entity*> entities;
    entities.reserve(m_animals.size() + m_players.size());

    for (Animal& animal : m_animals)
    {
        entities.push_back(&animal);
    }

    for (Player& player : m_players)
    {
        if (player.IsActive())
        {
            entities.push_back(&player);
        }
    }

    for (Player& owner : m_players)
    {
        const std::vector<Vec2>& loop = owner.GetClosedLoop();

        if (loop.size() < 3)
            continue;

        m_lastLoopAnimalCount = 0;

        for (Entity* entity : entities)
        {
            if (!owner.IsActive())
                continue;

            const std::vector<Vec2>& loop = owner.GetClosedLoop();

            if (entity == &owner)
                continue;

            if (entity->IsMarkedForRemoval())
                continue;

            if (!IsPointInsidePolygon(entity->GetPosition(), loop))
                continue;

            entity->OnCaptured();

            if (entity->IsMarkedForRemoval())
            {
                ++m_lastLoopAnimalCount;
                owner.AddScore(10);
            }
        }
    }

    m_animals.remove_if([](const Animal& animal) { return animal.IsMarkedForRemoval(); });
}

void dino::Scene::_DrawScores() const
{
    const Color playerColors[4] = {Color_BLUE, Color_RED, Color_YELLOW, Color_GREEN};

    std::vector<jv::gpu::Vertex> vertices;

    for (std::size_t i = 0; i < m_players.size(); ++i)
    {
        const Player& player = m_players[i];

        std::string text = std::format("J{}\n{}", i + 1, player.GetScore());

        if (!player.IsActive())
        {
            text += "\nStart";
        }

        const Vec2 position{8.f, 80.f + 48.f * static_cast<float>(i)};

        GenVertices_Text(
            vertices, text, playerColors[player.GetColorIndex()], Color_BLACK, position);
    }

    if (vertices.empty())
        return;

    jv::gpu::VertexBuffer* buffer = jv::gpu::CreateVertexBuffer("Scores", vertices);

    jv::gpu::Draw(buffer, m_pTextureText);
    jv::gpu::DestroyVertexBuffer(buffer);
}


void dino::Scene::_DrawRoundTimer() const
{
    std::string text;

    if (m_gameState == GameState::Lobby)
    {
        if (m_lobbyDelayRemaining > 0.0)
        {
            text = std::format(
                "Lobby - Attendez {} s", static_cast<int>(std::ceil(m_lobbyDelayRemaining)));
        }
        else
        {
            text = "Start: rejoindre | Select: quitter | Z/W: lancer";
        }
    }
    else
    {
        text = std::format("Temps: {}", static_cast<int>(std::ceil(m_roundTimeRemaining)));
    }

    const Vec2 renderSize = jv::gpu::GetRenderSize();

    const float textWidth = 6.f * static_cast<float>(text.size());

    std::vector<jv::gpu::Vertex> vertices;

    GenVertices_Text(
        vertices, text, Color_WHITE, Color_BLACK, {(renderSize.x - textWidth) * 0.5f, 8.f});

    jv::gpu::VertexBuffer* buffer = jv::gpu::CreateVertexBuffer("RoundTimer", vertices);

    jv::gpu::Draw(buffer, m_pTextureText);
    jv::gpu::DestroyVertexBuffer(buffer);
}

void dino::Scene::_DrawPauseMessage() const
{
    if (!m_isPaused)
        return;

    const Vec2 renderSize = jv::gpu::GetRenderSize();

    const std::string title = "PAUSE";
    const std::string hint = "Start / Espace / Entree";

    std::vector<jv::gpu::Vertex> vertices;

    GenVertices_Text(vertices, title, Color_YELLOW, Color_BLACK,
        {(renderSize.x - 6.f * static_cast<float>(title.size())) * 0.5f,
            renderSize.y * 0.5f - 18.f});

    GenVertices_Text(vertices, hint, Color_WHITE, Color_BLACK,
        {(renderSize.x - 6.f * static_cast<float>(hint.size())) * 0.5f, renderSize.y * 0.5f + 2.f});

    jv::gpu::VertexBuffer* buffer = jv::gpu::CreateVertexBuffer("Pause", vertices);

    jv::gpu::Draw(buffer, m_pTextureText);
    jv::gpu::DestroyVertexBuffer(buffer);
}

bool dino::Scene::_UpdateInputs()
{
    bool launchRequested = false;
    bool pauseRequested = false;

    for (std::size_t i = 0; i < m_startWasDown.size(); ++i)
    {
        const auto device = jv::input::GamepadIdx_ALL[i];

        jv::input::Gamepad gamepad{};

        const bool connected = jv::input::GetGamepad(device, gamepad);

        const bool startDown = connected && gamepad.start;
        const bool selectDown = connected && gamepad.select;
        const bool launchDown = connected && gamepad.btn_up;

        const bool startPressed = startDown && !m_startWasDown[i];

        const bool selectPressed = selectDown && !m_selectWasDown[i];

        const bool launchPressed = launchDown && !m_launchWasDown[i];

        m_startWasDown[i] = startDown;
        m_selectWasDown[i] = selectDown;
        m_launchWasDown[i] = launchDown;

        Player* participant = nullptr;

        for (Player& player : m_players)
        {
            if (player.IsActive() && player.GetGamepadIdx() == device)
            {
                participant = &player;
                break;
            }
        }

        if (m_gameState == GameState::Lobby)
        {
            if (selectPressed && participant != nullptr)
            {
                participant->Leave();
                continue;
            }

            if (startPressed && participant == nullptr)
            {
                for (Player& player : m_players)
                {
                    if (!player.IsActive())
                    {
                        player.Join(device);
                        participant = &player;
                        break;
                    }
                }
            }
            if (launchPressed && participant != nullptr)
            {
                launchRequested = true;
            }
        }
        else if (startPressed && participant != nullptr)
        {
            pauseRequested = true;
        }
    }

    if (pauseRequested)
    {
        m_isPaused = !m_isPaused;
    }

    return launchRequested;
}