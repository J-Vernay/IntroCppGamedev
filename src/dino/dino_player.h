#pragma once

#include <dino/dino_main.h>
#include <dino/dino_terrain.h>
#include <dino/Movable.h>

namespace dino
{

class Player: public Movable
{
public:
    /// Initialise le joueur avec un type au hasard.
    Player(Vec2 pos, double absTime, jv::gpu::Texture* texture, int pindxPlayer,
        jv::input::GamepadIdx pgamepadIdx, Color pColor);

    /// Affiche le joueur
    void Draw() const override;
    void Update(double absTime, float deltaTime);

    std::vector<Vec2> m_points;

private:
    jv::input::GamepadIdx m_gamepadIdx;
    int m_idxPlayer;
    float m_timerStun;
    float m_timerTrail;
    bool m_bRunning = false;
    double m_absTime;
    std::vector<jv::gpu::Vertex> m_trailVertex;
    Color m_color;
    void Trail(float deltaTime);

};
};
