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

        // Variables membres
    Vec2 m_playerPos;
    Vec2 m_aiPos;
    Vec2 m_ballPos;
    Vec2 m_ballDir;
    float m_ballSpeed;
    int m_scorePlayer = 0;
    int m_scoreAi = 0;


    void UpdateAi(double absTime, float deltaTime);
    void UpdatePlayer(double absTime, float deltaTime);
    void UpdateBall(double absTime, float deltaTime);

};


} // namespace exo2
