#include <dino/dino_entity.h>

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
