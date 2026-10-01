#pragma once

#include <dino/dino_main.h>

namespace dino
{

/// Représente le terrain de jeu.
class Terrain
{
public:
    /// Initialise le terrain.
    Terrain(int32_t tileCountX, int32_t tileCountY);
    
    /// Change la saison du terrain (0 à 3 : printemps, été, automne, hiver)
    void SetSeason(int32_t idxSeason);

    /// Met à jour le terrain (déroulement de l'animation).
    void Update(double absTime, float deltaTime);
    
    /// Affiche le terrain de jeu.
    void Draw() const;

    /// Génère une position aléatoire dans le terrain.
    Vec2 GenerateRandomSpawn() const;

    /// Transforme la position 'pos' pour s'assurer qu'elle reste sur le terrain.
    Vec2 ClampPos(Vec2 pos) const;

    bool IsInside(Vec2 pos) const;

    /// Libère les ressources.
    ~Terrain();

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