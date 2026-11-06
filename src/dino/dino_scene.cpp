#include <dino/dino_draw_utils.h>
#include <dino/dino_scene.h>

#include <algorithm>
#include <array>
#include <format>
#include <string>
#include <vector>

namespace
{

constexpr std::array<jv::input::GamepadIdx, 4> kGamepadIndices{
    jv::input::GamepadIdx::Keyboard,
    jv::input::GamepadIdx::Gamepad1,
    jv::input::GamepadIdx::Gamepad2,
    jv::input::GamepadIdx::Gamepad3,
};

constexpr std::array<dino::Color, 4> kPlayerColors{
    dino::Color_BLUE,
    dino::Color_RED,
    dino::Color_YELLOW,
    dino::Color_GREEN,
};

constexpr std::array<std::string_view, 4> kSeasonNames{
    "PRINTEMPS",
    "ETE",
    "AUTOMNE",
    "HIVER",
};

// Vérifie si un point se trouve à l'intérieur d'un polygone.
bool IsInsidePolygon(dino::Vec2 point, std::vector<dino::Vec2> const& polygon)
{
    if (polygon.size() < 4)
        return false;

    bool inside = false;

    for (size_t i = 0, j = polygon.size() - 1; i < polygon.size(); j = i++)
    {
        dino::Vec2 const& a = polygon[i];
        dino::Vec2 const& b = polygon[j];

        bool crosses = (a.y > point.y) != (b.y > point.y);
        if (!crosses)
            continue;

        float x = (b.x - a.x) * (point.y - a.y) / (b.y - a.y) + a.x;
        if (point.x < x)
            inside = !inside;
    }

    return inside;
}

} // namespace


// Initialise le terrain, les textures et les emplacements des joueurs.
dino::Scene::Scene() : m_Terrain{24, 16}
{
    m_pTextureText = dino::LoadImageAsset("monogram-bitmap.bmp");
    m_pTextureAnimal = dino::LoadImageAsset("animals.bmp");
    m_pTexturePlayer = dino::LoadImageAsset("dinosaurs.bmp");

    std::array<Color, 1> whitePixel{Color_WHITE};
    m_pTextureWhite = jv::gpu::CreateTexture("Dino white", {1, 1}, whitePixel);

    m_players.emplace_back(Vec2{}, 0, jv::input::GamepadIdx::Keyboard, m_pTexturePlayer);
    m_players.emplace_back(Vec2{}, 1, jv::input::GamepadIdx::Gamepad1, m_pTexturePlayer);
    m_players.emplace_back(Vec2{}, 2, jv::input::GamepadIdx::Gamepad2, m_pTexturePlayer);
    m_players.emplace_back(Vec2{}, 3, jv::input::GamepadIdx::Gamepad3, m_pTexturePlayer);

    _BuildEntityList();
}

//Libère les textures utilisées par la scène.
dino::Scene::~Scene()
{
    jv::gpu::DestroyTexture(m_pTextureText);
    jv::gpu::DestroyTexture(m_pTextureAnimal);
    jv::gpu::DestroyTexture(m_pTexturePlayer);
    jv::gpu::DestroyTexture(m_pTextureWhite);
}


dino::Scene::~Scene()
{
    jv::gpu::DestroyTexture(m_pTexturePlayer);
    jv::gpu::DestroyTexture(m_pTextureAnimal);
}

void dino::Scene::Update(double absTime, float deltaTime)
{
    m_lastDeltaTime = deltaTime;
    m_lastAbsTime = absTime;

    _UpdateInput();

    if (m_state != State::Paused)
    {
        m_sceneTime += deltaTime;
        m_Terrain.Update(m_sceneTime, deltaTime);

        if (m_state == State::Playing)
        {
            m_gameElapsed += deltaTime;

            if (m_gameElapsed >= 60.0)
                _EndGame();
            else
                _UpdateGame(deltaTime);
        }
        else
        {
            _UpdateLobby(deltaTime);
        }
    }

    _BuildEntityList();
}

//commandes de connexion, de déconnexion et de pause.
void dino::Scene::_UpdateInput()
{
    bool pauseRequested = false;

    for (size_t i = 0; i < kGamepadIndices.size(); ++i)
    {
        m_gamepads[i] = {};
        m_gamepadAvailable[i] = jv::input::GetGamepad(kGamepadIndices[i], m_gamepads[i]);

        if (!m_gamepadAvailable[i])
            continue;

        bool startPressed = m_gamepads[i].start && !m_wasStartDown[i];
        bool selectPressed = m_gamepads[i].select && !m_wasSelectDown[i];

        if (m_state == State::Lobby)
        {
            if (startPressed && !m_players[i].IsConnected())
                m_players[i].Join(m_Terrain.GenerateRandomSpawn());

            if (selectPressed && m_players[i].IsConnected())
                m_players[i].Leave();
        }
        else if (m_players[i].IsConnected() && startPressed)
        {
            pauseRequested = true;
        }

        m_wasStartDown[i] = m_gamepads[i].start;
        m_wasSelectDown[i] = m_gamepads[i].select;
    }

    if (pauseRequested)
    {
        if (m_state == State::Playing)
            m_state = State::Paused;
        else if (m_state == State::Paused)
            m_state = State::Playing;
    }
}

