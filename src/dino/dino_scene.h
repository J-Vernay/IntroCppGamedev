#pragma once

#include <dino/dino_main.h>
#include <dino/dino_animal.h>
#include <dino/dino_player.h>
#include <dino/dino_terrain.h>
#include "dino/Tree.h"

#include <deque>

namespace dino
{

class Scene
{
public:
    Scene();
    ~Scene();
    void Update(double absTime, float deltaTime);
    void Draw() const;
    void SwitchPauseState();

private:
    void SortEntitys();
    static bool HeightDiff(Entity* firstElt, Entity* secondElt);

    jv::gpu::Texture* m_pTextureText = nullptr;
    jv::gpu::Texture* m_pTextureAnimal = nullptr;
    jv::gpu::Texture* m_pTexturePlayer = nullptr;
    jv::gpu::Texture* m_pTextureTerrain = nullptr;
    float m_lastDeltaTime = 0;


    float m_spawnTime = 0.3;
    float m_timer = 60;
    float m_timeScale = 1;
    bool isPaused = false;

    Terrain m_Terrain;
    Player m_unactivePlayers[4];
    std::deque<Player> m_players;

    std::deque<Animal> m_animals;
    std::deque<Tree> m_trees;
    std::vector<Entity*> m_entitys;
    double m_animalSpawnTime = 0;

    void _UpdateAnimals(double absTime, float deltaTime);
    void _UpdateTimer(double absTime, float deltaTime);

    void HandleLassoColisions();

    void DrawTimer() const;

    void CheckEntitysInLoop(std::vector<Vec2> lassoPoints);

    void CleanEntitys();

    void PlayerJoin();
    void StartGame();

    enum SceneState
    {
        Lobby,
        Game
    };

    SceneState m_SceneState = Lobby;
};

} // namespace dino
