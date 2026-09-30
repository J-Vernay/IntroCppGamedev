#pragma once

#include <dino/dino_main.h>
#include <dino/dino_terrain.h>
#include <dino/dino_entity.h>

namespace dino
{

// Représente un joueur.
class Player : public Entity
{
public:
    /// Initialise le joueur.
    Player(Vec2 pos, double absTime, jv::gpu::Texture* texture, jv::input::GamepadIdx gamepadIdx,
        int32_t color, Terrain* terrain);

    /// Déplace le joueur et met à jour son animation.
    void Update(double absTime, float deltaTime);

    /// Affiche le joueur.
    void Draw() const;

    /// Détruit les ressources associées au joueur.
    ~Player();

private:
    enum State
    {
        IDLE,
        WALK,
        RUN,
        HURT
    };

    double m_timeStart;
    uint8_t m_alpha = 255;
    Vec2 m_dir;
    bool m_facingLeft = false;
    float m_hurtTime = 0;
    State m_state;
    int32_t m_idxFrame = 0;
    int32_t m_color;
    jv::input::GamepadIdx m_gamepadIdx;
    jv::gpu::Texture* m_pTexture;

protected:
    void OnOutsideTerrain() override;
};

}