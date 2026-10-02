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

bool dino::PointInPolygon(Vec2 point, std::vector<Vec2> polygon_verticies)
{
    if (polygon_verticies.size() < 3)
        return false;

    // Take a really far ray
    Vec2 ray = {-1000, 0};
    unsigned short count = 0;
    for (int i = 0; i < polygon_verticies.size(); i++)
    {
        if (IntersectSegment(ray, point, polygon_verticies[i],
                polygon_verticies[(i + 1) % polygon_verticies.size()]))
            count++;
    }
    return count % 2 == 1;
}