#include <dino/dino_entity.h>
#include <dino/dino_terrain.h>

dino::Entity::Entity(Vec2 pos, float radius) : m_pos{pos}, m_radius{radius}
{
}

dino::Vec2 dino::Entity::GetPosition() const
{
    return m_pos;
}

void dino::Entity::SetPosition(Vec2 pos)
{
    m_pos = pos;
}

float dino::Entity::GetRadius() const
{
    return m_radius;
}

void dino::Entity::ClampToTerrain(const Terrain& terrain)
{
    Vec2 correctedPos = terrain.ClampPos(m_pos);

    Vec2 correction{correctedPos.x - m_pos.x, correctedPos.y - m_pos.y};

    if (correction.x == 0.f && correction.y == 0.f)
        return;

    m_pos = correctedPos;

    OnTerrainCollision(correction);
}

void dino::Entity::OnTerrainCollision(Vec2)
{

}