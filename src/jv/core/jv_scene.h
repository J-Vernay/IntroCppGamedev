#pragma once

#include <jv/core/jv_gameobject.h>
#include <jv/physics/jv_restricted_space.h>
#include <memory>
#include <vector>

namespace jv::core
{

class Scene
{
public:
    Scene() = default;
    ~Scene() = default;
    Scene(Scene const&) = delete;
    Scene& operator=(Scene const&) = delete;

    /// Permet de créer nimporte quel gameobject dont le propriétaire sera la scène, renvoi un rawPtr
    template <typename T, typename... Args> 
    T* CreateObject(Args&&... args);

    /// Définit l'espace dans lequel les objets de la scène évoluent
    void SetRestrictedSpace(jv::physics::IRestrictedSpace const* space) { m_pSpace = space; }

    /// Met à jour la logique interne de tous les objets et nettoie les objets détruits (+ logique des vfx)
    void Update(double absTime, float deltaTime);

    /// Résolution physique
    void PhysicsUpdate();

    /// Trie et dessine l'ensemble des objets (+ les vfx)
    void Draw() const;

    /// Accès non propriétaire en lecture seule des objets de la scène
    std::vector<std::unique_ptr<GameObject>> const& GetObjects() const { return m_objects; }

private:
    jv::physics::IRestrictedSpace const* m_pSpace = nullptr;
    std::vector<std::unique_ptr<GameObject>> m_objects;
};

template <typename T, typename... Args> 
T* Scene::CreateObject(Args&&... args)
{
    auto obj = std::make_unique<T>(std::forward<Args>(args)...);
    T* rawPtr = obj.get();
    m_objects.push_back(std::move(obj));
    return rawPtr;
}

} // namespace jv::core