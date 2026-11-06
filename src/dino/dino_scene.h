#pragma once

#include <dino/dino_animal.h>
#include <dino/dino_main.h>
#include <dino/dino_player.h>
#include <dino/dino_terrain.h>
#include <dino/dino_player.h>

#include <array>
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

private:
    enum class State
    {
        Lobby,
        Playing,
        Paused,
    };

    jv::gpu::Texture* m_pTextureText = nullptr;
    jv::gpu::Texture* m_pTextureAnimal = nullptr;
    jv::gpu::Texture* m_pTexturePlayer = nullptr;
    jv::gpu::Texture* m_pTextureWhite = nullptr;

    float m_lastDeltaTime = 0;
    double m_lastAbsTime = 0;
    double m_sceneTime = 0;
    double m_gameElapsed = 0;
    double m_animalSpawnTime = 0;
    double m_treeSelectionEnabledAt = 0;

    State m_state = State::Lobby;

    Terrain m_Terrain;
    std::deque<Player> m_players;
    std::deque<Animal> m_animals;
    std::vector<Entity*> m_entities;

    std::array<jv::input::Gamepad, 4> m_gamepads{};
    std::array<bool, 4> m_gamepadAvailable{};
    std::array<bool, 4> m_wasStartDown{};
    std::array<bool, 4> m_wasSelectDown{};

    void _UpdateInput();
    void _UpdateLobby(float deltaTime);
    void _UpdateGame(float deltaTime);
    void _UpdateAnimals(float deltaTime);
    void _BuildEntityList();
    void _StartGame(int32_t idxSeason);
    void _EndGame();
};

} // namespace dino