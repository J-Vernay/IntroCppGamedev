#include <dino/Entity.h>
#include "dino_terrain.h"


void dino::Entity::ResolveCollision(Entity& a, Entity& b)
{
    Vec2 dist{b.m_pos.x - a.m_pos.x, b.m_pos.y - a.m_pos.y};
    float distnorm = sqrtf(dist.x * dist.x + dist.y * dist.y);

    if (distnorm > 0 && distnorm < 16)
    {
        float factor = (16 - distnorm) / (2 * distnorm);
        Vec2 depl{dist.x * factor, dist.y * factor};
        a.m_pos.x -= depl.x;
        a.m_pos.y -= depl.y;
        b.m_pos.x += depl.x;
        b.m_pos.y += depl.y;
    }
}

void dino::Entity::CheckTerrain(Terrain const& terrain)
{
    Vec2 oldPos = m_pos;
    m_pos = terrain.ClampPos(m_pos);
    if (m_pos.x != oldPos.x || m_pos.y != oldPos.y)
        _ReactTerrain();
}

void dino::Entity::_ReactTerrain()
{
}

bool dino::Entity::OrderByPosY(Entity const* a, Entity const* b)
{
    return a->m_pos.y < b->m_pos.y;
}