// Met à jour le lobby et vérifie la sélection d'une saison.
void dino::Scene::_UpdateLobby(float deltaTime)
{
    for (Player& player : m_players)
    {
        if (!player.IsConnected())
            continue;

        player.Update(m_sceneTime, deltaTime);
        player.CheckTerrain(m_Terrain);
    }

    for (size_t i = 0; i < m_players.size(); ++i)
    {
        if (!m_players[i].IsConnected())
            continue;

        for (size_t j = i + 1; j < m_players.size(); ++j)
        {
            if (m_players[j].IsConnected())
                Entity::ResolveEntityCollision(m_players[i], m_players[j]);
        }
    }

    bool treesCanBeSelected = m_lastAbsTime >= m_treeSelectionEnabledAt;

    for (Player& player : m_players)
    {
        if (!player.IsConnected())
            continue;

        std::vector<Vec2> loop = player.TakeCompletedLoop();
        if (!treesCanBeSelected || loop.empty())
            continue;

        for (int32_t season = 0; season < 4; ++season)
        {
            if (IsInsidePolygon(m_Terrain.GetSeasonTreeCenter(season), loop))
            {
                _StartGame(season);
                return;
            }
        }
    }
}

void dino::Scene::_UpdateGame(float deltaTime)
{
    for (Player& player : m_players)
    {
        if (player.IsConnected())
        {
            player.Update(m_sceneTime, deltaTime);
            player.CheckTerrain(m_Terrain);
        }
    }

    _UpdateAnimals(deltaTime);

    std::vector<jv::gpu::Vertex> uiVertices;

    for (size_t i = 0; i < m_players.size(); ++i)
    {
        if (!m_players[i].IsConnected())
            continue;

        std::string scoreText = std::format("P{} {:05}", i + 1, m_players[i].GetScore());

        GenVertices_Text(uiVertices, scoreText, kPlayerColors[i], Color_BLACK,
            {4, 18.0f + 15.0f * static_cast<float>(i)});
    }

    for (Animal& animal : m_animals)
        animal.CheckTerrain(m_Terrain);

    for (size_t i = 0; i < m_animals.size(); ++i)
    {
        for (size_t j = i + 1; j < m_animals.size(); ++j)
            Entity::ResolveEntityCollision(m_animals[i], m_animals[j]);
    }

    for (Player& player : m_players)
    {
        if (!player.IsConnected())
            continue;

        for (Animal& animal : m_animals)
            Entity::ResolveEntityCollision(player, animal);
    }

    for (Player& player : m_players)
    {
        if (!player.IsConnected())
            continue;

        std::vector<Vec2> loop = player.TakeCompletedLoop();
        if (loop.empty())
            continue;

        for (size_t i = 0; i < m_animals.size();)
        {
            if (IsInsidePolygon(m_animals[i].GetPosition(), loop))
            {
                player.AddScore(10);
                m_animals.erase(m_animals.begin() + i);
            }
            else
            {
                ++i;
            }
        }
    }
}

// Gère l'apparition et la mise à jour des animaux.
void dino::Scene::_UpdateAnimals(float deltaTime)
{
    double timeRemaining = std::max(0.0, 60.0 - m_gameElapsed);
    double spawnInterval = 0.9 - 0.72 * (1.0 - timeRemaining / 60.0);
    spawnInterval = std::max(0.18, spawnInterval);

    if (m_gameElapsed - m_animalSpawnTime >= spawnInterval)
    {
        m_animalSpawnTime = m_gameElapsed;
        m_animals.emplace_back(m_Terrain.GenerateRandomSpawn(), m_gameElapsed, m_pTextureAnimal);
    }

    for (Animal& animal : m_animals)
        animal.Update(m_gameElapsed, deltaTime);
}

// prépare les entités à afficher selon leur position 
void dino::Scene::_BuildEntityList()
{
    m_entities.clear();

    for (Player& player : m_players)
    {
        if (player.IsConnected())
            m_entities.push_back(&player);
    }

    for (Animal& animal : m_animals)
        m_entities.push_back(&animal);

    std::sort(m_entities.begin(), m_entities.end(), Entity::OrderByPosY);
}

