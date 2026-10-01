#pragma once

#include <dino/dino_main.h>
#include <dino/dino_animal.h>
#include <dino/dino_terrain.h>
#include <dino/dino_players.h>
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
    float m_lastDeltaTime = 0;

    Terrain m_Terrain;

    std::deque<Animal> m_animals;
    std::deque<Player> m_players;
    double m_animalSpawnTime = 0;
    jv::gpu::Texture* pTexture;
    jv::gpu::Texture* pTexturePlayer;
    void _UpdateAnimals(double absTime, float deltaTime);
};

} // namespace dino
