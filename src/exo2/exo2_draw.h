#pragma once

#include <jv/jv.h>

namespace exo2
{

using jv::util::Color;
using jv::util::Vec2;

/// Affiche un rectangle coloré.
void DrawRect(Vec2 pos, Vec2 size, Color color);

/// Affiche le score du joueur et de l'IA sous forme de rectangles.
void DrawScore(
    float center, Vec2 pointSize, int scorePlayer, Color colorPlayer, int scoreAI, Color colorAI);

} // namespace exo2