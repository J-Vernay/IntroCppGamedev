#include "Movable.h"

void dino::Movable::ResolveCollision(Movable& a, Movable& b)
{
    Vec2 dist{ b.m_pos.x - a.m_pos.x, b.m_pos.y - a.m_pos.y };
    float distnorm = sqrtf(dist.x * dist.x + dist.y * dist.y);

    if (distnorm > 0 && distnorm < 16)
    {
        float factor = (16 - distnorm) / (2 * distnorm);
        Vec2 depl{ dist.x * factor, dist.y * factor };
        a.m_pos.x -= depl.x;
        a.m_pos.y -= depl.y;
        b.m_pos.x += depl.x;
        b.m_pos.y += depl.y;
    }
}

bool dino::Movable::OrderByPosY(Movable const* a, Movable const* b)
{
    return a->m_pos.y < b->m_pos.y;
}

void dino::Movable::CheckTerrain(Terrain const& m_Terrain)
{
    Vec2 oldPos = m_pos;
    m_pos = m_Terrain.ClampPos(m_pos);
    if (m_pos.x != oldPos.x || m_pos.y != oldPos.y)
        _ReactTerrain();
}

void dino::Movable::_ReactTerrain()
{

}
