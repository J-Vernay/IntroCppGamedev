#include <jv/core/jv_gameobject.h>
#include <jv/physics/jv_collider.h>
#include <jv/physics/jv_physics_system.h>
#include <jv/physics/jv_restricted_space.h>

namespace jv::physics
{
void PhysicsSystem::Update(std::vector<std::unique_ptr<jv::core::GameObject>> const& objects,
    IRestrictedSpace const* space)
{
    // CONTRAINTES DE L'ESPACE
    if (space)
    {
        for (auto const& obj : objects)
        {
            if (obj->IsDead() || !obj->IsActive() || !obj->GetCollider()) continue;
            
            obj->ConstrainToBounds(*space);
        }
    }

    // RESOLUTION DES COLLISIONS
    for (size_t i = 0; i < objects.size(); ++i)
    {
        jv::core::GameObject* a = objects[i].get();
        if (a->IsDead() || !a->IsActive() || !a->GetCollider()) // est ce que "a" est affécté par la physique
            continue;

        for (size_t j = i + 1; j < objects.size(); ++j)
        {
            jv::core::GameObject* b = objects[j].get();
            if (b->IsDead() || !b->IsActive() || !b->GetCollider()) // est ce que "b" est affécté par la physique
                continue;
            
            a->GetCollider()->ResolveCollisionWith(b->GetCollider()); // résolution
        }
    }
}

} // namespace jv::physics