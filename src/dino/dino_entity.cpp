#include <dino/dino_entity.h>

dino::Entity::Entity(Terrain* terrain, Vec2 pos)
{
    m_terrain = terrain;
    m_pos = pos;
    m_lastPos = pos;
}

void dino::Entity::Collide(Entity& other)
{
    Vec2 otherPos = other.GetPos();

    float sqrDistance = abs(m_pos.x - otherPos.x) * abs(m_pos.x - otherPos.x) +
                        abs(m_pos.y - otherPos.y) * abs(m_pos.y - otherPos.y);
    if (sqrDistance != 0 && sqrDistance < 16 * 16)
    {
        float distance = sqrt(sqrDistance);
        float collisionDistance = 16 - distance;
        float collisionFraction = (collisionDistance / distance / 2);
        Vec2 direction = Vec2{
            (m_pos.x - otherPos.x) * collisionFraction, (m_pos.y - otherPos.y) * collisionFraction};
        Vec2 opposite = Vec2{-direction.x, -direction.y};

        Move(direction);
        other.Move(opposite);
    }
}

void dino::Entity::Move(Vec2 const amount)
{
    m_lastPos = m_pos;
    m_pos = Vec2{m_pos.x + amount.x, m_pos.y + amount.y};

    Vec2 clampedPos = m_terrain->ClampPos(m_pos);

    if (m_pos.x != clampedPos.x || m_pos.y != clampedPos.y)
    {
        m_pos = clampedPos;
        OnOutsideTerrain();
    }
}

jv::util::Vec2 dino::Entity::GetPos() const
{
    return m_pos;
}

jv::util::Vec2 dino::Entity::GetLastPos() const
{
    return m_lastPos;
}
