#include <dino/Entity.h>
#include <math.h>

void dino::Entity::ResolveCollision(Entity& a, Entity& b)
{
    // On ne peut pas accéder à m_pos car méthode statique = pas d'état implicite
    // On peut accéder à a.m_pos et b.m_pos car en paramètre, et que la méthode peut accéder à
    // l'état privé

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

void dino::Entity::PushOutOfCircle(Vec2 center, float radius)
{
    float dx = m_pos.x - center.x;
    float dy = m_pos.y - center.y;
    float dist = sqrtf(dx * dx + dy * dy);

    if (dist >= radius)
        return;

    //centre de larbre
    if (dist == 0)
    {
        dx = 0;
        dy = 1;
        dist = 1;
    }

    m_pos.x = center.x + dx / dist * radius;
    m_pos.y = center.y + dy / dist * radius;
}

void dino::Entity::_ReactTerrain()
{
}