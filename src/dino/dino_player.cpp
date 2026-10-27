#include <dino/dino_draw_utils.h>
#include <dino/dino_player.h>
#include <dino/dino_terrain.h>
#include <dino/dino_geometry.h>
#include <math.h>

dino::Player::Player(
    Vec2 pos, int32_t idxPlayer, jv::input::GamepadIdx gamepadIdx, jv::gpu::Texture* pTexture)
{
    m_pos = pos;
    m_idxPlayer = idxPlayer;
    m_gamepadIdx = gamepadIdx;
    m_pTexture = pTexture;
    id = static_cast<int>(gamepadIdx);

    m_playerColor = m_playersColors[m_idxPlayer % 4];
}


dino::Player::~Player()
{
}

void dino::Player::Update(double absTime, float deltaTime, Terrain const& terrain)
{
    float speed = 100;
    jv::input::Gamepad padInput;

   if (jv::input::GetGamepad(m_gamepadIdx, padInput))
    {
        float dirX = 0, dirY = 0;
        if (padInput.dpad_left)
            dirX -= 1;
        if (padInput.dpad_right)
            dirX += 1;
        if (padInput.dpad_up)
            dirY -= 1;
        if (padInput.dpad_down)
            dirY += 1;

        //sprint

        m_bRunning = padInput.btn_right;

        if (m_bRunning)
            speed *= 2;
        else
            speed /= 2;

        if (padInput.btn_left) // 'A/Q' sur le clavier
            m_hitTime = 3;

        m_dir = {dirX, dirY};
    }

    if (m_hitTime <= 0) // Animation de dégâts fini / pas active
    {
        m_pos.x += m_dir.x * deltaTime * speed;
        m_pos.y += m_dir.y * deltaTime * speed;
    }
    else
    {
        m_hitTime -= deltaTime; // On avance le temps de l'anim
    }

    m_absTime = absTime;

    if (m_dir.x != 0)
        m_bLeft = m_dir.x < 0;

    CheckTerrain(terrain);
    LassoUpdate();
}

void dino::Player::Caught()
{
    if (m_hitTime > 0)
        return;

    m_hitTime = 3;
    m_lasso.clear();
}

void dino::Player::LassoUpdate()
{
    Vec2 anchor = {m_pos.x, m_pos.y - 8};

    // On ajoute un point seulement si on s'est assez éloigné du dernier.
    if (m_lasso.empty())
    {
        m_lasso.push_back(anchor);
        return;
    }

    float dx = anchor.x - m_lasso.back().x;
    float dy = anchor.y - m_lasso.back().y;
    if (sqrtf(dx * dx + dy * dy) >= kLassoMinStep)
        m_lasso.push_back(anchor);

    if (m_lasso.size() > kLassoMaxPoints)
        m_lasso.erase(m_lasso.begin());
}

void dino::Player::LassoDraw() const
{
    if (m_lasso.empty())
        return;

    // Points fixes + position actuelle, pour que le lasso colle toujours au joueur.
    std::vector<Vec2> pts = m_lasso;
    pts.push_back({m_pos.x, m_pos.y - 8});

    std::vector<jv::gpu::Vertex> vs;
    float h = kLassoWidth / 2;

    for (size_t i = 0; i + 1 < pts.size(); ++i)
    {
        Vec2 a = pts[i];
        Vec2 b = pts[i + 1];

        float dx = b.x - a.x;
        float dy = b.y - a.y;
        float len = sqrtf(dx * dx + dy * dy);
        if (len == 0)
            continue;

        // Normale du segment, de longueur h.
        float nx = -dy / len * h;
        float ny = dx / len * h;

        Vec2 a1 = {a.x + nx, a.y + ny};
        Vec2 a2 = {a.x - nx, a.y - ny};
        Vec2 b1 = {b.x + nx, b.y + ny};
        Vec2 b2 = {b.x - nx, b.y - ny};

        // Un quad par segment = 2 triangles.
        vs.push_back({a1, Vec2{0, 0}, m_playerColor});
        vs.push_back({a2, Vec2{0, 0}, m_playerColor});
        vs.push_back({b1, Vec2{0, 0}, m_playerColor});

        vs.push_back({a2, Vec2{0, 0}, m_playerColor});
        vs.push_back({b2, Vec2{0, 0}, m_playerColor});
        vs.push_back({b1, Vec2{0, 0}, m_playerColor});
    }

    if (vs.empty())
        return;

    jv::gpu::VertexBuffer* pVBuf = jv::gpu::CreateVertexBuffer("Lasso", vs);
    jv::gpu::Draw(pVBuf, nullptr);
    jv::gpu::DestroyVertexBuffer(pVBuf);
}

bool dino::Player::FindLassoLoop(std::vector<Vec2>& outLoop) const
{
    std::vector<Vec2> pts = m_lasso;
    pts.push_back({m_pos.x, m_pos.y - 8});

    size_t n = pts.size();
    if (n < 4)
        return false;

    for (size_t s = n - 1; s + 2 >= n && s >= 1; --s)
    {
        Vec2 c = pts[s - 1];
        Vec2 d = pts[s];
        if (c.x == d.x && c.y == d.y)
            continue;

        for (size_t i = 0; i + 2 < s; ++i)
        {
            if (IntersectSegment(pts[i], pts[i + 1], c, d))
            {
                outLoop.assign(pts.begin() + i + 1, pts.begin() + s + 1);
                return outLoop.size() >= 3;
            }
        }
    }
    return false;
}

void dino::Player::Draw() const
{
    LassoDraw();
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

#if 0

    v1 = 0, v2 = 24;

    u1 += 32 * m_idxFrame + 128 * m_kind;
    u2 += 32 * m_idxFrame + 128 * m_kind;

#endif

    jv::util::Color color = Color_WHITE;
    color.a = 255;

    v1 += 24 * m_idxPlayer;
    v2 += 24 * m_idxPlayer;

    std::vector<jv::gpu::Vertex> vs;
    vs.emplace_back(Vec2{m_pos.x - 16, m_pos.y - 32}, Vec2{u1, v1});
    vs.emplace_back(Vec2{m_pos.x + 16, m_pos.y - 32}, Vec2{u2, v1});
    vs.emplace_back(Vec2{m_pos.x - 16, m_pos.y}, Vec2{u1, v2});
    vs.emplace_back(Vec2{m_pos.x + 16, m_pos.y - 32}, Vec2{u2, v1});
    vs.emplace_back(Vec2{m_pos.x - 16, m_pos.y}, Vec2{u1, v2});
    vs.emplace_back(Vec2{m_pos.x + 16, m_pos.y}, Vec2{u2, v2});

    jv::gpu::VertexBuffer* pVBuf = jv::gpu::CreateVertexBuffer("Player", vs);
    jv::gpu::Draw(pVBuf, m_pTexture);
    jv::gpu::DestroyVertexBuffer(pVBuf);
}