#include <dino/dino_animal.h>
#include <dino/dino_draw_utils.h>
#include <jv/physics/jv_circle_collider.h>
#include <jv/core/jv_vfx_system.h>
#include <dino/dino_player.h>
#include <math.h>

dino::Animal::Animal(Vec2 pos, double absTime, jv::gpu::Texture* pTexture)
    : jv::core::GameObject(pos)
{
    m_kind = jv::util::RandomInt32(0, 7);
    m_dir = jv::util::RandomRotate({1, 0}, 0, 360);
    m_timeStart = absTime;

    m_pTexture = pTexture; //injection de dépendance

    CreateCollider<jv::physics::CircleCollider>(8.0f);
    SetSortingOrder(2);
}

void dino::Animal::Update(double absTime, float deltaTime)
{
    float speed = 30;
    Translate({m_dir.x * deltaTime * speed, m_dir.y * deltaTime * speed});

    m_idxFrame = int32_t(absTime * 8) % 4;

    double aliveTime = absTime - m_timeStart;
    if (aliveTime < 1)
        m_alpha = uint8_t(UINT8_MAX * aliveTime);
}

std::vector<jv::gpu::Vertex> dino::Animal::GenerateVertices() const
{
    float u1 = 0.0f, u2 = 32.0f, v1 = 0.0f, v2 = 32.0f;

    // COORDONNEES HORIZONTALES
    if (m_dir.x > 0)
    {
        std::swap(u1, u2);
    }

    u1 += 32.0f * m_idxFrame + 128.0f * m_kind;
    u2 += 32.0f * m_idxFrame + 128.0f * m_kind;

    // COORDONNEES VERTICALES
    if (std::abs(m_dir.y) > std::abs(m_dir.x))
    {
        float pxOffset = (m_dir.y > 0) ? 32.0f : 64.0f;
        v1 += pxOffset;
        v2 += pxOffset;
    }

    jv::util::Color color = Color_WHITE;
    color.a = m_alpha;

    jv::util::Vec2 pos = GetPos();

    std::vector<jv::gpu::Vertex> vs;
    vs.reserve(6);
    vs.emplace_back(jv::util::Vec2{pos.x - 16.0f, pos.y - 32.0f}, jv::util::Vec2{u1, v1}, color);
    vs.emplace_back(jv::util::Vec2{pos.x + 16.0f, pos.y - 32.0f}, jv::util::Vec2{u2, v1}, color);
    vs.emplace_back(jv::util::Vec2{pos.x - 16.0f, pos.y}, jv::util::Vec2{u1, v2}, color);
    vs.emplace_back(jv::util::Vec2{pos.x + 16.0f, pos.y - 32.0f}, jv::util::Vec2{u2, v1}, color);
    vs.emplace_back(jv::util::Vec2{pos.x - 16.0f, pos.y}, jv::util::Vec2{u1, v2}, color);
    vs.emplace_back(jv::util::Vec2{pos.x + 16.0f, pos.y}, jv::util::Vec2{u2, v2}, color);

    return vs;
}

void dino::Animal::Draw() const
{
    std::vector<jv::gpu::Vertex> vs = GenerateVertices();

    jv::gpu::VertexBuffer* pVBuf = jv::gpu::CreateVertexBuffer("Animal", vs);
    jv::gpu::Draw(pVBuf, m_pTexture);
    jv::gpu::DestroyVertexBuffer(pVBuf);
}

void dino::Animal::OnCaughtInLasso(Player* pLassoOwner)
{
    pLassoOwner->AddScore(10); // Ajout de score au joueur qui l'a attrapé

    m_idxFrame = 0;
    m_dir = {0.0f, 1.0f};
    m_alpha = 255;

    // Prend un snapshot de l'animal
    std::vector<jv::gpu::Vertex> vs = GenerateVertices();

    // Affiche un effet de fade out strictement visuel
    jv::core::VfxSystem::SpawnFadeOutGhost(m_pTexture, vs);

    // L'animal est détruit
    Destroy();
}

void dino::Animal::OnSpaceBoundsHit()
{
    m_dir = jv::util::RandomRotate({1, 0}, 0, 360);
}
