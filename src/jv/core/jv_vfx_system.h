#pragma once
#include <jv/jv.h>
#include <vector>

namespace jv::core
{

struct GhostVFX
{
    jv::gpu::Texture* pTexture;
    std::vector<jv::gpu::Vertex> vertices;
    float alpha;
};

class VfxSystem
{
public:
    VfxSystem() = delete; // classe statique

    static void SpawnFadeOutGhost( jv::gpu::Texture* pTexture, std::vector<jv::gpu::Vertex> const& vertices);
    static void Update(float deltaTime);
    static void Draw();

private:
    static std::vector<GhostVFX> ghosts;
};

} // namespace jv::core