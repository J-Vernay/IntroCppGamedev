
#include <dino/dino_draw_utils.h>
#include <dino/dino_scene.h>
#include <dino/dino_geometry.h>
#include <dino/dino_assets.h>
#include <format>
#include <algorithm>

dino::Scene::Scene() : m_Terrain{24, 16}
{
    m_Terrain.SetSeason(jv::util::RandomInt32(0, 3));
    StartLobby(-5);
}

dino::Scene::~Scene() {}

void dino::Scene::StartGame(int32_t season)
{
    // remove all animals
    for (int i = 0; i < m_entities.size(); i++)
    {
        Animal* ptr = dynamic_cast<Animal*>(m_entities[i]);
        if (ptr == nullptr)
            continue;

        m_entities.erase(m_entities.begin() + i);
        delete ptr;
        i--;
    }

    m_game = true;
    m_pause = false;
    m_timer = g_gameDuration;
    m_Terrain.SetSeason(season);

    // remove all trees
    for (int i = 0; i < m_entities.size(); i++)
    {
        Tree* ptr = dynamic_cast<Tree*>(m_entities[i]);
        if (ptr == nullptr)
            continue;

        m_entities.erase(m_entities.begin() + i);
        delete ptr;
        i--;
    }
}

void dino::Scene::StartLobby(double absTime)
{
    // remove all animals
    for (int i = 0; i < m_entities.size(); i++)
    {
        Animal* ptr = dynamic_cast<Animal*>(m_entities[i]);
        if (ptr == nullptr)
            continue;

        m_entities.erase(m_entities.begin() + i);
        delete ptr;
        i--;
    }

    m_game = false;
    m_pause = false;
    m_timer = 0.0f;
    // Spawn all trees
    for (int i = 0; i < 4; i++)
    {
        Tree* treePtr = (new dino::Tree(absTime, i));
        Tree& tree = *treePtr;
        tree.m_pos = {
            i * (jv::gpu::GetRenderSize().x / 6) + jv::gpu::GetRenderSize().x / 4, 
            jv::gpu::GetRenderSize().y / 2.5f
        };
        m_entities.push_back(treePtr);
    }
}

void dino::Scene::SetPause()
{
    m_pause = true;
    m_pauseHandler.m_selectedOption = 0;
}

void dino::Scene::_PreGameLogic()
{
    // Handle player connection/disconnection
    jv::input::GamepadIdx gamePads[4] = {
        jv::input::GamepadIdx::Keyboard,
        jv::input::GamepadIdx::Gamepad1, 
        jv::input::GamepadIdx::Gamepad2,
        jv::input::GamepadIdx::Gamepad3
    };
    for (char i = 0; i < 4; i++)
    {
        jv::input::GamepadIdx idx = gamePads[i];
        bool found = false;

        for (Entity* entityPtr : m_entities)
        {
            Player* playerPtr = dynamic_cast<Player*>(entityPtr);
            if (playerPtr == nullptr)
                continue;
            if ((*playerPtr).m_gamepadIdx == idx)
                found = true;
        }
        if (found)
            continue;

        jv::input::Gamepad gamepad;
        if (jv::input::GetGamepad(idx, gamepad))
        {
            if (gamepad.start)
            {
                Vec2 spawnPos = m_Terrain.GenerateRandomSpawn();
                dino::Player* player = new dino::Player(idx, spawnPos, i);

                m_entities.push_back(player);
            }
        }
    }
}

void dino::Scene::Update(double absTime, float deltaTime)
{
    if (m_pause)
        deltaTime = 0.0f;
    m_lastDeltaTime = deltaTime;

    // Handle game timer
    if (m_game && !m_pause)
    {
        m_timer -= deltaTime;
        if (m_timer <= 0) // Game end
        {
            StartLobby(absTime);
        }
    }
    // Handle lobby logic (especially non-connected player input)
    if (!m_game)
        _PreGameLogic();

    m_Terrain.Update(absTime, deltaTime);

    _UpdateEntities(absTime, deltaTime);
    _HandleCollisions();
    _HandlePlayersLasso();
    m_pauseHandler.Update(*this, absTime, deltaTime);
}

void dino::Scene::_UpdateEntities(double absTime, float deltaTime)
{
    // Spawner un animal si besoin.
    if (m_game && !m_pause)
    {
        double kSpawnTime = (m_timer / g_gameDuration) + 0.05f;
        if (absTime - m_animalSpawnTime >= kSpawnTime)
        {
            m_animalSpawnTime = absTime;
            Vec2 spawnPos = m_Terrain.GenerateRandomSpawn();
            dino::Animal* animal = new dino::Animal(spawnPos, absTime);
            m_entities.push_back(animal);
        }
    }

    for (Entity* entityPtr : m_entities)
    {
        Entity& entity = *entityPtr;
        entity.Update(*this, absTime, deltaTime);
    }
}

void dino::Scene::_HandleCollisions()
{
    // Check every pairs for collisions
    for (int i = 0; i < m_entities.size(); i++)
    {
        for (int j = i + 1; j < m_entities.size(); j++)
        {
            Entity* entityPtr1 = m_entities[i];
            Entity* entityPtr2 = m_entities[j];

            Entity& entity1 = *entityPtr1;
            Entity& entity2 = *entityPtr2;

            float dist = dino::math::Distance(entity1.m_pos, entity2.m_pos);
            float radiusTotal = entity1.m_collisionRadius + entity2.m_collisionRadius;

            // The 2 entities are too close (closer than combined collision radius) and not perfectly overlapping
            if (dist < radiusTotal && dist > FLT_EPSILON)
            {
                // Get displacement direction
                jv::util::Vec2 displacement = dino::math::NormalizeVector(dino::math::VectorSubtract(entity1.m_pos, entity2.m_pos));
                // Get Displacement amount
                float delta = radiusTotal - dist;
                entity1.m_pos = {entity1.m_pos.x + displacement.x * delta, entity1.m_pos.y + displacement.y * delta};
                entity2.m_pos = {entity2.m_pos.x - displacement.x * delta, entity2.m_pos.y - displacement.y * delta};
            }
            
        }
    }
}

