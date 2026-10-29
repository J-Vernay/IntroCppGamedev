#include <dino/dino_draw_utils.h>
#include <dino/dino_player.h>

#include <algorithm>
#include <cmath>

namespace
{

// Calcule le produit de deux vecteurs
float Cross(dino::Vec2 a, dino::Vec2 b)
{
    return a.x * b.y - a.y * b.x;
}

// cherche l'intersection entre deux segments 
bool FindSegmentIntersection(dino::Vec2 a, dino::Vec2 b, dino::Vec2 c, dino::Vec2 d, dino::Vec2& intersection)
{
    dino::Vec2 r{b.x - a.x, b.y - a.y};
    dino::Vec2 s{d.x - c.x, d.y - c.y};
    float denominator = Cross(r, s);

    if (std::abs(denominator) < 0.0001f)
        return false;

    dino::Vec2 cMinusA{c.x - a.x, c.y - a.y};
    float t = Cross(cMinusA, s) / denominator;
    float u = Cross(cMinusA, r) / denominator;

    if (t < 0 || t > 1 || u < 0 || u > 1)
        return false;

    intersection = {a.x + t * r.x, a.y + t * r.y};
    return true;
}

// calcule l'aire d'un polygone
float GetPolygonArea(std::vector<dino::Vec2> const& polygon)
{
    float area = 0;

    for (size_t i = 0; i < polygon.size(); ++i)
    {
        dino::Vec2 const& a = polygon[i];
        dino::Vec2 const& b = polygon[(i + 1) % polygon.size()];
        area += a.x * b.y - b.x * a.y;
    }

    return std::abs(area) * 0.5f;
}

} // namespace

// init un joueur 
dino::Player::Player(
    Vec2 pos, int32_t idxPlayer, jv::input::GamepadIdx gamepadIdx, jv::gpu::Texture* pTexture)
{
    m_pos = pos;
    m_idxPlayer = idxPlayer;
    m_gamepadIdx = gamepadIdx;
    m_pTexture = pTexture;
}

dino::Player::~Player() = default;
// Connecte le joueur à la partie
void dino::Player::Join(Vec2 pos)
{
    m_pos = pos;
    m_dir = {};
    m_hitTime = 0;
    m_isConnected = true;
    ClearLasso();
}

// Déconnecte le joueur et efface son lasso
void dino::Player::Leave()
{
    m_isConnected = false;
    m_dir = {};
    m_bRunning = false;
    ClearLasso();
}

// efface le tracé du lasso et toute boucle terminée
void dino::Player::ClearLasso()
{
    m_lassoPoints.clear();
    m_lassoTimes.clear();
    m_completedLoop.clear();
}

//Récupère la boucle de lasso terminée
std::vector<dino::Vec2> dino::Player::TakeCompletedLoop()
{
    std::vector<Vec2> loop;
    loop.swap(m_completedLoop);
    return loop;
}

void dino::Player::Update(double absTime, float deltaTime)
{
    if (!m_isConnected)
        return;

    float speed = 100;
    jv::input::Gamepad gamepad{};

    if (jv::input::GetGamepad(m_gamepadIdx, gamepad))
    {
        float dirx = 0;
        float diry = 0;

        if (gamepad.dpad_up)
            diry -= 1;
        if (gamepad.dpad_down)
            diry += 1;
        if (gamepad.dpad_left)
            dirx -= 1;
        if (gamepad.dpad_right)
            dirx += 1;
        
        m_dir = {dirx, diry};
        m_bRunning = gamepad.btn_right;

        m_bRunning = keyboard.btn_right;
        if (m_bRunning)
            speed *= 2;

        // Conservation du comportement du projet : Q/A déclenche les dégâts.
        if (gamepad.btn_left)
            m_hitTime = 3;
    }
    else
    {
        m_dir = {};
        m_bRunning = false;
    }

    if (m_hitTime <= 0)
    {
        m_pos.x += m_dir.x * deltaTime * speed;
        m_pos.y += m_dir.y * deltaTime * speed;
    }
    else
    {
        m_hitTime = std::max(0.0, m_hitTime - deltaTime);
    }

    m_absTime = absTime;

    if (m_dir.x != 0)
        m_goingLeft = m_dir.x < 0;

    _UpdateLasso(absTime);
}

