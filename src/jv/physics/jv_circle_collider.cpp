#include <jv/physics/jv_circle_collider.h>
#include <jv/core/jv_gameobject.h>
#include <math.h>

namespace jv::physics
{

CircleCollider::CircleCollider(GameObject* owner, float radius, Vec2 offset)
    : Collider(owner, offset), m_radius(radius)
{
}

void CircleCollider::ResolveCollision(CircleCollider* other)
{
    Vec2 centerA = this->GetAbsoluteCenter();
    Vec2 centerB = other->GetAbsoluteCenter();

    // Calcul du vecteur directeur (A,B) et la distance entre les centres
    Vec2 vecAB = {centerB.x - centerA.x, centerB.y - centerA.y};
    float distance = sqrtf(vecAB.x * vecAB.x + vecAB.y * vecAB.y);

    // Calcul la distance de collision minimale
    float sumRadius = this->GetRadius() + other->GetRadius();

    if (distance > 0 && distance < sumRadius)
    {
        // P = (R1+R2) - distance(C1,C2)
        float penetration = sumRadius - distance;

        // vecteur directeur (C1,C2) normalisé en le divisant par sa longueur
        Vec2 direction = {vecAB.x / distance, vecAB.y / distance};

        // Vecteur de déplacement final
        Vec2 depl = {direction.x * (penetration / 2.0f), direction.y * (penetration / 2.0f)};

        // Récupération des propriétaires (GameObjects)
        GameObject* objA = this->m_owner;
        GameObject* objB = other->m_owner;

        // Répulsion de B dans la direction de (A,B)
        objB->Translate(depl);

        // Répulsion de A dans la direction de (B,A)
        objA->Translate({-depl.x, -depl.y});
    }
}
} // namespace jv::physics