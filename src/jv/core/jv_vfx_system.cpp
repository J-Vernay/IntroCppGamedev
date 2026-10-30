#include <algorithm>
#include <jv/core/jv_vfx_system.h>

namespace jv::core
{
std::vector<GhostVFX> VfxSystem::ghosts;

void VfxSystem::SpawnFadeOutGhost(
    jv::gpu::Texture* pTexture, std::vector<jv::gpu::Vertex> const& vertices)
{
    ghosts.push_back({pTexture, vertices, 255.0f});
}

void VfxSystem::Update(float deltaTime)
{
    float alphaDrop = 255.0f * deltaTime;

    for (GhostVFX& ghost : ghosts)
    {
        ghost.alpha -= alphaDrop;
        uint8_t currentAlpha = ghost.alpha > 0 ? static_cast<uint8_t>(ghost.alpha) : 0;

        for (jv::gpu::Vertex& vertex : ghost.vertices)
        {
            vertex.color.a = currentAlpha;
        }
    }

    // Ceux qui ont fini de fade out sont détruits
    std::erase_if(ghosts, [](GhostVFX const& ghost) { return ghost.alpha <= 0; });
}

void VfxSystem::Draw()
{
    for (GhostVFX const& ghost : ghosts)
    {
        jv::gpu::VertexBuffer* pVBuf = jv::gpu::CreateVertexBuffer("VFX_Ghost", ghost.vertices);
        jv::gpu::Draw(pVBuf, ghost.pTexture);
        jv::gpu::DestroyVertexBuffer(pVBuf);
    }
}

} // namespace jv::core