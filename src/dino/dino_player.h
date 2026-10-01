#pragma once

#include <dino/dino_main.h>

namespace dino
{

class Player
{
public:
    /// Initialise le joueur avec un type au hasard.
    Player(Vec2 pos, double absTime, jv::gpu::Texture* texture);

    /// Déplace le joueur et met à jour son animation.
    void Update(double absTime, float deltaTime);

    /// Affiche le joueur
    void Draw() const;

    /// Détruit les ressources associées au joueur.
    ~Player();

private:
    Vec2 m_pos;
    double m_timeStart;
    uint8_t m_alpha = 0;
    Vec2 m_dir;
    bool m_bRunning;
    double m_absTime;
    int32_t m_kind;
    int32_t m_idxFrame;
    jv::gpu::Texture* m_pTexture;
};
};
