#include <dino/dino_geometry.h>
#include <cmath>

bool dino::IsPointInsidePolygon(Vec2 point, const std::vector<Vec2>& polygon)
{
    if (polygon.size() < 3)
        return false;

    bool inside = false;
    constexpr double epsilon = 0.0001;

    for (std::size_t i = 0, j = polygon.size() - 1; i < polygon.size(); j = i++)
    {
        const Vec2 a = polygon[j];
        const Vec2 b = polygon[i];

        const double dx = double(b.x) - a.x;
        const double dy = double(b.y) - a.y;

        const double px = double(point.x) - a.x;
        const double py = double(point.y) - a.y;

        const double lengthSquared = dx * dx + dy * dy;

        if (lengthSquared <= 1e-12)
            continue;

        const double cross = dx * py - dy * px;
        const double dot = px * dx + py * dy;

        if (cross * cross <= epsilon * epsilon * lengthSquared && dot >= 0.0 &&
            dot <= lengthSquared)
        {
            return true;
        }

        if ((a.y > point.y) != (b.y > point.y))
        {
            const double crossingX = a.x + py * dx / dy;

            if (point.x < crossingX)
                inside = !inside;
        }
    }

    return inside;
}

bool dino::IntersectSegment(Vec2 A, Vec2 B, Vec2 C, Vec2 D)
{
    if (A.x == B.x && A.y == B.y)
        return false;

    float z_AB_AC = (B.x - A.x) * (C.y - A.y) - (B.y - A.y) * (C.x - A.x);
    float z_AB_AD = (B.x - A.x) * (D.y - A.y) - (B.y - A.y) * (D.x - A.x);
    float z_CD_CA = (D.x - C.x) * (A.y - C.y) - (D.y - C.y) * (A.x - C.x);
    float z_CD_CB = (D.x - C.x) * (B.y - C.y) - (D.y - C.y) * (B.x - C.x);

    if (z_AB_AC * z_AB_AD > 0)
        return false; // CD separated on axis perpendicular to AB
    if (z_CD_CA * z_CD_CB > 0)
        return false; // AB separated on axis perpendicular to CD

    // What if ABCD are aligned?

    float dot_AB_AB = (B.x - A.x) * (B.x - A.x) + (B.y - A.y) * (B.y - A.y);
    float dot_AB_AC = (B.x - A.x) * (C.x - A.x) + (B.y - A.y) * (C.y - A.y);
    float dot_AB_AD = (B.x - A.x) * (D.x - A.x) + (B.y - A.y) * (D.y - A.y);

    if (dot_AB_AC <= dot_AB_AD)
        return 0 <= dot_AB_AD && dot_AB_AC <= dot_AB_AB;
    else
        return 0 <= dot_AB_AC && dot_AB_AD <= dot_AB_AB;
}

bool dino::ResolveCircleCollision(Vec2& posA, float radiusA, Vec2& posB, float radiusB)
{
    float dx = posB.x - posA.x;
    float dy = posB.y - posA.y;

    float distanceSquared = dx * dx + dy * dy;
    float radiusSum = radiusA + radiusB;

    if (distanceSquared >= radiusSum * radiusSum)
        return false;

    float distance = std::sqrt(distanceSquared);

    Vec2 normal{1.f, 0.f};

    if (distance > 0.f)
    {
        normal.x = dx / distance;
        normal.y = dy / distance;
    }

    float overlap = radiusSum - distance;
    float correction = overlap * 0.5f;

    posA.x -= normal.x * correction;
    posA.y -= normal.y * correction;

    posB.x += normal.x * correction;
    posB.y += normal.y * correction;

    return true;
}

bool dino::FindSegmentIntersection(Vec2 a, Vec2 b, Vec2 c, Vec2 d, Vec2& intersection, float& t, float& u)
{
    Vec2 ab{b.x - a.x, b.y - a.y};
    Vec2 cd{d.x - c.x, d.y - c.y};
    Vec2 ac{c.x - a.x, c.y - a.y};

    float determinant = ab.x * cd.y - ab.y * cd.x;

    if (std::abs(determinant) < 0.000001f)
        return false;

    t = (ac.x * cd.y - ac.y * cd.x) / determinant;
    u = (ac.x * ab.y - ac.y * ab.x) / determinant;

    if (t < 0.f || t > 1.f || u < 0.f || u > 1.f)
        return false;

    intersection = {a.x + t * ab.x, a.y + t * ab.y};

    return true;
}