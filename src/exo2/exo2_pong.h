#pragma once

#include <jv/jv.h>

namespace exo2
{

class Pong
{
public:
    Pong();

    void Update(double absTime, float deltaTime);

    void Draw() const;

private:
    using Vec2 = jv::util::Vec2;
    using Color = jv::util::Color;

    // VARIABLES
    Color m_color;

    // Variables globales
    Vec2 g_playerPos;
    Vec2 g_aiPos;
    Vec2 g_ballPos;
    Vec2 g_ballDir;
    float g_ballSpeed;

    void _UpdateAI(double absTime, float deltaTime);
    void _UpdatePlayer(float deltaTime);
    void _UpdateBall(float deltaTime);

};

} // namespace exo2
