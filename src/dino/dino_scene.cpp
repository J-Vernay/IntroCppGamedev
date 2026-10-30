
#include <dino/dino_draw_utils.h>
#include <dino/dino_scene.h>
#include <dino/dino_entity.h>
#include <dino/dino_geometry.h>
#include <algorithm>

#include <format>

dino::Scene::Scene() : m_Terrain{24, 16}
{
    m_pTextureText = dino::LoadImageAsset("monogram-bitmap.bmp");
    m_pTextureAnimal = dino::LoadImageAsset("animals.bmp");
    m_pTexturePlayer = dino::LoadImageAsset("dinosaurs.bmp");

    m_Terrain.SetSeason(0);

    m_players.emplace_back(m_Terrain.GenerateRandomSpawn(), 0, jv::input::GamepadIdx::Keyboard, m_pTexturePlayer);
    m_players.emplace_back(m_Terrain.GenerateRandomSpawn(), 1, jv::input::GamepadIdx::Gamepad1, m_pTexturePlayer);
    m_players.emplace_back(m_Terrain.GenerateRandomSpawn(), 2, jv::input::GamepadIdx::Gamepad2, m_pTexturePlayer);
    m_players.emplace_back(m_Terrain.GenerateRandomSpawn(), 3, jv::input::GamepadIdx::Gamepad3, m_pTexturePlayer);

    m_players[0].SetActive(true);
}

dino::Scene::~Scene()
{
    jv::gpu::DestroyTexture(m_pTexturePlayer);
    jv::gpu::DestroyTexture(m_pTextureAnimal);
}

bool IsPointInsidePolygon(dino::Vec2 pt, std::vector<dino::Vec2> const& poly) // tester si un point est dans un polygone
{
    bool inside = false;
    size_t n = poly.size();
    for (size_t i = 0, j = n - 1; i < n; j = i++)
    {
        if (((poly[i].y > pt.y) != (poly[j].y > pt.y)) &&
            (pt.x <
                (poly[j].x - poly[i].x) * (pt.y - poly[i].y) / (poly[j].y - poly[i].y) + poly[i].x))
        {
            inside = !inside;
        }
    }
    return inside;
}