// Met à jour le tracé et détecte la fermeture d'une boucle
void dino::Player::_UpdateLasso(double absTime)
{
    constexpr float MAX_LASSO_LENGTH = 240;

    if (!m_completedLoop.empty())
        return;

    bool shouldAddPoint = m_lassoPoints.empty();

    if (!m_lassoPoints.empty())
    {
        Vec2 const& last = m_lassoPoints.back();
        float dx = m_pos.x - last.x;
        float dy = m_pos.y - last.y;
        shouldAddPoint = dx * dx + dy * dy >= 16;
    }

    if (shouldAddPoint)
    {
        m_lassoPoints.push_back(m_pos);
        m_lassoTimes.push_back(absTime);
    }

    float lassoLength = 0;
    for (size_t i = 1; i < m_lassoPoints.size(); ++i)
    {
        float dx = m_lassoPoints[i].x - m_lassoPoints[i - 1].x;
        float dy = m_lassoPoints[i].y - m_lassoPoints[i - 1].y;
        lassoLength += std::hypot(dx, dy);
    }

    while (m_lassoPoints.size() > 2 && lassoLength > MAX_LASSO_LENGTH)
    {
        float dx = m_lassoPoints[1].x - m_lassoPoints[0].x;
        float dy = m_lassoPoints[1].y - m_lassoPoints[0].y;
        lassoLength -= std::hypot(dx, dy);

        m_lassoPoints.erase(m_lassoPoints.begin());
        m_lassoTimes.erase(m_lassoTimes.begin());
    }

    size_t pointCount = m_lassoPoints.size();
    if (pointCount < 4)
        return;

    size_t lastSegmentStart = pointCount - 2;

    for (size_t i = 0; i + 2 < lastSegmentStart; ++i)
    {
        Vec2 intersection{};

        if (!FindSegmentIntersection(m_lassoPoints[i], m_lassoPoints[i + 1],
                m_lassoPoints[lastSegmentStart], m_lassoPoints[lastSegmentStart + 1], intersection))
        {
            continue;
        }

        std::vector<Vec2> loop;
        loop.push_back(intersection);

        for (size_t j = i + 1; j <= lastSegmentStart; ++j)
            loop.push_back(m_lassoPoints[j]);

        loop.push_back(intersection);

        if (loop.size() < 4 || GetPolygonArea(loop) < 32)
            continue;

        m_completedLoop = std::move(loop);

        std::vector<Vec2> remainingPoints;
        std::vector<double> remainingTimes;

        for (size_t j = 0; j <= i; ++j)
        {
            remainingPoints.push_back(m_lassoPoints[j]);
            remainingTimes.push_back(m_lassoTimes[j]);
        }

        remainingPoints.push_back(intersection);
        remainingTimes.push_back(absTime);

        float dx = m_pos.x - intersection.x;
        float dy = m_pos.y - intersection.y;
        if (dx * dx + dy * dy > 1)
{
            remainingPoints.push_back(m_pos);
            remainingTimes.push_back(absTime);
        }

        m_lassoPoints = std::move(remainingPoints);
        m_lassoTimes = std::move(remainingTimes);
        break;
    }
}

void dino::Player::Draw() const
{
    if (!m_isConnected)
        return;

    float u1;
    float u2;
    float v1 = 0;
    float v2 = 24;

    if (m_hitTime > 0)
    {
        // Degat
        int32_t idxFrame = int32_t(m_absTime * 8) % 3;
        u1 = 336 + 24 * idxFrame;
        u2 = 336 + 24 + 24 * idxFrame;
    }
    else if (m_bRunning)
    {
        int32_t idxFrame = int32_t(m_absTime * 16) % 6;
        u1 = 432 + 24 * idxFrame;
        u2 = 432 + 24 + 24 * idxFrame;
    }
    else if (m_dir.x != 0 || m_dir.y != 0)
    {
        int32_t idxFrame = int32_t(m_absTime * 8) % 6;
        u1 = 96 + 24 * idxFrame;
        u2 = 96 + 24 + 24 * idxFrame;
    }
    else
    {
        int32_t idxFrame = int32_t(m_absTime * 8) % 4;
        u1 = 24 * idxFrame;
        u2 = 24 + 24 * idxFrame;

    }

    if (m_goingLeft)
    {
        std::swap(u1, u2);
    }

    v1 += 24 * m_idxPlayer;
    v2 += 24 * m_idxPlayer;

    std::vector<jv::gpu::Vertex> vertices;
    vertices.emplace_back(Vec2{m_pos.x - 16, m_pos.y - 32}, Vec2{u1, v1});
    vertices.emplace_back(Vec2{m_pos.x + 16, m_pos.y - 32}, Vec2{u2, v1});
    vertices.emplace_back(Vec2{m_pos.x - 16, m_pos.y}, Vec2{u1, v2});
    vertices.emplace_back(Vec2{m_pos.x + 16, m_pos.y - 32}, Vec2{u2, v1});
    vertices.emplace_back(Vec2{m_pos.x - 16, m_pos.y}, Vec2{u1, v2});
    vertices.emplace_back(Vec2{m_pos.x + 16, m_pos.y}, Vec2{u2, v2});

    jv::gpu::VertexBuffer* pVBuf = jv::gpu::CreateVertexBuffer("Player", vertices);
    jv::gpu::Draw(pVBuf, m_pTexture);
    jv::gpu::DestroyVertexBuffer(pVBuf);
}