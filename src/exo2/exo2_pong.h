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

    // VARIABLES
    Color m_color;

};

} // namespace exo2
