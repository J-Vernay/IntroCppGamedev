#pragma once

#include <jv/core/jv_gameobject.h>
#include <dino/dino_lasso_target.h>
#include <dino/dino_main.h>

namespace dino
{

class Animal : public jv::core::GameObject, public ILassoTarget
{
public:
    /// Initialise l'animal avec un type au hasard
    Animal(Vec2 pos, double absTime, jv::gpu::Texture* pTexture);

    /// Constructeur par copie désactivé : application de la sémantique d'entitée
    Animal(Animal const& other) = delete;

    /// Opérateur d'assignation par copie désactivé : application de la sémantique d'entitée
    Animal& operator=(Animal const& b) = delete;

    ~Animal() override = default;

    /// Déplace l'animal
    void Update(double absTime, float deltaTime) override;

    /// Anime et affiche l'animal
    void Draw() const override;

    void OnCaughtInLasso(Player* pLassoOwner) override;

private:
    double m_timeStart;
    uint8_t m_alpha = 0;
    Vec2 m_dir;
    int32_t m_kind;
    int32_t m_idxFrame = 0;
    jv::gpu::Texture* m_pTexture;

    void OnSpaceBoundsHit() override;

    std::vector<jv::gpu::Vertex> GenerateVertices() const;
};

} // namespace dino