void dino::Scene::_HandlePlayersLasso() {
    // Get only players
    std::vector<Player*> players;
    for (Entity* entityPtr : m_entities)
    {
        Player* playerPtr = dynamic_cast<Player*>(entityPtr);
        if (playerPtr == nullptr)
            continue;
        players.push_back(playerPtr);
    }
    // Check every player pairs
    for (auto&& playerPtr1 : players)
    {
        for (auto&& playerPtr2 : players)
        {   
            Player& player1 = *playerPtr1;
            Player& player2 = *playerPtr2;

            if (player1.m_lassoPoints.size() < 2)
                break;
            // Only the end of the lasso can intersect (does not make sense to check a segment that didn't change this frame, only the last segment changed)
            int p1LassoIdx = player1.m_lassoPoints.size() - 2;

            // Check every segment pairs
            for (int p2LassoIdx = 0; p2LassoIdx < player2.m_lassoPoints.size() - 1; p2LassoIdx++)
            {
                if (player2.m_lassoPoints.size() == 0)
                    break;

                // Intersection
                if (IntersectSegment(player1.m_lassoPoints[p1LassoIdx], player1.m_lassoPoints[p1LassoIdx + 1], 
                    player2.m_lassoPoints[p2LassoIdx], player2.m_lassoPoints[p2LassoIdx + 1]))
                {
                    if (&player1 == &player2) // Self-Loop
                    {
                        // Does not intersect with itself
                        if (p2LassoIdx >= p1LassoIdx - 1)
                            continue;

                        // Check and hit every entity in the lasso's loop
                        for (Entity* entityPtr : m_entities)
                        {
                            if (entityPtr == playerPtr1)
                                continue;

                            Entity& entity = *entityPtr;
                            std::vector<Vec2> lassoLoopVerticies = {player1.m_lassoPoints.begin() + p2LassoIdx, player1.m_lassoPoints.end()};
                            if (PointInPolygon(entity.m_pos, lassoLoopVerticies))
                            {
                                entity.OnLassoHit(player1, *this);
                            }
                        }
                        
                        // Remove every points between the player and intersection point
                        for (int i = p2LassoIdx; i < player1.m_lassoPoints.size();)
                        {
                            player1.m_lassoPoints.erase(player1.m_lassoPoints.begin() + p2LassoIdx);
                            player1.m_lassoPointsSpawnTime.erase(player1.m_lassoPointsSpawnTime.begin() + p2LassoIdx);
                        }
                        break;
                    }
                    else // Player 1 is over Player 2
                    {
                        // Remove player 2 beginning
                        for (int i = 0; i < p2LassoIdx; i++)
                        {
                            player2.m_lassoPoints.erase(player2.m_lassoPoints.begin());
                            player2.m_lassoPointsSpawnTime.erase(player2.m_lassoPointsSpawnTime.begin());
                        }
                    }
                }
            }
        }
    }
}

bool _SortByY(dino::Entity* aPtr, dino::Entity* bPtr)
{
    dino::Entity& a = *aPtr;
    dino::Entity& b = *bPtr;
    return a.m_pos.y < b.m_pos.y;
}

void dino::Scene::Draw() const
{
    m_Terrain.Draw();

    // Sort entities by height (Y)
    std::vector<Entity*> sortedEntities(m_entities.size());
    std::partial_sort_copy(m_entities.begin(), m_entities.end(), sortedEntities.begin(), sortedEntities.end(), _SortByY);

    for (Entity const* entityPtr : sortedEntities)
    {
        Entity const& entity = *entityPtr;
        entity.Draw();
    }

    // Nombre de millisecondes qu'il a fallu pour afficher la frame précédente.
    {
        std::string text = std::format("dTime={:04.1f}ms", m_lastDeltaTime * 1000.0);
        std::vector<jv::gpu::Vertex> vs;
        dino::GenVertices_Text(vs, text, Color_WHITE, Color_GREY);
        jv::gpu::VertexBuffer* pVBuf = jv::gpu::CreateVertexBuffer("dTime", vs);
        jv::gpu::Draw(pVBuf, (&dino::AssetsHolder::getInstance())->g_Textures["text"]);
        jv::gpu::DestroyVertexBuffer(pVBuf);
    }
    // Chronomètre de la partie
    if (m_game)
    {
        std::string text = std::format("{:04.1f}sec", m_timer);
        std::vector<jv::gpu::Vertex> vs;
        dino::GenVertices_Text(vs, text, Color_WHITE, Color_GREY, {jv::gpu::GetRenderSize().x / 2 - 20, 0});
        jv::gpu::VertexBuffer* pVBuf = jv::gpu::CreateVertexBuffer("chronoText", vs);
        jv::gpu::Draw(pVBuf, (&dino::AssetsHolder::getInstance())->g_Textures["text"]);
        jv::gpu::DestroyVertexBuffer(pVBuf);
    }
    // Pause menu
    if (m_pause)
    {
        m_pauseHandler.Draw(m_timer);
    }
}

dino::Terrain const& dino::Scene::GetTerrain() const
{
    return m_Terrain;
}

void dino::Scene::RemoveEntity(Entity* entity) 
{
    for (size_t i = 0; i < m_entities.size(); i++)
        if (m_entities[i] == entity)
        {
            m_entities.erase(m_entities.begin() + i);
            break;
        }
}