void dino::Scene::Update(double absTime, float deltaTime)
{

    m_lastDeltaTime = deltaTime;

    for (auto it = m_notifications.begin(); it != m_notifications.end();) // gérer les notifications de score 
    {
        it->lifetime -= deltaTime;
        it->pos.y -= 15.0f * deltaTime;
        if (it->lifetime <= 0.0f)
            it = m_notifications.erase(it);
        else
            ++it;
    }

    if (m_GameState == GameState::Lobby) // lobby
    {
        if (m_lobbyCooldown > 0)
        {
            m_lobbyCooldown -= deltaTime;
            if (m_lobbyCooldown < 0)
                m_lobbyCooldown = 0;
        }

        jv::input::GamepadIdx indices[] = {jv::input::GamepadIdx::Keyboard,
            jv::input::GamepadIdx::Gamepad1, jv::input::GamepadIdx::Gamepad2,
            jv::input::GamepadIdx::Gamepad3};

        for (int i = 0; i < 4; ++i)
        {
            jv::input::Gamepad gp;
            if (jv::input::GetGamepad(indices[i], gp))
            {
                if (gp.start)
                    m_players[i].SetActive(true);
                if (gp.select)
                    m_players[i].SetActive(false);
            }
        }
        for (Player& player : m_players)
        {
            if (player.IsActive())
            {
                player.Update(absTime, deltaTime);
                player.CheckTerrain(m_Terrain);
            }
        }
        Vec2 rdrSize = jv::gpu::GetRenderSize(); // arbres a entourer
        Vec2 treeCenters[4] = {
            {rdrSize.x * 0.25f, rdrSize.y * 0.3f},
            {rdrSize.x * 0.75f, rdrSize.y * 0.3f},
            {rdrSize.x * 0.25f, rdrSize.y * 0.7f},
            {rdrSize.x * 0.75f, rdrSize.y * 0.7f}
        };

        for (Player& player : m_players)
        {
            if (!player.IsActive())
                continue;

            auto const& trail = player.GetTrail();
            if (trail.size() > 5)
            {
                Vec2 A = trail[trail.size() - 2].pos;
                Vec2 B = trail.back().pos;

                for (size_t i = 0; i < trail.size() - 3; ++i)
                {
                    Vec2 C = trail[i].pos;
                    Vec2 D = trail[i + 1].pos;
                    if (dino::IntersectSegment(A, B, C, D))
                    {
                        std::vector<Vec2> closedPolygon;
                        for (size_t k = i; k < trail.size(); ++k)
                            closedPolygon.push_back(trail[k].pos);
                        if (m_lobbyCooldown <= 0)
                        {
                            for (int s = 0; s < 4; ++s)
                            {
                                if (IsPointInsidePolygon(treeCenters[s], closedPolygon))
                                {
                                    m_Terrain.SetSeason(s);
                                    m_GameState = GameState::Playing;
                                    m_timeLeft = 60.0;
                                    m_animals.clear();
                                    for (auto& p : m_players)
                                        p.ClearTrail();
                                    break;
                                }
                            }
                            if (m_GameState == GameState::Playing)
                                break;
                        }
                        std::deque<TrailPoint> newTrail(trail.begin(), trail.begin() + i + 1);
                        player.SetTrail(newTrail);
                        break;
                    }
                }
            }
        }
        m_entities.resize(0);
        for (Player& player : m_players)
        {
            if (player.IsActive())
                m_entities.push_back(&player);
        }
        std::sort(m_entities.begin(), m_entities.end(), Entity::OrderByPosY);
        return;
    }

    bool startPressed = false;
    jv::input::Gamepad gp;
    jv::input::GamepadIdx indices[] = {jv::input::GamepadIdx::Keyboard,
        jv::input::GamepadIdx::Gamepad1, jv::input::GamepadIdx::Gamepad2,
        jv::input::GamepadIdx::Gamepad3};

    for (auto idx : indices) // verifie tous les boutons start de toutes les manettes 
    {
        if (jv::input::GetGamepad(idx, gp))
        {
            if (gp.start)
            {
                startPressed = true;
                break;
            }
        }
    }

    if (startPressed && !m_bWasStartPressed)
    {
        m_bPaused = !m_bPaused;
    }
    m_bWasStartPressed = startPressed;

    if (m_bPaused) // stopper le jeu
    {
        return;
    }

    
    m_timeLeft -= deltaTime;// update du temps restant 

   if (m_timeLeft <= 0) // verifie si la partie est finie 
   {
        m_timeLeft = 0;
        m_GameState = GameState::Lobby;
        m_lobbyCooldown = 5.0;
        m_bPaused = false;
        m_animals.clear();

        for (auto& p : m_players)
        {
            p.ClearTrail();
        }
        m_entities.resize(0);
        for (Player& player : m_players)
        {
            if (player.IsActive())
                m_entities.push_back(&player);
        }
        std::sort(m_entities.begin(), m_entities.end(), Entity::OrderByPosY);

        return;
    }

    m_Terrain.Update(absTime, deltaTime);

    _UpdateAnimals(absTime, deltaTime);

    for (Player& player : m_players)
        player.Update(absTime, deltaTime);

    
    for (int i = 0; i < m_players.size(); ++i)
        for (int j = i + 1; j < m_players.size(); ++j)
            if (m_players[i].IsActive() && m_players[j].IsActive())
                Entity::ResolveCollision(m_players[i], m_players[j]);
        
            

    for (int i = 0; i < m_animals.size(); ++i)
        for (int j = i + 1; j < m_animals.size(); ++j)
            Entity::ResolveCollision(m_animals[i], m_animals[j]);

    for (int i = 0; i < m_players.size(); ++i)
        for (int j = 0; j < m_animals.size(); ++j)
            Entity::ResolveCollision(m_players[i], m_animals[j]);

    for (Player& player : m_players)
        player.CheckTerrain(m_Terrain);

    for (Animal& animal : m_animals)
        animal.CheckTerrain(m_Terrain);

    for (Player& player : m_players)
    {
        auto const& trail = player.GetTrail();
        if (trail.size() > 5)
        {
            Vec2 A = trail[trail.size() - 2].pos;
            Vec2 B = trail.back().pos;

            for (size_t i = 0; i < trail.size() - 3; ++i)
            {
                Vec2 C = trail[i].pos;
                Vec2 D = trail[i + 1].pos;

                if (dino::IntersectSegment(A, B, C, D))
                {
                    std::vector<Vec2> closedPolygon;
                    for (size_t k = i; k < trail.size(); ++k)
                    {
                        closedPolygon.push_back(trail[k].pos);
                    }

                    auto it = m_animals.begin();
                    while (it != m_animals.end())
                    {
                        if (IsPointInsidePolygon(it->GetPos(), closedPolygon)) // un animal est capturé
                        {
                            Vec2 animalPos = it->GetPos();
                            player.AddScore(10);
                            m_notifications.push_back({animalPos, "+10", player.GetColor(), 1.0f});
                            it = m_animals.erase(it);
                            
                        }
                        else
                        {
                            ++it;
                        }
                    }

                    std::deque<TrailPoint> newTrail(trail.begin(), trail.begin() + i + 1);
                    player.SetTrail(newTrail);
                    break;
                }
            }
        }
    }

    m_entities.resize(0);
    for (Player& player : m_players)
    {
        if (player.IsActive())
            m_entities.push_back(&player);
    }
    for (Animal& animal : m_animals)
        m_entities.push_back(&animal);

    std::sort(m_entities.begin(), m_entities.end(), Entity::OrderByPosY);


}

