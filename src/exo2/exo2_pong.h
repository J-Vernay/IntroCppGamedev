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

    // Variables globales
    Vec2 m_playerPos;
    Vec2 m_aiPos;
    Vec2 m_ballPos;
    Vec2 m_ballDir;
    float m_ballSpeed;
    int m_playerScore = 0;
    int m_aiScore = 0;

    void _UpdateAI(double absTime, float deltaTime);
    void _UpdatePlayer(double absTime, float deltaTime);
    void _UpdateBall(double absTime, float deltaTime);
    void _UpdateWin(double absTime, float deltaTime);

};

} // namespace exo2
