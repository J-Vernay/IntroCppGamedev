#pragma once

#include <dino/dino_main.h>
#include <dino/dino_terrain.h>
#include <dino/dino_entity.h>

namespace dino
{

using PointList = std::vector<Vec2>;

// Représente un joueur.
class Player : public Entity
{
public:
    /// Initialise le joueur.
    Player(Vec2 pos, double absTime, jv::gpu::Texture* texture, jv::input::GamepadIdx gamepadIdx, Color color,
        int32_t colorIndex);

    bool Pauses();

    bool CheckJoin();

    bool CheckLeave();

    void Update(double absTime, float deltaTime, Terrain& terrain) override;

    void UpdateTrail(
        double absTime, float deltaTime, std::vector<std::pair<Vec2, Vec2>> playersLastMove);

    void Draw() const override;

    void DrawTrail() const;

    void OnCaughtInLoop() override;

    /// Détruit les ressources associées au joueur.
    ~Player();

    std::pair<int32_t, int32_t> CheckLoop();

    PointList GetTrail();

    void CutLoop(int32_t loopPoint1, int32_t loopPoint2);

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
    float m_hurtTimer = 0;
    State m_state;
    int32_t m_idxFrame = 0;
    int32_t m_colorIndex;
    jv::input::GamepadIdx m_gamepadIdx;
    jv::gpu::Texture* m_pTexture;
    PointList m_pastPositions;
    Color m_color;
    bool m_bPauseReleased = true;
    bool m_bHasJoined = false;

    void CheckPlayerTrailOverlap(std::vector<std::pair<Vec2, Vec2>> playersLastMove);

protected:
    void OnOutsideTerrain() override;
};

}