void dino::Scene::_StartGame(int32_t idxSeason)
{
    m_Terrain.SetSeason(idxSeason);
    m_animals.clear();
    m_gameElapsed = 0;
    m_animalSpawnTime = 0;
    m_state = State::Playing;

    for (Player& player : m_players)
        player.ClearLasso();
}

void dino::Scene::_EndGame()
{
    m_state = State::Lobby;
    m_animals.clear();
    m_treeSelectionEnabledAt = m_lastAbsTime + 5.0;

    for (Player& player : m_players)
        player.ClearLasso();
}

// Affiche le terrain, les entités et l'interface.
void dino::Scene::Draw() const
{
    m_Terrain.Draw();

    if (m_state == State::Lobby)
    {
        bool treesDimmed = m_lastAbsTime < m_treeSelectionEnabledAt;
        m_Terrain.DrawSeasonTrees(treesDimmed);
    }

    for (size_t i = 0; i < m_players.size(); ++i)
    {
        if (!m_players[i].IsConnected())
            continue;

        std::vector<jv::gpu::Vertex> lassoVertices;
        GenVertices_Polyline(lassoVertices, m_players[i].GetLassoPoints(), 2.0f, kPlayerColors[i]);

        if (!lassoVertices.empty())
        {
            jv::gpu::VertexBuffer* pVBuf =
                jv::gpu::CreateVertexBuffer("Player lasso", lassoVertices);
            jv::gpu::Draw(pVBuf, m_pTextureWhite);
            jv::gpu::DestroyVertexBuffer(pVBuf);
        }
    }

    for (Entity const* entity : m_entities)
        entity->Draw();

    std::vector<jv::gpu::Vertex> uiVertices;

    for (size_t i = 0; i < m_players.size(); ++i)
    {
        std::string scoreText = std::format("P{} {:05}", i + 1, m_players[i].GetScore());

        GenVertices_Text(uiVertices, scoreText, kPlayerColors[i], Color_BLACK,
            {4, 18.0f + 15.0f * static_cast<float>(i)});
    }

    if (m_state == State::Lobby)
    {
        GenVertices_Text(uiVertices, "LOBBY - START: REJOINDRE / SELECT: QUITTER", Color_WHITE,
            Color_BLACK, {54, 4});

        for (int32_t season = 0; season < 4; ++season)
        {
            std::string_view name = kSeasonNames[season];
            Vec2 center = m_Terrain.GetSeasonTreeCenter(season);
            Vec2 labelPos{center.x - static_cast<float>(name.size()) * 3, center.y + 18};

            GenVertices_Text(uiVertices, name, Color_WHITE, Color_BLACK, labelPos);
        }

        if (m_lastAbsTime < m_treeSelectionEnabledAt)
        {
            GenVertices_Text(
                uiVertices, "ARBRES INDISPONIBLES", Color_WHITE, Color_BLACK, {156, 330});
        }
        else
        {
            GenVertices_Text(uiVertices, "FAIRE UNE BOUCLE AUTOUR D'UN ARBRE", Color_WHITE,
                Color_BLACK, {108, 330});
        }
    }
    else
    {
        double timeRemaining = std::max(0.0, 60.0 - m_gameElapsed);
        std::string timerText = std::format("{:.1f}", timeRemaining);
        float timerX =
            (jv::gpu::GetRenderSize().x - static_cast<float>(timerText.size()) * 6) * 0.5f;

        GenVertices_Text(uiVertices, timerText, Color_WHITE, Color_BLACK, {timerX, 4});

        if (m_state == State::Paused)
        {
            GenVertices_Text(
                uiVertices, "PAUSE - START POUR REPRENDRE", Color_WHITE, Color_BLACK, {156, 170});
        }
    }

    if (!uiVertices.empty())
    {
        jv::gpu::VertexBuffer* pVBuf = jv::gpu::CreateVertexBuffer("Dino UI", uiVertices);
        jv::gpu::Draw(pVBuf, m_pTextureText);
        jv::gpu::DestroyVertexBuffer(pVBuf);
    }

    // Nombre de millisecondes nécessaires à l'affichage de la frame précédente.
    std::string text = std::format("dTime={:04.1f}ms", m_lastDeltaTime * 1000.0);
    std::vector<jv::gpu::Vertex> debugVertices;
    GenVertices_Text(debugVertices, text, Color_WHITE, Color_GREY);

    jv::gpu::VertexBuffer* pDebugBuffer = jv::gpu::CreateVertexBuffer("dTime", debugVertices);
    jv::gpu::Draw(pDebugBuffer, m_pTextureText);
    jv::gpu::DestroyVertexBuffer(pDebugBuffer);
}