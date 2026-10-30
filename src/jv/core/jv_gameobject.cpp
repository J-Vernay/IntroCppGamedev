#include <jv/core/jv_gameobject.h>
#include <jv/physics/jv_collider.h>
#include <jv/physics/jv_restricted_space.h>

namespace jv::core
{

GameObject::GameObject(Vec2 startPos)
{
    m_pos = startPos;
}

GameObject::~GameObject() = default;

bool GameObject::RenderSort(GameObject const* a, GameObject const* b)
{
    // Selon le Sorting Order
    if (a->m_sortingOrder != b->m_sortingOrder)
    {
        return a->m_sortingOrder < b->m_sortingOrder;
    }

    // Selon Y pour les objets sur le même calque
    return a->GetPos().y < b->GetPos().y;
}

void GameObject::ConstrainToBounds(IRestrictedSpace const& space)
{
    Vec2 oldPos = m_pos;
    m_pos = space.ClampPos(m_pos);
    if (m_pos.x != oldPos.x || m_pos.y != oldPos.y)
    {
        OnSpaceBoundsHit();
    }
}

void GameObject::Translate(Vec2 delta)
{
    m_pos.x += delta.x;
    m_pos.y += delta.y;
}

void GameObject::Destroy()
{
    m_isDead = true;
    for (GameObject* pChild : m_children)
    {
        if (pChild)
            pChild->Destroy();
    }
}

void GameObject::SetActive(bool active)
{
    m_isActive = active;
    for (GameObject* pChild : m_children)
    {
        if (pChild)
            pChild->SetActive(active);
    }
}

void GameObject::AddChild(GameObject* pChild)
{
    if (pChild)
    {
        m_children.push_back(pChild);
        pChild->m_pParent = this;
    }
}

void GameObject::OnSpaceBoundsHit()
{
}

} // namespace jv::core