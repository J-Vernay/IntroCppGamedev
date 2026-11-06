#pragma once

#include <dino/dino_main.h>

namespace dino
{

/// Représente le terrain de jeu.
class Terrain
{
public:
    Terrain(int32_t tileCountX, int32_t tileCountY);

    void SetSeason(int32_t idxSeason);
    void Update(double absTime, float deltaTime);
    void Draw() const;
    void DrawSeasonTrees(bool dimmed) const;

    Vec2 GetSeasonTreeCenter(int32_t idxSeason) const;
    Vec2 GenerateRandomSpawn() const;
    Vec2 ClampPos(Vec2 pos) const;

    ~Terrain();

private:
    Vec2 m_spawnOffset;
    Vec2 m_spawnSize;
    int32_t m_idxSeason = 0;
    int32_t m_idxFrame = 0;
    size_t m_idxVertexOceanEnd = 0;
    size_t m_idxVertexNonAnimatedEnd = 0;

    jv::gpu::Texture* m_pTexture = nullptr;
    std::vector<jv::gpu::Vertex> m_vertices;
};

} // namespace dino