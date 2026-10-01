#pragma once

#include <dino/dino_main.h>
#include <dino/dino_animal.h>
#include <dino/dino_terrain.h>
#include <dino/dino_player.h>

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

private:
    jv::gpu::Texture* m_pTextureText = nullptr;
    jv::gpu::Texture* m_pTextureAnimal = nullptr;
    float m_lastDeltaTime = 0;

    Terrain m_Terrain;
    Player m_players[4];

    std::deque<Animal> m_animals;
    double m_animalSpawnTime = 0;

    void _UpdateAnimals(double absTime, float deltaTime);
    void _UpdatePlayers(double absTime, float deltaTime);
};

} // namespace dino
