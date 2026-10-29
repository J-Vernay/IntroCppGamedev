#include <dino/dino_draw_utils.h>
#include <dino/Entity.h>


// Sépare deux entités qui se chevauchent.
void dino::Entity::ResolveEntityCollision(Entity& a, Entity& b)
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
// Contraint l'entité aux limites du terrain.
void dino::Entity::CheckTerrain(Terrain const& terrain)
{
    Vec2 oldPos = m_pos;
    m_pos = terrain.ClampPos(m_pos);
    if (m_pos.x != oldPos.x || m_pos.y != oldPos.y)
        _ReactTerrain();
}

// comportement par défaut au contact du terrain
void dino::Entity::_ReactTerrain()
{
}
// Compare deux entités selon leur position verticale.
bool dino::Entity::OrderByPosY(Entity const* a, Entity const* b)
{
    return a->m_pos.y < b->m_pos.y;
}
