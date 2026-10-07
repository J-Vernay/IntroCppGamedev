
#include <dino/dino_player.h>
#include <dino/dino_draw_utils.h>
#include <dino/dino_geometry.h>
#include <dino/dino_scene.h>
#include <math.h>

using PointList = std::vector<jv::util::Vec2>;

dino::Player::Player(Vec2 pos, double absTime, jv::gpu::Texture* texture,
    jv::input::GamepadIdx gamepadIdx, Color color, int32_t colorIndex)
    : Entity(pos)
{
    m_state = IDLE;
    m_colorIndex = colorIndex;
    m_color = color;
    m_gamepadIdx = gamepadIdx;
    m_dir = jv::util::RandomRotate({1, 0}, 0, 360);
    m_timeStart = absTime;
    m_pTexture = texture;
}

bool dino::Player::Pauses()
{
    jv::input::Gamepad input;
    if (jv::input::GetGamepad(m_gamepadIdx, input))
    {
        // Pause.
        if (m_bPauseReleased && input.start)
        {
            m_bPauseReleased = false;
            return true;
        }
        else if (!m_bPauseReleased && !input.start)
        {
            m_bPauseReleased = true;
        }
    }

    return false;
}

bool dino::Player::CheckJoin()
{
    // Ne pas vérifier si déjà rejoint.
    if (m_bHasJoined)
        return false;

    jv::input::Gamepad input;
    if (jv::input::GetGamepad(m_gamepadIdx, input))
    {
        if (input.start) // Rejoindre en appuyant sur start.
        {
            m_bHasJoined = true;
            return true;
        }
    }

    return false;
}

bool dino::Player::CheckLeave()
{
    // Ne pas vérifier si pas rejoint.
    if (!m_bHasJoined)
        return false;

    jv::input::Gamepad input;
    if (jv::input::GetGamepad(m_gamepadIdx, input))
    {
        if (input.select) // Quitter en appuyant sur select.
        {
            m_bHasJoined = false;
            return true;
        }
    }

    return false;
}

dino::Player::~Player()
{
}

void dino::Player::Update(double absTime, float deltaTime, Terrain& terrain)
{
    float speed = 80;
    bool running = false;
    m_hurtTimer -= deltaTime;

    // Entrées joueur.
    m_dir = {0, 0};
    jv::input::Gamepad input;
    if (jv::input::GetGamepad(m_gamepadIdx, input))
    {
        // Direction de mouvement.
        if (input.dpad_up)
            m_dir.y += -1;
        if (input.dpad_down)
            m_dir.y += 1;
        if (input.dpad_left)
            m_dir.x += -1;
        if (input.dpad_right)
            m_dir.x += 1;
        
        // Logique de normalisation simplifiée.
        if (abs(m_dir.x) > 0 && abs(m_dir.y) > 0)
        {
            m_dir.x *= 0.7;
            m_dir.y *= 0.7;
        }

        // Courir.
        if (input.btn_right)
        {
            speed *= 2;
            running = true;
        }
    }

    if (m_state != HURT)
    {
        Vec2 displacement = Vec2{m_dir.x * deltaTime * speed, m_dir.y * deltaTime * speed};
        Move(displacement, terrain);

        // Mettre à jour la direction du sprite, droite ou gauche.
        if (m_dir.x > 0)
        {
            m_facingLeft = false;
        }
        else if (m_dir.x < 0)
        {
            m_facingLeft = true;
        }

        // L'action que le joueur réalise en ce moment (cela dictera son animation).
        m_state = (m_dir.x == 0 && m_dir.y == 0) ? IDLE : (running ? RUN : WALK);
    }
    else
    {
        // L'immobilisation des dégâts dure 3 secondes.
        if (m_hurtTimer < 0)
        {
            m_state = IDLE;
        }
    }

    int frameRate = 0;
    int frameCount = 0;

    switch (m_state)
    {
    case IDLE:
        frameRate = 8;
        frameCount = 4;
        break;
    case WALK:
        frameRate = 8;
        frameCount = 6;
        break;
    case RUN:
        frameRate = 16;
        frameCount = 6;
        break;
    case HURT:
        frameRate = 8;
        frameCount = 3;
        break;
    default:
        break;
    }

    m_idxFrame = int32_t(absTime * frameRate) % frameCount;
}

void dino::Player::UpdateTrail(double absTime, float deltaTime, std::vector<std::pair<Vec2, Vec2>> playersLastMove)
{
    // Stocker les positions passées pour le lasso.
    m_pastPositions.push_back(m_pos);
    if (m_pastPositions.size() > 120)
    {
        m_pastPositions.erase(m_pastPositions.begin());
    }

    // Vérifier si un autre joueur est passé sur son lasso.
    CheckPlayerTrailOverlap(playersLastMove);
}

