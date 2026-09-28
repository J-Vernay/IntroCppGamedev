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
    Vec2 m_playerPos;
    Vec2 m_aiPos;
    Vec2 m_ballPos;
    Vec2 m_ballDir;
    float m_ballSpeed;

    unsigned char m_playerScore;
    unsigned char m_aiScore;

    void _UpdateAI(double absTime, float deltaTime);
    void _UpdatePlayer(double absTime, float deltaTime);
    void _UpdateBall(double absTime, float deltaTime);
};

} // namespace exo2
