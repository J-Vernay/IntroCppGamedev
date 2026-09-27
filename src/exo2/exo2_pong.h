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

};

} // namespace exo2
