#pragma once

#include <dino/Movable.h>
#include <dino/PlayerState.cpp>
#include <dino/dino_main.h>
#include <dino/dino_terrain.h>
namespace dino
{

class Player : public Movable
{
public:
    /// Initialise le joueur avec un type au hasard.
    Player(Vec2 pPos, jv::gpu::Texture* pTexture, int pIndxPlayer,
        jv::input::GamepadIdx pGamepadIdx, Color pColor, PlayerState pState);

    /// Affiche le joueur
    void Draw() const override;

    ///verifie si la trail a fait une boucle
    std::vector<dino::Vec2> CheckIfLoop();
    void Update(double pAbsTime, float pDeltaTime);

    ///mais le joueur en stun
    void TakeDamage();

    ///verifie si le joueur souhaite mettre en pause ou pas le jeu
    bool IsPause();
    std::vector<Vec2> m_points;
    bool m_isSpawned = false;
    ///permet de verifier si le joueur est dans le lobby ou pas
    void ChangeState(PlayerState pState);
    int m_score = 0;

private:
    jv::input::GamepadIdx m_gamepadIdx;
    int m_idxPlayer;
    float m_timerStun;
    bool m_bRunning = false;
    double m_absTime;
    std::vector<jv::gpu::Vertex> m_trailVertex;
    Color m_color;

    ///creer la trail du joueur
    void _Trail();
    bool m_IsPaused = false;
    PlayerState m_currentState;
    bool m_prevStart = false;
    int m_stunLenght = 3;
    int m_maxLenghtTrail = 120;
};
}; // namespace dino
