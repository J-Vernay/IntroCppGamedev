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
    jv::gpu::Texture* m_pTexturePlayer = nullptr;
    float m_lastDeltaTime = 0;
    float m_timer = 60.0f;
    bool m_bPaused = false;

    bool m_bDeleteWasDown = false;
    void _ReturnToLobby(double absTime);

    jv::util::Vec2 m_tmerPos = Vec2(480 / 2, 10);

    Terrain m_Terrain;

    std::deque<Player> m_players;
    std::deque<Animal> m_animals;
    double m_animalSpawnTime = 0;

    void _UpdateAnimals(double absTime, float deltaTime);

    void _UpdatePlayer(double absTime, float deltaTime);

    void LassoCollisionCheck();
    static constexpr float kTreeCollisionRadius = 10.f;

    void PauseGame() const;

    enum class GameState{Lobby, Playing};
    GameState m_state = GameState::Lobby;
    static constexpr float kTimerStart = 60.f;

    //le lobby locké
    float m_lobbyLock = 0.f;
    static constexpr float kLobbyLockTime = 5.f;
    void _ReturnToLobby(double absTime, bool bTimerExpired = false);

    jv::util::Vec2 _LobbyTreePos(int32_t idxSeason) const;
    void _StartGame(int32_t idxSeason, double absTime);

    void _DrawScores() const;

    //on defini on écart enttre les arbres
    static constexpr float kTreeSpacing = 72.f;
    static constexpr float kTreeTriggerRadius = 14.f;

    int32_t _LobbyLassoCheck();
};

} // namespace dino
