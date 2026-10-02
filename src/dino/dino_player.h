#pragma once

#include <dino/dino_main.h>
#include <dino/dino_terrain.h>

namespace dino
{

class Player
{
public:
    /// Initialise le joueur avec un type au hasard.
    Player(Vec2 pos, double absTime, jv::gpu::Texture* texture, int pindxPlayer,
        jv::input::GamepadIdx pgamepadIdx);

    /// Déplace le joueur et met à jour son animation.
    void Update(double absTime, float deltaTime);

    /// Affiche le joueur
    void Draw() const;

    void CheckTerrain(Terrain const& m_Terrain);

private:
    jv::input::GamepadIdx m_gamepadIdx;
    int m_idxPlayer;
    float m_timerStun;
    Vec2 m_pos;
    double m_timeStart;
    uint8_t m_alpha = 0;
    Vec2 m_dir;
    bool m_bRunning = false;
    double m_absTime;
    int32_t m_kind;
    jv::gpu::Texture* m_pTexture;
};
};
