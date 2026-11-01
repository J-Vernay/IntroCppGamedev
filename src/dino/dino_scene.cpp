
#include <dino/dino_draw_utils.h>
#include <dino/dino_scene.h>
#include <iostream>
#include <format>
#include <algorithm>
#include "dino_geometry.h"

dino::Scene::Scene()
    : m_Terrain{24, 16},
      m_unactivePlayers{
          Player{m_Terrain.GenerateRandomSpawn(), 0, jv::input::GamepadIdx::Keyboard, 0},
          Player{m_Terrain.GenerateRandomSpawn(), 0, jv::input::GamepadIdx::Gamepad1, 1},
          Player{m_Terrain.GenerateRandomSpawn(), 0, jv::input::GamepadIdx::Gamepad2, 2},
          Player{m_Terrain.GenerateRandomSpawn(), 0, jv::input::GamepadIdx::Gamepad3, 3}}
{
    m_pTextureText = dino::LoadImageAsset("monogram-bitmap.bmp");
    m_pTextureAnimal = dino::LoadImageAsset("animals.bmp");
    m_pTextureTerrain = dino::LoadImageAsset("terrain.bmp");

    
    for (int i = 0; i < 4; i++)
    {
        Vec2 pos = {140 + i * 80,200};
        m_trees.emplace_back(pos, (float)0 , i, m_pTextureTerrain);
        m_entitys.push_back(&m_trees.back());
    }
    

    /* for (Player& p : m_players)
    {
        m_entitys.push_back(&p);
    }*/

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
    bool hasPaused = false;
    if (m_SceneState == Lobby)
    {
        PlayerJoin();
    }
    for (Player& p : m_players)
    {

        p.PauseInput();
        if (p.hasPaused)
        {
            hasPaused = true;
        }
    }
    if (hasPaused)
    {
        SwitchPauseState();
    }

    if (!isPaused)
    {
        m_Terrain.Update(absTime, deltaTime);

        if (m_SceneState == Game)
        {
            _UpdateTimer(absTime, deltaTime);

            _UpdateAnimals(absTime, deltaTime);
        
        }

        for (Player& p : m_players)
        {
            p.Update(absTime, deltaTime);
            p.HandlePhysics(m_entitys);
            
        }
        HandleLassoColisions();

        for (Entity* e : m_entitys)
        {
            e->HandleTerrainClamp(&m_Terrain);
        }
        SortEntitys();
    }
}



void dino::Scene::_UpdateTimer(double absTime, float deltaTime)
{
    m_timer -= deltaTime;
    if (m_timer <= 0)
    {
        ReturnToLobby();
    }
}


