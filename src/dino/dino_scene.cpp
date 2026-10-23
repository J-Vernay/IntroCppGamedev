
#include <algorithm>
#include <dino/dino_draw_utils.h>
#include <dino/dino_geometry.h>
#include <dino/dino_scene.h>
#include <format>
#include <iostream>
#include <string>

dino::Scene::Scene() : m_Terrain{24, 16}
{
    m_pTextureText = dino::LoadImageAsset("monogram-bitmap.bmp");
    m_pTextureAnimal = dino::LoadImageAsset("animals.bmp");
    m_pTextureTree = dino::LoadImageAsset("terrain.bmp");
    m_pTextureDinosaurs = dino::LoadImageAsset("dinosaurs.bmp");

    m_Terrain.SetSeason(jv::util::RandomInt32(0, 3));

    PlayerState state = PlayerState::Lobby;

    m_players.emplace_back(m_Terrain.GenerateRandomSpawn(), m_pTextureDinosaurs, 0,
        jv::input::GamepadIdx::Keyboard, Color_BLUE, state);
    m_players.emplace_back(m_Terrain.GenerateRandomSpawn(), m_pTextureDinosaurs, 1,
        jv::input::GamepadIdx::Gamepad1, Color_RED, state);
    m_players.emplace_back(m_Terrain.GenerateRandomSpawn(), m_pTextureDinosaurs, 2,
        jv::input::GamepadIdx::Gamepad2, Color_YELLOW, state);
    m_players.emplace_back(m_Terrain.GenerateRandomSpawn(), m_pTextureDinosaurs, 3,
        jv::input::GamepadIdx::Gamepad3, Color_GREEN, state);

    SpawnTree();
}

void dino::Scene::SpawnTree()
{
    for (size_t i = 0; i < 4; i++)
    {
        m_trees.emplace_back(
            dino_tree(Vec2(m_treeSpawn.x + i * 40, m_treeSpawn.y), m_pTextureTree, i, m_lastAbsTime));
    }
}

//liberer la memoire
dino::Scene::~Scene()
{
    jv::gpu::DestroyTexture(m_pTextureAnimal);
    jv::gpu::DestroyTexture(m_pTextureDinosaurs);
    jv::gpu::DestroyTexture(m_pTextureText);
    jv::gpu::DestroyTexture(m_pTextureTree);
}

void dino::Scene::Update(double pAbsTime, float pDeltaTime)
{
    m_lastAbsTime = pAbsTime;
    m_lastDeltaTime = pDeltaTime;
    m_movable.clear();

    _UpdatePlayer(pAbsTime, pDeltaTime);

    if (m_currentState == PlayerState::Lobby && m_trees.size() > 0)
    {
        for (dino_tree& tree : m_trees)
        {
            m_movable.push_back(&tree);
            tree.Update(pAbsTime, pDeltaTime);
        }
    }
    else
    {
        for (Animal& animal : m_animals)
        {
            m_movable.push_back(&animal);
        }
    }

    //doit checker loop apres avoir ajouter abres et animal pour que sa puisse les detecter
    for (Player& player : m_players)
    {
        CheckLoop(player);
    }

    if (m_isPaused || m_currentState == PlayerState::Lobby)
        return;

    m_Terrain.Update(pAbsTime, pDeltaTime);

    _UpdateAnimals(pAbsTime, pDeltaTime);

    //eviter des pointeur residut nul si supprimer dans la loop ou autre
    m_movable.clear();
    for (Player& p : m_players)
        if (p.m_isSpawned)
            m_movable.push_back(&p);
    for (Animal& a : m_animals)
        m_movable.push_back(&a);

    std::sort(m_movable.begin(), m_movable.end(), Movable::OrderByPosY);

    m_timer = std::clamp(m_timer - pDeltaTime, 0.0f, 60.0f);

    //si parit terminer revenir au lobby
    if (m_timer == 0)
    {
        m_movable.clear();
        m_animals.clear();
        SpawnTree();
        m_currentState = PlayerState::Lobby;
        for (Player& p : m_players)
        {
            p.ChangeState(PlayerState::Lobby);
        }
    }
}

void dino::Scene::_UpdatePlayer(double pAbsTime, float pDeltaTime)
{
    for (size_t i = 0; i < m_players.size(); i++)
    {
        Player& player = m_players[i];

        m_movable.push_back(&player);

        if (player.IsPause())
        {
            m_isPaused = !m_isPaused;
        }

        if (m_isPaused)
            return;

        player.Update(pAbsTime, pDeltaTime);

        if (!player.m_isSpawned)
            continue;

        player.CheckTerrain(m_Terrain);

        for (int j = i + 1; j < m_players.size(); ++j)
            Movable::ResolveCollision(player, m_players[j]);

        for (int j = 0; j < m_animals.size(); ++j)
            Movable::ResolveCollision(player, m_animals[j]);

        for (int j = 0; j < m_trees.size(); ++j)
            Movable::ResolveCollision(player, m_trees[j]);

        if (player.m_points.size() < 2)
            continue;

        CheckPlayerIntersect(player);
        
    }
}

