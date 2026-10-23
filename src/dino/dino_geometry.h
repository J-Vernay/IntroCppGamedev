#pragma once

#include <dino/dino_main.h>

namespace dino
{

bool IsPointInsidePolygon(Vec2 point, const std::vector<Vec2>& polygon);

/// Vérifie si les deux segments [AB] et [CD] ont une intersection.
bool IntersectSegment(Vec2 a, Vec2 b, Vec2 c, Vec2 d);

bool ResolveCircleCollision(Vec2& posA, float radiusA, Vec2& posB, float radiusB);

bool FindSegmentIntersection(Vec2 a, Vec2 b, Vec2 c, Vec2 d, Vec2& intersection, float& t, float& u);

} // namespace dino