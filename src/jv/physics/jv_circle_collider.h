#pragma once
#include <jv/physics/jv_collider.h>

namespace jv::physics
{
using GameObject = jv::core::GameObject;

class CircleCollider : public Collider
{
public:
    CircleCollider(GameObject* owner, float radius, Vec2 offset = {0, 0});

    void ResolveCollisionWith(Collider* other) override
    {
        other->ResolveCollision(this); // on passe la main à l'autre collider en lui précisant notre propre type
    }

    /// Resolution : cercle <-> cercle
    void ResolveCollision(CircleCollider* other) override;

    /// On pourrait ajouter une fonction de résolution pour cercle <-> box ou box <-> box avec ce
    /// systeme

    float GetRadius() const
    {
        return m_radius;
    }

private:
    float m_radius;
};

} // namespace jv::physics