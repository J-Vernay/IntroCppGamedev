#include <exo2/exo2_draw.h>
#include <exo2/exo2_pong.h>

#include <algorithm>
#include <cmath>

namespace exo2
{

using jv::util::Color;

constexpr float kPlayerSpeed = 200;
constexpr float kAIPeriod = 1;
constexpr float kBallInitSpeed = 100;
constexpr float kFactor = 1.2;

// Constantes de couleur du jeu Pong.
constexpr Color kBackgroundColor = {0, 0, 0, 255};
constexpr Color kArenaColor = {70, 120, 70, 255};
constexpr Color kBallColor = {255, 255, 255, 255};
constexpr Color kPlayerColor = {128, 128, 255, 255};
constexpr Color kPlayerColorScore = {128, 128, 255, 155};
constexpr Color kAIColor = {255, 128, 128, 255};
constexpr Color kAIColorScore = {255, 128, 128, 155};
constexpr Color kDelimColor = {100, 150, 100, 255};

// Constantes des dimensions des objets.
constexpr Vec2 kArenaSize = {300, 200};
constexpr Vec2 kBallSize = {10, 10};
constexpr Vec2 kPlayerSize = {10, 40};
constexpr Vec2 kAiSize = {10, 100};

} // namespace exo2

exo2::Pong::Pong()
{
    jv::gpu::SetBackgroundColor(kBackgroundColor);
    jv::gpu::SetRenderSize(kArenaSize);

    // Joueur
    m_playerPos.x = 0;
    m_playerPos.y = (kArenaSize.y - kPlayerSize.y) / 2;

    // Balle
    m_ballPos.x = (kArenaSize.x - kBallSize.x) / 2;
    m_ballPos.y = (kArenaSize.y - kBallSize.y) / 2;
    m_ballDir = jv::util::RandomRotate({-1, 0}, -60, 60);
    m_ballSpeed = kBallInitSpeed;
}

void exo2::Pong::_UpdateAI(double absTime)
{
    // Calcul de la position de la raquette de l'IA, à droite de l'écran.
    float f = std::fabsf(2 * std::fmodf(absTime, kAIPeriod) / kAIPeriod - 1);
    m_aiPos.x = kArenaSize.x - kAiSize.x;
    m_aiPos.y = (kArenaSize.y - kAiSize.y) * f;
}

void exo2::Pong::_UpdatePlayer(float deltaTime)
{
    // Calcul de la position de la raquette du joueur, en utilisant les flèches du clavier.
    jv::input::Gamepad keyboard;
    if (jv::input::GetGamepad(jv::input::GamepadIdx::Keyboard, keyboard))
    {
        float dir = 0;
        if (keyboard.dpad_up)
            dir -= 1;
        if (keyboard.dpad_down)
            dir += 1;
        float y = m_playerPos.y + deltaTime * dir * kPlayerSpeed;

        m_playerPos.x = 0;
        m_playerPos.y = std::clamp(y, 0.f, kArenaSize.y - kPlayerSize.y);
    }
}

void exo2::Pong::_UpdateBall(float deltaTime)
{
    // Calcul de la position de la balle.

    // DEPLACEMENT DE LA BALLE
    m_ballPos.x = m_ballSpeed * m_ballDir.x * deltaTime + m_ballPos.x;
    m_ballPos.y = m_ballSpeed * m_ballDir.y * deltaTime + m_ballPos.y;

    bool bPlayerWin = false;
    bool bAiWin = false;

    if (m_ballPos.y <= 0)
    {
        // Rebond mur du haut
        m_ballPos.y = -m_ballPos.y;
        m_ballDir.y = -m_ballDir.y;
    }
    else if (m_ballPos.y >= kArenaSize.y - kBallSize.y)
    {
        // Rebond mur du bas
        m_ballPos.y = 2 * kArenaSize.y - m_ballPos.y - 2 * kBallSize.y;
        m_ballDir.y = -m_ballDir.y;
    }
    else if (m_ballPos.x <= kPlayerSize.x)
    {
        // Rebond joueur
        m_ballPos.x = kPlayerSize.x;
        m_ballDir = jv::util::RandomRotate({1, 0}, -60, 60);
        m_ballSpeed *= kFactor;

        bool isInPlayer = m_ballPos.y <= m_playerPos.y + kPlayerSize.y - kBallSize.y &&
                          m_ballPos.y >= m_playerPos.y - kPlayerSize.y;

        if (!isInPlayer)
            bAiWin = true;
    }
    else if (m_ballPos.x + kBallSize.x >= kArenaSize.x - kAiSize.x)
    {
        // Rebond AI
        m_ballPos.x = kArenaSize.x - kAiSize.x - kBallSize.x;
        m_ballDir = jv::util::RandomRotate({-1, 0}, -60, 60);
        m_ballSpeed *= kFactor;

        bool isInAi = m_ballPos.y <= m_aiPos.y + kAiSize.y - kBallSize.y &&
                      m_ballPos.y >= m_aiPos.y - kAiSize.y;

        if (!isInAi)
            bPlayerWin = true;
    }

    if (bPlayerWin || bAiWin)
    {
        m_ballPos.x = (kArenaSize.x - kBallSize.x) / 2;
        m_ballPos.y = (kArenaSize.y - kBallSize.y) / 2;
        m_ballDir = jv::util::RandomRotate({-1, 0}, -10, 10);
        m_ballSpeed = kBallInitSpeed;
        
        if (bPlayerWin)
            m_scorePlayer++;
        if (bAiWin)
            m_scoreAi++;
    }
}

void exo2::Pong::Update(double absTime, float deltaTime)
{
    
    _UpdateAI(absTime);

    _UpdatePlayer(deltaTime);

    _UpdateBall(deltaTime);

}

void exo2::Pong::Draw() const
{
    Vec2 delimPos = {kArenaSize.x / 2 - 1, 0};
    Vec2 delimSize = {2, kArenaSize.y};

    DrawRect({0, 0}, kArenaSize, kArenaColor);
    DrawRect(delimPos, delimSize, kDelimColor);
    DrawRect(m_aiPos, kAiSize, kAIColor);
    DrawRect(m_playerPos, kPlayerSize, kPlayerColor);
    DrawRect(m_ballPos, kBallSize, kBallColor);

    exo2::DrawScore(
        {kArenaSize.x / 2, kArenaSize.y / 2 - kArenaSize.y / 3},
        {10, 10},
        m_scorePlayer,
        kPlayerColorScore,
        m_scoreAi,
        kAIColorScore);
    
}
