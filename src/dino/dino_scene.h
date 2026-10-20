#pragma once

#include <deque>
#include <dino/dino_animal.h>
#include <dino/dino_main.h>
#include <dino/dino_player.h>
#include <dino/dino_terrain.h>
#include <dino/PlayerState.cpp> 
#include <dino/dino_tree.h>

namespace dino
{

class Scene
{
public:
    Scene();
    ~Scene();
    void Update(double absTime, float deltaTime);
    void Draw() const;


private:
    jv::gpu::Texture* m_pTextureText = nullptr;
    float m_lastDeltaTime = 0;
    double m_lastAbsTime = 0;
    jv::gpu::Texture* m_pTexture;
    jv::gpu::Texture* m_pTextureTree;
    jv::gpu::Texture* m_pTextureDinosaurs;

    Terrain m_Terrain;

    std::vector<Movable*> m_movable;
    std::deque<Animal> m_animals;
    std::deque<Player> m_players;
    std::deque<dino_tree> m_trees;
    

    jv::util::Color colors[4] = {Color_BLUE, Color_RED, Color_YELLOW, Color_GREEN};
    double m_animalSpawnTime = 0;
    float m_timer = 60;
    jv::util::Vec2 m_whereTimer = Vec2(480/2, 10);   
    jv::util::Vec2 m_whereScore = Vec2(2, 50);   
    void _UpdatePlayer(double absTime, float deltaTime);
    void CheckLoop(Player& player);
    void _UpdateAnimals(double absTime, float deltaTime);
    bool m_isPaused = false;
    PlayerState m_currentState = PlayerState::Lobby;
    void SpawnTree();
};


} // namespace dino
