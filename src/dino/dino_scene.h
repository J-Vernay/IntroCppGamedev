#pragma once

#include <dino/dino_main.h>
#include <dino/dino_animal.h>
#include <dino/dino_player.h>
#include <dino/dino_terrain.h>

#include <deque>

namespace dino
{

class Scene
{
public:
    Scene();
    ~Scene();
    void Update(double absTime, float deltaTime);
    // Trier les entités à afficher selon leur position y.
    void SortEntities();
    void Draw() const;

private:
    jv::gpu::Texture* m_pTextureText = nullptr;
    float m_lastDeltaTime = 0;

    Terrain m_terrain;
    
    std::deque<Player> m_players;
    jv::gpu::Texture* m_playerTexture;

    std::deque<Animal> m_animals;
    double m_animalSpawnTime = 0;
    jv::gpu::Texture* m_animalTexture;

    std::deque<Entity*> m_entities;

    std::vector<std::pair<Vec2, Vec2>> m_playerLastMoves;

    float m_timer = 60;
    bool m_bPause = false;

    jv::input::GamepadIdx m_gamepads[4] = {
        jv::input::GamepadIdx::Keyboard,
        jv::input::GamepadIdx::Gamepad1,
        jv::input::GamepadIdx::Gamepad2,
        jv::input::GamepadIdx::Gamepad3
    };

    void _CheckPause();
    void _UpdatePlayers(double absTime, float deltaTime);
    void _UpdateAnimals(double absTime, float deltaTime);
    void _UpdateCollisions(double absTime, float deltaTime);
    void _DrawTimer() const;
    void _OnPlayerLoop(Player* pPlayer, int32_t start, int32_t end);
};

} // namespace dino
