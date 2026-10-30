#include <dino/dino_player.h>
#include <dino/dino_draw_utils.h>
#include <jv/physics/jv_circle_collider.h>

dino::Player::Player(Vec2 pos, int32_t idxPlayer, InputHandler* pInputHandler, jv::gpu::Texture* pTexture, Lasso* pLasso)
    : jv::core::GameObject(pos)
{
    CreateCollider<jv::physics::CircleCollider>(8.0f);
    SetSortingOrder(2);

    m_idxPlayer = idxPlayer;
    m_pInputHandler = pInputHandler;
    m_pTexture = pTexture;
    m_pLasso = pLasso;
}

void dino::Player::Update(double absTime, float deltaTime)
{
    float speed = 100;

    if (m_pInputHandler)
    {
        m_dir = m_pInputHandler->GetDir();
        m_bRunning = m_pInputHandler->GetGamepadState().btn_right; // 'D' sur un clavier

        if (m_bRunning)
            speed *= 2;
    }

    if (m_hitTime > 0) // est ce qu'on est KO ?
    {
        m_hitTime -= deltaTime; // On decremente le temps mis KO
    }
    else
    {
        Translate({m_dir.x * deltaTime * speed, m_dir.y * deltaTime * speed}); // si on est pas KO, on peut se déplacer
    }

    m_absTime = absTime;

    if (m_dir.x != 0)
        m_bLeft = m_dir.x < 0;

    // ENVOI SIMPLEMENT LA POSITION ACTUELLE AU LASSO POUR QU'IL L'ENREGISTRE
    if (m_pLasso)
        m_pLasso->AddPoint(GetPos(), absTime);
}

void dino::Player::Draw() const
{
    float u1, u2, v1 = 0, v2 = 24;

    if (m_hitTime > 0)
    {
        // Degat
        int32_t idxFrame = int32_t(m_absTime * 8) % 3;
        u1 = 336 + 24 * idxFrame;
        u2 = 336 + 24 + 24 * idxFrame;
    }
    else if (m_bRunning)
    {
        // Course
        int32_t idxFrame = int32_t(m_absTime * 16) % 6;
        u1 = 432 + 24 * idxFrame;
        u2 = 432 + 24 + 24 * idxFrame;
    }
    else if (m_dir.x != 0 || m_dir.y != 0)
    {
        // Marche
        int32_t idxFrame = int32_t(m_absTime * 8) % 6;
        u1 = 96 + 24 * idxFrame;
        u2 = 96 + 24 + 24 * idxFrame;
    }
    else
    {
        // Immobile
        int32_t idxFrame = int32_t(m_absTime * 8) % 4;
        u1 = 0 + 24 * idxFrame;
        u2 = 24 + 24 * idxFrame;
    }

    if (m_bLeft)
        std::swap(u1, u2);

    v1 += 24 * m_idxPlayer;
    v2 += 24 * m_idxPlayer;

    Vec2 pos = GetPos();

    std::vector<jv::gpu::Vertex> vs;
    vs.emplace_back(Vec2{pos.x - 16, pos.y - 32}, Vec2{u1, v1});
    vs.emplace_back(Vec2{pos.x + 16, pos.y - 32}, Vec2{u2, v1});
    vs.emplace_back(Vec2{pos.x - 16, pos.y}, Vec2{u1, v2});
    vs.emplace_back(Vec2{pos.x + 16, pos.y - 32}, Vec2{u2, v1});
    vs.emplace_back(Vec2{pos.x - 16, pos.y}, Vec2{u1, v2});
    vs.emplace_back(Vec2{pos.x + 16, pos.y}, Vec2{u2, v2});

    jv::gpu::VertexBuffer* pVBuf = jv::gpu::CreateVertexBuffer("Player", vs);
    jv::gpu::Draw(pVBuf, m_pTexture);
    jv::gpu::DestroyVertexBuffer(pVBuf);
}