void dino::Scene::_UpdateAnimals(double absTime, float deltaTime)
{
    // Spawner un animal si besoin.

    if (absTime - m_animalSpawnTime >= m_spawnTime)
    {
        m_animalSpawnTime = absTime;
        m_spawnTime = m_timer / 60 + 0.1;
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

void dino::Scene::HandleLassoColisions()
{
    for (size_t i = 0; i < m_players.size(); i++)
    {
        for (size_t j = 0; j < m_players.size(); j++)
        {
            std::vector<Vec2>* lassoPoints = m_players[i].GetLassoPoints();

            Vec2 pos = m_players[j].GetPos();
            Vec2 lastPos = m_players[j].GetLastPos();

            if (i == j)
            {
                
                if (lassoPoints->size() < 6) continue;
                
                for (int h = lassoPoints->size() - 5; h > 0; h--)
                {
                    if (dino::IntersectSegment(
                            lastPos, pos, lassoPoints->at(h), lassoPoints->at(h + 1)))
                    {

                        std::vector<Vec2> loopPoints(lassoPoints->begin() + h + 1, lassoPoints->end());

                        CheckEntitysInLoop(loopPoints, i);

                        m_players[i].HandleLassoColision(lassoPoints->size() - h);
                        break;
                    }
                }
            }
            if (lastPos.x == pos.x && lastPos.y == pos.y)
                continue;
            if (lassoPoints->size() < 6)
                continue;
            for (int h = lassoPoints->size() - 5; h > 1; h--)
            {
                if (dino::IntersectSegment(
                        lastPos, pos, lassoPoints->at(h), lassoPoints->at(h + 1)))
                {
                    m_players[i].HandleLassoOpponentCollision(h);
                    break;
                }
            }
        }
    }
    CleanEntitys();
}

void dino::Scene::CheckEntitysInLoop(std::vector<Vec2> lassoPoints, int playerIndex)
{
    int n = 0;
    int points = 0;
    for (Entity& e : m_animals)
    {
        n = e.RayCastCollisionCheck(lassoPoints);
        if (n % 2 != 0)
        {
            e.HandleCapture();
            points += 10;
        }
    }
    m_players[playerIndex].points += points;
    for (Entity& e : m_players)
    {
        n = e.RayCastCollisionCheck(lassoPoints);
        if (n % 2 != 0)
        {
            e.HandleCapture();
        }
    }
    for (Entity& e : m_trees)
    {
        n = e.RayCastCollisionCheck(lassoPoints);
        if (n % 2 != 0)
        {
            e.HandleCapture();

            Tree& t = static_cast<Tree&>(e);
            m_Terrain.SetSeason(t.m_TreeType);
            StartGame();
        }
    }
}

void dino::Scene::CleanEntitys()
{
    for (int i = m_animals.size()-1; i >= 0; i--)
    {
        if (!m_animals[i].isAlive)
        {
            m_animals.erase(m_animals.begin() + i);
        }
    }
    for (int i = m_trees.size() - 1; i >= 0; i--)
    {
        if (!m_trees[i].isAlive)
        {
            m_trees.erase(m_trees.begin() + i);
        }
    }
    m_entitys.clear();
    for (Player& p : m_players)
    {
        m_entitys.push_back(&p);
    }
    for (Animal& animal : m_animals)
    {
        m_entitys.push_back(&animal);
    }
    for (Tree& tree : m_trees)
    {
        m_entitys.push_back(&tree);
    }
}

void dino::Scene::PlayerJoin()
{
    if (m_SceneState != Lobby)
    {
        return;
    }

    for (Player& p : m_unactivePlayers)
    {
        if (p.Join())
        {
            m_players.push_back(p);
            CleanEntitys();
        }
        if (p.Leave())
        {
            int idx = -1;
            for (int i = 0; i < m_players.size(); i++)
            {
                if (p.GetId() == m_players[i].GetId())
                {
                    idx = i;
                }
            }
            m_players.erase(m_players.begin() + idx);
            CleanEntitys();
        }
    }
}

void dino::Scene::StartGame()
{
    for (Tree& t : m_trees)
    {
        t.isAlive = false;
    }

    for (Player& p : m_players)
    {
        p.points = 0;
    }
    CleanEntitys();
    m_timer = 60;
    m_SceneState = Game;
}

void dino::Scene::ReturnToLobby()
{
    m_SceneState = Lobby;

    for (Animal& a : m_animals)
    {
        a.isAlive = false;
    }

    for (int i = 0; i < 4; i++)
    {
        Vec2 pos = {140 + i * 80, 200};
        m_trees.emplace_back(pos, (float)0, i, m_pTextureTerrain);
        m_entitys.push_back(&m_trees.back());
    }

    CleanEntitys();
}



void dino::Scene::Draw() const
{
    m_Terrain.Draw();

    for (Entity* const& e : m_entitys)
    {
        if (e != nullptr)
        {
            e->Draw();
        }
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
    DrawTimer();
    DrawScore();
}

void dino::Scene::DrawTimer () const
{
    std::string text = std::format("Timer = {:.1f}s", m_timer);
    std::vector<jv::gpu::Vertex> vs;
    dino::GenVertices_Text(vs, text, Color_WHITE, Color_GREY, Vec2{200, 0});
    jv::gpu::VertexBuffer* pVBuf = jv::gpu::CreateVertexBuffer("dTimer", vs);
    jv::gpu::Draw(pVBuf, m_pTextureText);
    jv::gpu::DestroyVertexBuffer(pVBuf);
}

void dino::Scene::DrawScore() const
{
    for (int i = 0; i < m_players.size(); i++)
    {
        std::string text = std::format("Score = {:} points", m_players[i].points);
        std::vector<jv::gpu::Vertex> vs;
        dino::GenVertices_Text(
            vs, text, Color_WHITE, m_players[i].GetColor(), Vec2{0, (float)i * 50 + 100});
        jv::gpu::VertexBuffer* pVBuf = jv::gpu::CreateVertexBuffer("dScore", vs);
        jv::gpu::Draw(pVBuf, m_pTextureText);
        jv::gpu::DestroyVertexBuffer(pVBuf);
    }
}



void dino::Scene::SwitchPauseState()
{
    isPaused = !isPaused;
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
