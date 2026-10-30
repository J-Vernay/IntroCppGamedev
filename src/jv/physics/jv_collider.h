#pragma once
#include "jv/jv.h"
#include <jv/core/jv_gameobject.h>

// Forward declaration
namespace jv::core { class GameObject; }

namespace jv::physics
{
    using GameObject = jv::core::GameObject;
    using Vec2 = jv::util::Vec2;

// Forward declaration des differents types de colliders
class CircleCollider;

class Collider
{
public:
    // le collider est lié à son propriétaire à la création
    GameObject* const m_owner;

    Collider(GameObject* owner, Vec2 offset = {0, 0}) : m_owner(owner), m_offset(offset)
    {
    }
    virtual ~Collider() = default;

    Vec2 GetAbsoluteCenter() const
    {
        return {m_owner->GetPos().x + m_offset.x, m_owner->GetPos().y + m_offset.y};
    }

    // Utilisation du double dispatch pour connaitre le type concret des deux colliders, sans
    // utiliser de cast explicite : On va demander au Collider1 de résoudre avec le Collider2 dont
    // il ne connait pas le type final (il ne peut pas le faire) Collider1 va déléguer cette
    // résolution à Collider2 MAIS en lui donnant son propre type final Collider2 connait son propre
    // type et Collider1 lui a donné le siens. Il sait donc quelle version de la résolution utiliser

    /// Premier dispatch (version abstraite)
    virtual void ResolveCollisionWith(Collider* other) = 0;

    /// Resolution concrete de la collision (ici entre deux CircleCollider)
    virtual void ResolveCollision(CircleCollider* other) = 0;

private:
    Vec2 m_offset;
};

} // namespace jv::physics