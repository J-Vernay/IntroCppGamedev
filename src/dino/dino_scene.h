#pragma once

#include <dino/dino_main.h>
#include <dino/dino_animal.h>
#include <dino/dino_player.h>
#include <dino/dino_terrain.h>

#include <deque>
#include <vector>

namespace dino
{

class Scene
{
public:
    Scene();
    ~Scene();
    void Update(double absTime, float deltaTime);
    void Draw() const;
    Terrain const& GetTerrain() const;

private:
    jv::gpu::Texture* m_pTextureText = nullptr;
    float m_lastDeltaTime = 0;

    Terrain m_Terrain;

    std::vector<Entity*> m_entities;

    double m_animalSpawnTime = 0;

    void _SetupPlayers();

    void _UpdateEntities(double absTime, float deltaTime);
    void _HandleCollisions();
    void _HandlePlayersLasso();
};

} // namespace dino
