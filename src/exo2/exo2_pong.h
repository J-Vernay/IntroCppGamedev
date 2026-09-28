#pragma once

#include <jv/jv.h>

namespace exo2
{

class Pong
{
public:
    Pong();

    void Update(double absTime, float deltaTime);

    void _UpdateAI(double absTime);
    void _UpdatePlayer(float delataTime);
    void _UpdateBall(float delataTime);

    void Draw() const;

  

private:
    using Vec2 = jv::util::Vec2;
    using Color = jv::util::Color;

    int m_scorePlayer = 0;
    int m_scoreAi = 0;

    Vec2 m_playerPos;
    Vec2 m_aiPos;
    Vec2 m_ballPos;
    Vec2 m_ballDir;
    float m_ballSpeed;

    Color m_color;

};

} // namespace exo2
