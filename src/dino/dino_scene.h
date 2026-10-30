#pragma once

#include <dino/dino_main.h>
#include <dino/dino_animal.h>
#include <dino/dino_terrain.h>
#include <dino/dino_player.h>
#include <deque>

namespace dino
{
enum class GameState
{
    Lobby,
    Playing
};

struct FloatingText // notifications de score
{
    Vec2 pos;
    std::string text;
    jv::util::Color color;
    float lifetime = 1.0f;
};
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
    jv::gpu::Texture* m_pTexturePlayer = nullptr;

    float m_lastDeltaTime = 0;

    GameState m_GameState = GameState::Lobby;
    double m_lobbyCooldown = 0.0;

    double m_timeLeft = 60.0;
    bool m_bPaused = false;
    bool m_bWasStartPressed = false;

    Terrain m_Terrain;

    std::deque<Player> m_players;
    std::deque<Animal> m_animals;
    std::vector<Entity*> m_entities;
    double m_animalSpawnTime = 0;
    std::vector<FloatingText> m_notifications;

    void _UpdateAnimals(double absTime, float deltaTime);
};

} // namespace dino
