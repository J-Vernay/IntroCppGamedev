#include <exo2/exo2_draw.h>
#include <exo2/exo2_pong.h>

#include <algorithm>
#include <cmath>

namespace exo2
{

using jv::util::Color;

// CONSTANTES ICI
    constexpr float kPlayerSpeed = 200;
    constexpr float kAIPeriod = 1;
    constexpr float kBallInitSpeed = 100;
    constexpr float kFactor = 1.2;
    constexpr float kStepScore = 10;

    constexpr Color kBackgroundColor = {0, 0, 0, 255};
    constexpr Color kArenaColor = {70, 120, 70, 255};
    constexpr Color kBallColor = {255, 255, 255, 255};
    constexpr Color kPlayerColor = {128, 128, 255, 255};
    constexpr Color kAIColor = {255, 128, 128, 255};
    constexpr Color kDelimColor = {100, 150, 100, 255};

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

void exo2::Pong::Update(double absTime, float deltaTime)
{
    // UPDATE
    m_color.r = 128; // je le laisse pour le style
    m_color.g = 127.5 + 127.5 * std::sin(absTime * 3);
    m_color.b = 127.5 + 127.5 * std::cos(absTime * 3);
    m_color.a = 255;

    _UpdateAI(absTime, deltaTime);
    _UpdatePlayer(absTime, deltaTime);
    _UpdateBall(absTime, deltaTime);

}
void exo2::Pong::_UpdateAI(double absTime, float deltaTime){
    // Calcul de la position de la raquette de l'IA, à droite de l'écran.
    float f = std::fabsf(2 * std::fmodf(absTime, kAIPeriod) / kAIPeriod - 1);
    m_aiPos.x = kArenaSize.x - kAiSize.x;
    m_aiPos.y = (kArenaSize.y - kAiSize.y) * f;
}
void exo2::Pong::_UpdatePlayer(double absTime, float deltaTime)
{
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
void exo2::Pong::_UpdateBall(double absTime, float deltaTime)
{
    m_ballPos.x = m_ballPos.x + m_ballDir.x * m_ballSpeed * deltaTime;
    m_ballPos.y = m_ballPos.y + m_ballDir.y * m_ballSpeed * deltaTime;

    bool bPlayerWin = false;
    bool bAiWin = false;

    if (m_ballPos.x <= 0)
        bAiWin = true;
    else if (m_ballPos.x >= kArenaSize.x)
        bPlayerWin = true;

    if (bPlayerWin || bAiWin)
    {
      
        m_ballPos.x = (kArenaSize.x - kBallSize.x) / 2;
        m_ballPos.y = (kArenaSize.y - kBallSize.y) / 2;
        m_ballDir = jv::util::RandomRotate({-1, 0}, -10, 10);
        m_ballSpeed = kBallInitSpeed;
        if (bAiWin)
        {
            m_aiScore++;
        }
        else
        {
            m_playerScore++;
            
        }
    }

    if (m_ballPos.y <= 0)
    {
        // Rebond mur du haut
        m_ballPos.y = -m_ballPos.y;
        m_ballDir.y = -m_ballDir.y;
    }
    else if (m_ballPos.y + kBallSize.y >= kArenaSize.y)
    {
        // Rebond mur du bas
        m_ballPos.y = 2 * kArenaSize.y - m_ballPos.y - 2 * kBallSize.y;
        m_ballDir.y = -m_ballDir.y;
    }
    else if (m_ballPos.x <= kPlayerSize.x && m_ballPos.y + kBallSize.y >= m_playerPos.y &&
             m_ballPos.y <= m_playerPos.y + kPlayerSize.y)
    {
        // Rebond joueur
        m_ballPos.x = kPlayerSize.x;
        m_ballDir = jv::util::RandomRotate({1, 0}, -60, 60);
        m_ballSpeed *= kFactor;
    }
    else if (m_ballPos.x >= kArenaSize.x - kAiSize.x && m_ballPos.y + kBallSize.y >= m_aiPos.y &&
             m_ballPos.y <= m_aiPos.y + kAiSize.y)
    {
        // Rebond AI
        m_ballPos.x = kArenaSize.x - kAiSize.x - kBallSize.x;
        m_ballDir = jv::util::RandomRotate({-1, 0}, -60, 60);
        m_ballSpeed *= kFactor;
    }
}



void exo2::Pong::Draw() const
{
    // DRAW
    jv::gpu::SetBackgroundColor(m_color);

    Vec2 delimPos = {kArenaSize.x / 2 - 1, 0};
    Vec2 delimSize = {2, kArenaSize.y};

    //DrawRect({0, 0}, kArenaSize, kArenaColor);   //Je l'enlève pour le style
    DrawRect(delimPos, delimSize, kDelimColor);
    DrawRect(m_aiPos, kAiSize, kAIColor);
    DrawRect(m_playerPos, kPlayerSize, kPlayerColor);
    DrawRect(m_ballPos, kBallSize, kBallColor);
    DrawScore(Vec2(kArenaSize.x / 2, 0), kBallSize, m_playerScore, kPlayerColor, m_aiScore, kAIColor);
  
}
