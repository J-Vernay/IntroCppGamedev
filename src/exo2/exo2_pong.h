#pragma once

#include <jv/jv.h>

namespace exo2
{

class Pong
{
public:
    Pong();

    void Update(double absTime, float deltaTime);

    void Draw();

private:
    using Vec2 = jv::util::Vec2;
    using Color = jv::util::Color;

    // VARIABLES
    Color m_color;
    Vec2 g_playerPos;
    Vec2 g_aiPos;
    Vec2 g_ballPos;
    Vec2 g_ballDir;
    float g_ballSpeed;

};

} // namespace exo2