void dino::Player::Draw() const
{
    // Obtenir l'offset pour afficher la bonne action joueur.
    float uActionOffset = 0;

    switch (m_state)
    {
    case IDLE:
        uActionOffset = 0;
        break;
    case WALK:
        uActionOffset = 96;
        break;
    case RUN:
        uActionOffset = 432;
        break;
    case HURT:
        uActionOffset = 336;
        break;
    default:
        break;
    }

    // Le joueur se déplace visuellement vers la droite par défaut.
    float u1 = 24 * m_idxFrame + uActionOffset;
    float u2 = 24 + 24 * m_idxFrame + uActionOffset;
    float v1 = m_colorIndex * 24;
    float v2 = m_colorIndex * 24 + 24;

    jv::util::Color color = Color_WHITE;
    color.a = m_alpha;

    if (m_facingLeft) // Le joueur regarde vers la gauche. Inversion de U.
    {
        float temp = u1;
        u1 = u2;
        u2 = temp;
    }

    std::vector<jv::gpu::Vertex> vs;
    vs.emplace_back(Vec2{m_pos.x - 16, m_pos.y - 32}, Vec2{u1, v1}, color);
    vs.emplace_back(Vec2{m_pos.x + 16, m_pos.y - 32}, Vec2{u2, v1}, color);
    vs.emplace_back(Vec2{m_pos.x - 16, m_pos.y}, Vec2{u1, v2}, color);
    vs.emplace_back(Vec2{m_pos.x + 16, m_pos.y - 32}, Vec2{u2, v1}, color);
    vs.emplace_back(Vec2{m_pos.x - 16, m_pos.y}, Vec2{u1, v2}, color);
    vs.emplace_back(Vec2{m_pos.x + 16, m_pos.y}, Vec2{u2, v2}, color);

    jv::gpu::VertexBuffer* pVBuf = jv::gpu::CreateVertexBuffer("Player", vs);
    jv::gpu::Draw(pVBuf, m_pTexture);
    jv::gpu::DestroyVertexBuffer(pVBuf);
}

void dino::Player::DrawTrail() const
{
    float trailWidth = 5;
    std::vector<jv::gpu::Vertex> vs;

    GenVertices_Polyline(vs, m_pastPositions, trailWidth, m_color);
    jv::gpu::VertexBuffer* pVBuf = jv::gpu::CreateVertexBuffer("Trail", vs);
    jv::gpu::Draw(pVBuf, nullptr);
    jv::gpu::DestroyVertexBuffer(pVBuf);
}

void dino::Player::OnCaughtInLoop()
{
    m_state = HURT;
    m_hurtTimer = 3;
}

std::pair<int32_t, int32_t> dino::Player::CheckLoop()
{
    int32_t loopPointIndex1 = -1;
    int32_t loopPointIndex2 = -1;

    for (int i = m_pastPositions.size() - 3; i >= 0 && loopPointIndex1 == -1; i--)
    {
        Vec2 A = m_pastPositions[i + 1];
        Vec2 B = m_pastPositions[i];
        for (int j = m_pastPositions.size() - 2; j > i + 2 && loopPointIndex1 == -1; j--)
        {
            Vec2 C = m_pastPositions[j + 1];
            Vec2 D = m_pastPositions[j];

            if (!(A.x == B.x && A.y == B.y) && !(C.x == D.x && C.y == D.y) &&
                IntersectSegment(A, B, C, D))
            {
                loopPointIndex1 = i + 1;
                loopPointIndex2 = j;
            }
        }
    }

    return std::pair<int32_t, int32_t>(loopPointIndex1, loopPointIndex2);
}

PointList dino::Player::GetTrail()
{
    return m_pastPositions;
}

void dino::Player::CutLoop(int32_t loopPoint1, int32_t loopPoint2)
{
    m_pastPositions.erase(
        m_pastPositions.begin() + loopPoint1,
        m_pastPositions.begin() + loopPoint2);
}

void dino::Player::CheckPlayerTrailOverlap(std::vector<std::pair<Vec2, Vec2>> playersLastMove)
{
    int32_t overlapPointIndex = -1;

    for (int i = m_pastPositions.size() - 2; i >= 0 && overlapPointIndex == -1; i--)
    {
        Vec2 A = m_pastPositions[i + 1];
        Vec2 B = m_pastPositions[i];

        for (int p = 0; p < playersLastMove.size() && overlapPointIndex == -1; p++)
        {
            if (IntersectSegment(A, B, playersLastMove[p].first, playersLastMove[p].second))
            {
                overlapPointIndex = i + 1;
            }
        }
    }

    if (overlapPointIndex != -1)
    {
        m_pastPositions.erase(m_pastPositions.begin(), m_pastPositions.begin() + overlapPointIndex);
    }
}

void dino::Player::OnOutsideTerrain()
{

}