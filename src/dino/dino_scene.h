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
    float m_lastDeltaTime = 0;
    jv::gpu::Texture* m_pTexture;
    jv::gpu::Texture* m_pTextureDinosaurs;

    Terrain m_Terrain;

    std::vector<Movable*> m_movable;
    std::deque<Animal> m_animals;
    std::deque<Player> m_players;
    double m_animalSpawnTime = 0;

    void _UpdatePlayer(double absTime, float deltaTime);
    void _UpdateAnimals(double absTime, float deltaTime);
};

} // namespace dino
