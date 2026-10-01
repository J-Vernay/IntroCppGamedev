#include "dino_entity.h"

void dino::Entity::ResolvePhysicConflict(Entity& other)
{
    float dist = sqrt((other.GetX() - GetX()) * (other.GetX() - GetX()) +
                     (other.GetY() - GetY()) * (other.GetY() - GetY()));

    if (dist > PHYSIC_RADIUS) return;

    Vec2 dir = {other.GetX() - GetX(), other.GetY() - GetY()};

    float overlap = PHYSIC_RADIUS - dist;

    SetPos({
        GetX() - dir.x / dist * overlap / 2,
        GetY() - dir.y / dist * overlap / 2});

    other.SetPos({
        other.GetX() + dir.x / dist * overlap / 2,
        other.GetY() + dir.y / dist * overlap / 2});
}

