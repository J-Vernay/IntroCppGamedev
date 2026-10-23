#pragma once

#include "dino_player.h"

#include <dino/dino_main.h>
#include <dino/dino_animal.h>
#include <dino/dino_terrain.h>

#include <array>

#include <list>

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

    
    jv::gpu::Texture* m_pTextureAnimals = nullptr;

private:
    jv::gpu::Texture* m_pTextureText = nullptr;
    jv::gpu::Texture* m_pTexturePlayers = nullptr;

    std::deque<Player> m_players;
    std::list<Animal> m_animals;

    float m_lastDeltaTime = 0;

    Terrain m_Terrain;

    double m_animalSpawnAccumulator = 0.0;

    void _UpdateAnimals(double absTime, float deltaTime);

    void _ResolveCollisions();

    void _CutPlayerLassos(const std::vector<Vec2>& previousPositions);

    int m_lastLoopAnimalCount = 0;

    void _CheckClosedLoops();

    void _DrawScores() const;

    static constexpr double kRoundDuration = 60.0;

    double m_roundTimeRemaining = kRoundDuration;
    double m_simulationTime = 0.0;

    void _DrawRoundTimer() const;

    bool m_isPaused = false;

    void _DrawPauseMessage() const;

    enum class GameState
    {
        Lobby,
        Playing
    };

    GameState m_gameState = GameState::Lobby;

    double m_lobbyDelayRemaining = 0.0;

    void _StartRound();
    void _EnterLobby();

    std::array<bool, 5> m_startWasDown{};
    std::array<bool, 5> m_selectWasDown{};
    std::array<bool, 5> m_launchWasDown{};

    bool _UpdateInputs();
};

} // namespace dino
