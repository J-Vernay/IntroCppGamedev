#pragma once

#include <dino/dino_main.h>
#include <dino/dino_animal.h>
#include <dino/dino_player.h>
#include <dino/dino_terrain.h>
#include <dino/dino_tree.h>

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
    std::deque<Player*> m_activePlayers;

    std::deque<Tree> m_trees;
    jv::gpu::Texture* m_treeTexture;

    std::deque<Animal> m_animals;
    double m_animalSpawnTime = 0;
    jv::gpu::Texture* m_animalTexture;

    std::deque<Entity*> m_entities;

    std::vector<std::pair<Vec2, Vec2>> m_playerLastMoves;

    static constexpr float GameTime = 60;
    float m_timer = GameTime;
    bool m_bPause = false;
    bool m_bGameStarted = false;

    jv::input::GamepadIdx m_gamepads[4] = {
        jv::input::GamepadIdx::Keyboard,
        jv::input::GamepadIdx::Gamepad1,
        jv::input::GamepadIdx::Gamepad2,
        jv::input::GamepadIdx::Gamepad3
    };

    void _StartGame(int32_t idxSeason);
    void _CheckPause();
    void _CheckJoinLeave();
    void _UpdatePlayers(double absTime, float deltaTime);
    void _UpdateAnimals(double absTime, float deltaTime);
    void _UpdateCollisions(double absTime, float deltaTime);
    void _CheckGameStart();
    void _RefreshEntities();
    void _DrawTimer() const;
    void _OnPlayerLoop(Player* pPlayer, int32_t start, int32_t end);
};

} // namespace dino
