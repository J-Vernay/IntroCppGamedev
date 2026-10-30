#include <algorithm>
#include <jv/core/jv_scene.h>
#include <jv/physics/jv_physics_system.h>
#include <jv/core/jv_vfx_system.h>

namespace jv::core
{

void Scene::Update(double absTime, float deltaTime)
{
    for (auto const& obj : m_objects)
    {
        if (obj->IsDead() || !obj->IsActive()) continue;
        
        obj->Update(absTime, deltaTime);
    }

    VfxSystem::Update(deltaTime); // met à jour la logique du systeme de VFX

    // Nettoyage automatique des objets marqués comme morts
    std::erase_if(m_objects, [](std::unique_ptr<GameObject> const& obj) { return obj->IsDead(); });
}

void Scene::PhysicsUpdate()
{
    // Délégue la physique au système dédié du moteur
    jv::physics::PhysicsSystem::Update(m_objects, m_pSpace);
}

void Scene::Draw() const
{
    // Conteneur temporaire de pointeurs pour le tri
    std::vector<GameObject*> sortedObjects;
    sortedObjects.reserve(m_objects.size());

    for (auto const& obj : m_objects)
    {
        if (obj->IsDead() || !obj->IsActive()) continue;
        
        sortedObjects.push_back(obj.get());
    }

    // Tri selon le calque et la profondeur
    std::sort(sortedObjects.begin(), sortedObjects.end(), GameObject::RenderSort);

    // Appels triés
    for (GameObject const* obj : sortedObjects)
    {
        obj->Draw();
    }

    VfxSystem::Draw(); // Dessine les VFX par dessus le reste
}

} // namespace jv::core