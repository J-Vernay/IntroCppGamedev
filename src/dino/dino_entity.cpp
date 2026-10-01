#include <dino/dino_entity.h>

void dino::Entity::HandlePhysics(std::vector<Entity*> entitys)
{
    for (Entity* pEntity : entitys)
    {
        Vec2 entityPos = pEntity->GetPosition();
        Vec2 posDifference{entityPos.x - m_pos.x, entityPos.y - m_pos.y};
        float L = abs(posDifference.x) + abs(posDifference.y);
        if (L < 16)
        {
            m_pos.x -= posDifference.x /4;
            m_pos.y -= posDifference.y /4;

            m_dir.x = -posDifference.x;
            m_dir.y = -posDifference.y;
        }
    }
}
