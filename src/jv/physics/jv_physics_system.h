#pragma once
#include <memory>
#include <vector>

// Forward declarations
namespace jv::core { class GameObject; }
namespace jv::physics { class IRestrictedSpace; }

namespace jv::physics
{
using GameObjectPtr = std::unique_ptr<jv::core::GameObject>;

class PhysicsSystem
{
public:
    PhysicsSystem() = delete; // Classe statique

    /// Résout les collisions entre les objets et applique les contraintes de l'espace dans lequel ils évoluent
    static void Update(std::vector<GameObjectPtr> const& objects,
        IRestrictedSpace const* space);
};
} // namespace jv::physics