#include <exo2/exo2_draw.h>
#include <exo2/exo2_pong.h>

#include <algorithm>
#include <cmath>

namespace exo2
{

using jv::util::Color;

// CONSTANTES ICI


} // namespace exo2

exo2::Pong::Pong()
{
    // INITIALISATION
    jv::gpu::SetRenderSize({10, 10});
}

void exo2::Pong::Update(double absTime, float deltaTime)
{
    // UPDATE
    m_color.r = 128;
    m_color.g = 127.5 + 127.5 * std::sin(absTime * 3);
    m_color.b = 127.5 + 127.5 * std::cos(absTime * 3);
    m_color.a = 255;
}

void exo2::Pong::Draw() const
{
    // DRAW
    jv::gpu::SetBackgroundColor(m_color);
}