void dino::Scene::_UpdateAnimals(double absTime, float deltaTime)
{
    // Spawner un animal si besoin.

    double kSpawnTime = 0.1 + (m_timeLeft / 60.0) * 0.4; // changement du kSpawnTime afin de prendre en compte le timer (plus coonstexpr car au runtime)
    if (absTime - m_animalSpawnTime >= kSpawnTime)
    {
        m_animalSpawnTime = absTime;
        Vec2 spawnPos = m_Terrain.GenerateRandomSpawn();
        m_animals.emplace_back(spawnPos, absTime, m_pTextureAnimal);
    }

    for (Animal& animal : m_animals)
        animal.Update(absTime, deltaTime);
}

void dino::Scene::Draw() const
{
    m_Terrain.Draw();

    for (Player const& player : m_players) // trails
    {
        auto const& trail = player.GetTrail();
        if (trail.size() >= 2)
        {
            std::vector<Vec2> points;
            points.reserve(trail.size() + 1);
            for (auto const& tp : trail)
            {
                points.push_back(tp.pos);
            }
            points.push_back(player.GetPos());
            std::vector<jv::gpu::Vertex> trailVertices;
            dino::GenVertices_Polyline(trailVertices, points, 6.0f, player.GetColor());

            jv::gpu::VertexBuffer* pTrailVBuf =
                jv::gpu::CreateVertexBuffer("PlayerTrail", trailVertices);
            jv::gpu::Draw(pTrailVBuf, nullptr);
            jv::gpu::DestroyVertexBuffer(pTrailVBuf);
        }
    }

    if (m_GameState == GameState::Lobby) // arbres du lobby
    {
        Vec2 rdrSize = jv::gpu::GetRenderSize();
        Vec2 treePositions[4] = {
            {rdrSize.x * 0.25f - 16.0f, rdrSize.y * 0.3f - 24.0f},
            {rdrSize.x * 0.75f - 16.0f, rdrSize.y * 0.3f - 24.0f},
            {rdrSize.x * 0.25f - 16.0f, rdrSize.y * 0.7f - 24.0f},
            {rdrSize.x * 0.75f - 16.0f, rdrSize.y * 0.7f - 24.0f}};

        jv::util::Color treeColor = (m_lobbyCooldown > 0) ? Color_TRANSPARENT : Color_WHITE;

        for (int i = 0; i < 4; ++i)
        {
            std::vector<jv::gpu::Vertex> treeVs;
            Vec2 treeTopLeftUV = {48.0f + float(80 * i), 16.0f};
            dino::GenVertices_Rect(
                treeVs, treePositions[i], {32.0f, 48.0f}, treeTopLeftUV, treeColor);

            jv::gpu::VertexBuffer* pVBuf = jv::gpu::CreateVertexBuffer("SeasonTree", treeVs);
            jv::gpu::Draw(pVBuf, m_Terrain.GetTexture());
            jv::gpu::DestroyVertexBuffer(pVBuf);
        }
    }

    Vec2 rdrSize = jv::gpu::GetRenderSize();
    for (size_t i = 0; i < m_players.size(); ++i) // score
    {
        std::string scoreText = std::format("P{}: {} pts", i + 1, m_players[i].GetScore());
        if (m_GameState == GameState::Lobby && !m_players[i].IsActive())
        {
            scoreText = std::format("P{}: OFF", i + 1);
        }
        float textWidth = scoreText.size() * 6.0f;
        Vec2 scorePos = {rdrSize.x - textWidth - 20.0f, 40.0f + (i * 20.0f)};
        std::vector<jv::gpu::Vertex> vs;
        dino::GenVertices_Text(vs, scoreText, m_players[i].GetColor(), Color_BLACK, scorePos);
        jv::gpu::VertexBuffer* pVBuf =
            jv::gpu::CreateVertexBuffer(std::format("ScoreP{}", i).c_str(), vs);
        jv::gpu::Draw(pVBuf, m_pTextureText);
        jv::gpu::DestroyVertexBuffer(pVBuf);
    }

    for (auto const& notif : m_notifications) // notifications
    {
        std::vector<jv::gpu::Vertex> vs;
        float scale = 1.5f;
        dino::GenVertices_Text(vs, notif.text, notif.color, Color_INVISIBLE, notif.pos);
        for (auto& v : vs)
        {
            v.pos.x = notif.pos.x + (v.pos.x - notif.pos.x) * scale;
            v.pos.y = notif.pos.y + (v.pos.y - notif.pos.y) * scale;
        }
        jv::gpu::VertexBuffer* pVBuf = jv::gpu::CreateVertexBuffer("FloatingText", vs);
        jv::gpu::Draw(pVBuf, m_pTextureText);
        jv::gpu::DestroyVertexBuffer(pVBuf);
    }

    for (Entity const* entity : m_entities)
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
    if (m_GameState == GameState::Lobby) // lobby cooldown
    {
        std::string text =
            m_lobbyCooldown > 0 ? "LOBBY - WAIT 5s" : "LOBBY - ENCLOSE A TREE TO START";
        Vec2 rdrSize = jv::gpu::GetRenderSize();
        Vec2 textPos = {(rdrSize.x - text.size() * 6.0f) * 0.5f, 20.0f};
        std::vector<jv::gpu::Vertex> vs;
        dino::GenVertices_Text(vs, text, Color_WHITE, Color_GREY, textPos);
        jv::gpu::VertexBuffer* pVBuf = jv::gpu::CreateVertexBuffer("LobbyText", vs);
        jv::gpu::Draw(pVBuf, m_pTextureText);
        jv::gpu::DestroyVertexBuffer(pVBuf);
    }
    else
    {
        // timer
        std::string text = std::format("Time: {:02.0f}s", m_timeLeft);
        Vec2 rdrSize = jv::gpu::GetRenderSize();
        float textWidth = text.size() * 6.0f;
        Vec2 textPos = {(rdrSize.x - textWidth) * 0.5f, 10.0f};
        std::vector<jv::gpu::Vertex> vs;
        dino::GenVertices_Text(vs, text, Color_WHITE, Color_GREY, textPos);
        jv::gpu::VertexBuffer* pVBuf = jv::gpu::CreateVertexBuffer("Timer", vs);
        jv::gpu::Draw(pVBuf, m_pTextureText);
        jv::gpu::DestroyVertexBuffer(pVBuf);
    }
    
    if (m_bPaused) // petit indicateur de pause
    {
        std::string text = " PAUSE ";
        Vec2 rdrSize = jv::gpu::GetRenderSize();
        float textWidth = text.size() * 6.0f;
        Vec2 textPos = {(rdrSize.x - textWidth) * 0.5f, (rdrSize.y - 12.0f) * 0.5f};
        std::vector<jv::gpu::Vertex> vs;
        dino::GenVertices_Text(vs, text, Color_WHITE, Color_BLACK, textPos);
        jv::gpu::VertexBuffer* pVBuf = jv::gpu::CreateVertexBuffer("PauseText", vs);
        jv::gpu::Draw(pVBuf, m_pTextureText);
        jv::gpu::DestroyVertexBuffer(pVBuf);
    }
}
