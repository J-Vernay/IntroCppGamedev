#include <dino/dino_geometry.h>

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

bool dino::IsPointInPolygon(Vec2 point, std::vector<Vec2> const& polygon)
{
    if (polygon.size() < 3)
        return false;

    Vec2 rayEnd = {point.x + 10000.0f, point.y + 0.001f};
    int intersections = 0;
    size_t n = polygon.size();

    for (size_t i = 0; i < n; ++i)
    {
        Vec2 A = polygon[i];
        Vec2 B = polygon[(i + 1) % n];

        if (IntersectSegment(point, rayEnd, A, B))
        {
            intersections++;
        }
    }

    // Impair = à l'intérieur / Pair = à l'exterieur
    return (intersections % 2) != 0;
}
