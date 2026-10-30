#pragma once

#include <dino/dino_main.h>
#include <jv/core/jv_gameobject.h>
#include <dino/dino_lasso_target.h>

namespace dino
{
class Tree : public jv::core::GameObject, public ILassoTarget
{
public:
    const int32_t SeasonIdx;

    Tree(Vec2 pos, int32_t seasonIdx, jv::gpu::Texture* pTexture);

    /// Constructeur par copie désactivé : application de la sémantique d'entitée
    Tree(Tree const& other) = delete;

    /// Opérateur d'assignation par copie désactivé : application de la sémantique d'entitée
    Tree& operator=(Tree const& b) = delete;

    ~Tree() = default;

    void Update(double absTime, float deltaTime) override;

    void Draw() const override;

    void OnCaughtInLasso(Player* pLassoOwner) override
    {
        if (m_disableSelectionTimer > 0) return;
        m_isSelected = true; // cet arbre est selectionné pour lancer le jeu
    }

    bool IsSelected() const { return m_isSelected; }

    void ResetState() { m_isSelected = false; }

    void DisableSelectionDuring(float duration) { m_disableSelectionTimer = duration; }

private:
    bool m_isSelected = false;
    float m_disableSelectionTimer = 0.0f;
    jv::gpu::Texture* m_pTexture;
};

} // namespace dino