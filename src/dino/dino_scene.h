#pragma once

#include <dino/dino_main.h>
#include <dino/dino_animal.h>
#include <dino/dino_player.h>
#include <dino/dino_terrain.h>
#include <dino/dino_tree.h>
#include <dino/dino_pause.h>

#include <deque>
#include <vector>

namespace dino
{

class Scene
{
public:
    bool m_game = false;
    bool m_pause = false;
    float m_timer = 0.f;

    Scene();
    ~Scene();
    void Update(double absTime, float deltaTime);
    void Draw() const;
    void RemoveEntity(Entity* entity);
    void StartGame(int32_t season);
    void StartLobby(double absTime);
    void SetPause();
    Terrain const& GetTerrain() const;

private:
    float m_lastDeltaTime = 0;

    Terrain m_Terrain;

    std::vector<Entity*> m_entities;

    double m_animalSpawnTime = 0;

    Pause m_pauseHandler = *(new Pause);

    void _PreGameLogic();

    void _UpdateEntities(double absTime, float deltaTime);
    void _HandleCollisions();
    void _HandlePlayersLasso();

    const float g_gameDuration = 60.0f;
};

} // namespace dino
