#include <dino/dino_entity.h>
#include "dino_geometry.h"

void dino::Entity::HandlePhysics(std::vector<Entity*> entitys)
{
    for (Entity* pEntity : entitys)
    {
        if (pEntity == this) {
            break;
        }

        Vec2 entityPos = pEntity->GetPosition();
        Vec2 posDifference{entityPos.x - m_pos.x, entityPos.y - m_pos.y};
        float L = sqrt((entityPos.x - m_pos.x) * (entityPos.x - m_pos.x) +
                       (entityPos.y - m_pos.y) * (entityPos.y - m_pos.y));
        if (L < 16)
        {

            this->SetPositionCollision(posDifference, L);

            pEntity->SetPositionCollision(Vec2(-posDifference.x, -posDifference.y), L);

        }
    }
}

int dino::Entity::RayCastCollisionCheck(std::vector<Vec2> lassoPoints, int startIndex)
{
    int n = 0;
    Vec2 a = {1000, 0};

    if (lassoPoints.size() < 11)
        return 0;
    for (int i = lassoPoints.size() - startIndex; i < lassoPoints.size() - 10; i++)
    {
        if (dino::IntersectSegment(lassoPoints.at(i), lassoPoints.at(i+1), m_pos, a))
        {
            n++;
        }
    }
    return n;
}
