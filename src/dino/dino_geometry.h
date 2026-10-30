#pragma once

#include <dino/dino_main.h>

namespace dino
{

/// Vérifie si les deux segments [AB] et [CD] ont une intersection.
bool IntersectSegment(Vec2 a, Vec2 b, Vec2 c, Vec2 d);

/// Vérifie si un point est à l'intérieur d'un polygone ou non
bool IsPointInPolygon(Vec2 point, std::vector<Vec2> const& polygon);

} // namespace dino