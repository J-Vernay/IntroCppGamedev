#pragma once
#include <memory>
#include <jv/jv.h>

// Forward declaration
namespace jv::physics
{
class Collider;
class IRestrictedSpace;
} // namespace jv::physics

namespace jv::core
{
using Vec2 = jv::util::Vec2;
using IRestrictedSpace = jv::physics::IRestrictedSpace;
using Collider = jv::physics::Collider;

class GameObject
{
public:
    GameObject(Vec2 startPos);
    virtual ~GameObject();

    /// boucle logique de l'objet
    virtual void Update(double absTime, float deltaTime) = 0;
    /// boucle de rendu de l'objet
    virtual void Draw() const = 0;

    /// Tri les objets selon leur ordre de sorti ou, s'il est le meme, selon leur position en Y
    static bool RenderSort(GameObject const* a, GameObject const* b);

    /// S'assurer que la position de l'objet reste dans l'espace où il évolue
    void ConstrainToBounds(IRestrictedSpace const& space);

    Vec2 GetPos() const { return m_pos; }
    void Translate(Vec2 delta);

    /// Marque cet objet et tous ses enfants comme morts
    void Destroy();
    bool IsDead() const { return m_isDead; }

    Collider* GetCollider() const { return m_collider.get(); }

    /// Permet de définir le layer de rendu
    void SetSortingOrder(int order) { m_sortingOrder = order; }

    /// Systeme servant à desactiver puis réactiver un objet proprement. Sans devoir le détruire et le recréer.
    bool IsActive() const { return m_isActive; }
    void SetActive(bool active);

    /// Ajoute un enfant à ce GameObject
    void AddChild(GameObject* pChild);

    /// Récupère l'objet parent (nullptr si c'est un objet racine)
    GameObject* GetParent() const { return m_pParent; }

protected:
    /// Permet aux classes dérivées de créer et attacher un collider à cet objet
    template <typename T, typename... Args> 
    T* CreateCollider(Args&&... args);

    virtual void OnSpaceBoundsHit();

private:
    Vec2 m_pos;
    bool m_isDead = false;
    bool m_isActive = true;
    int m_sortingOrder = 0;
    std::unique_ptr<Collider> m_collider = nullptr;
    GameObject* m_pParent = nullptr;
    std::vector<GameObject*> m_children;
};

template <typename T, typename... Args> 
T* GameObject::CreateCollider(Args&&... args)
{
    if (!m_collider)
    {
        auto collider = std::make_unique<T>(this, std::forward<Args>(args)...);
        T* rawPtr = collider.get();
        m_collider = std::move(collider);
        return rawPtr;
    }
    return nullptr;
}

} // namespace jv::core