void dino::Scene::CheckLoop(Player& pPlayer)
{
    Vec2 vecZero = Vec2(0, 0);

    std::vector<dino::Vec2> temp_point = pPlayer.CheckIfLoop();
    int size = temp_point.size();
    if (temp_point.size() > 0)
    {
        //parcourir a l'envers car m_movable peut changer de taille
        for (size_t i = m_movable.size(); i > 0; i--)
        {
            int counter = 0;
            for (size_t y = 0; y < temp_point.size() - 1; y++)
            {
                if (dino::IntersectSegment(temp_point[y], temp_point[(y + 1) % temp_point.size()],
                        m_movable[i - 1]->GetPos(), vecZero))
                {
                    ++counter;
                }
            }

            if (counter % 2 == 0)
            {
                continue;
            }

            Movable* target = m_movable[i - 1];

            ///verifier qu'il ne se detecte pas lui meme
            if (target == &pPlayer)
                continue;

            ///verifie si il a capturer un animal que si n'est pas dans le lobby
            if (m_currentState != PlayerState::Lobby)
            {
                for (size_t k = 0; k < m_animals.size(); ++k)
                {
                    if (&m_animals[k] == target)
                    {
                        pPlayer.m_score += 10;
                        m_animals.erase(m_animals.begin() + k);
                        break;
                    }
                }
            }
            else
            {
                for (dino_tree& tree : m_trees)
                {
                    if (target == &tree && tree.IsActive())
                    {
                        m_Terrain.SetSeason(tree.m_index);
                        m_timer = 60;
                        m_currentState = PlayerState::InGame;
                        break;
                    }
                }
                ///change le state de touts les player en in game si on vient d'entourer un animal
                if (m_currentState == PlayerState::InGame)
                {
                    for (Player& playerCop : m_players)
                    {
                        playerCop.ChangeState(m_currentState);
                        playerCop.m_score = 0;
                    }
                    m_trees.clear();
                    return;
                }
            }

            for (Player& playerCop : m_players)
            {

                if (target == &playerCop)
                {
                    playerCop.TakeDamage();
                }
            }

            std::erase(m_movable, target);
        }
    }
}

void dino::Scene::CheckPlayerIntersect(Player& pPlayer)
{
    /// Efface les segments entre joueur s'ils se coupent
    Vec2 last = pPlayer.m_points.back();
    Vec2 lastToLast = pPlayer.m_points[pPlayer.m_points.size() - 2];

    for (size_t y = 0; y < m_players.size(); y++)
    {
        if (&m_players[y] == &pPlayer)
            continue;

        auto& pts = m_players[y].m_points;

        for (size_t x = 0; x + 1 < pts.size(); x++)
        {
            if (dino::IntersectSegment(lastToLast, last, pts[x], pts[x + 1]))
            {
                pts.erase(pts.begin(), pts.begin() + x + 1);
                break;
            }
        }
    }
}

void dino::Scene::_UpdateAnimals(double pAbsTime, float pDeltaTime)
{

    constexpr double kMaxSpawnInterval = 3.0;
    constexpr double kMinSpawnInterval = 0.3;

    double progress = 1.0 - std::clamp(m_timer / 60.0, 0.0, 1.0);

    double spawnInterval = kMaxSpawnInterval + (kMinSpawnInterval - kMaxSpawnInterval) * progress;

    //spawner les animeaux a intervalles precalculer
    if (pAbsTime - m_animalSpawnTime >= spawnInterval)
    {
        m_animalSpawnTime = pAbsTime;
        Vec2 spawnPos = m_Terrain.GenerateRandomSpawn();
        m_animals.emplace_back(spawnPos, pAbsTime, m_pTextureAnimal);
    }

    for (Animal& animal : m_animals)
    {
        animal.Update(pAbsTime, pDeltaTime);
        animal.CheckTerrain(m_Terrain);
    }

    for (int i = 0; i < m_animals.size(); ++i)
        for (int j = i + 1; j < m_animals.size(); ++j)
            Movable::ResolveCollision(m_animals[i], m_animals[j]);
}

void dino::Scene::Draw() const
{
    m_Terrain.Draw();

    // Convertit le timer en string en arrondissant
    std::string timeStr = std::to_string(std::lround(m_timer));
    std::string_view time = timeStr;

    // creer le visuel du timer
    std::vector<jv::gpu::Vertex> vsTimer;
    dino::GenVertices_Text(
        vsTimer, time, Color_BLACK, Color_TRANSPARENT, Vec2(m_whereTimer.x, m_whereTimer.y));
    jv::gpu::VertexBuffer* pVBufTimer = jv::gpu::CreateVertexBuffer("Timer", vsTimer);
    jv::gpu::Draw(pVBufTimer, m_pTextureText);
    jv::gpu::DestroyVertexBuffer(pVBufTimer);

    DrawScore();
    
    for (Movable const* entity : m_movable)
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

void dino::Scene::DrawScore() const
{
    // score du player
    for (size_t i = 0; i < m_players.size(); ++i)
    {
        Player const& player = m_players[i];

        if (player.m_isSpawned)
        {
            std::string scoreStr = std::to_string(std::lround(player.m_score));
            std::string_view score = scoreStr;

            std::vector<jv::gpu::Vertex> vsScore;
            dino::GenVertices_Text(vsScore, score, m_colors[i], Color_TRANSPARENT,
                Vec2(m_whereScore.x, m_whereScore.y + (10 * i)));
            jv::gpu::VertexBuffer* pVBufTimer = jv::gpu::CreateVertexBuffer("score", vsScore);
            jv::gpu::Draw(pVBufTimer, m_pTextureText);
            jv::gpu::DestroyVertexBuffer(pVBufTimer);
        }
    }
}
