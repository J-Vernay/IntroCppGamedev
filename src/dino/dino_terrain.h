#pragma once

#include <dino/dino_main.h>
#include <jv/core/jv_gameobject.h>
#include <jv/physics/jv_restricted_space.h>

namespace dino
{

/// Représente le terrain de jeu.
class Terrain : public jv::core::GameObject, public jv::physics::IRestrictedSpace
{
public:
    /// Initialise le terrain.
    Terrain(int32_t tileCountX, int32_t tileCountY, jv::gpu::Texture* pTexture);
    
    /// Change la saison du terrain (0 à 3 : printemps, été, automne, hiver)
    void SetSeason(int32_t idxSeason);

    // GAMEOBJECT
    /// Implémente la boucle logique d'un GameObject
    void Update(double absTime, float deltaTime) override;
    /// Implémente la boucle de rendu d'un GameObject
    void Draw() const override;

    // Implémente sa propre définition d'un espace restreint
    Vec2 GenerateRandomSpawn() const override;
    Vec2 ClampPos(Vec2 pos) const override;

    /// Libère les ressources.
    ~Terrain() override;

private:
    // Position in pixels
    Vec2 m_spawnOffset;
    Vec2 m_spawnSize;
    int32_t m_idxSeason = 0;
    int32_t m_idxFrame = 0;
    size_t m_idxVertexOceanEnd;
    size_t m_idxVertexNonAnimatedEnd;

    jv::gpu::Texture* m_pTexture = nullptr;
    std::vector<jv::gpu::Vertex> m_vertices;
};

} // namespace dino