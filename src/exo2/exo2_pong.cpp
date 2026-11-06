#include <exo2/exo2_draw.h>
#include <exo2/exo2_pong.h>

#include <algorithm>
#include <cmath>

namespace exo2
{

using jv::util::Color;

// Constantes des grandeurs physiques.
constexpr float kPlayerSpeed = 200;
constexpr float kAIPeriod = 1;
constexpr float kBallInitSpeed = 100;
constexpr float kFactor = 1.2;

// Constantes de couleur du jeu Pong.
constexpr Color kBackgroundColor = {0, 0, 0, 255};
constexpr Color kArenaColor = {70, 120, 70, 255};
constexpr Color kBallColor = {255, 255, 255, 255};
constexpr Color kPlayerColor = {128, 128, 255, 255};
constexpr Color kAIColor = {255, 128, 128, 255};
constexpr Color kDelimColor = {100, 150, 100, 255};

// Constantes des dimensions des objets.
constexpr Vec2 kArenaSize = {300, 200};
constexpr Vec2 kBallSize = {10, 10};
constexpr Vec2 kPlayerSize = {10, 40};
constexpr Vec2 kAiSize = {10, 100};

// Variables globales
Vec2 g_playerPos;
Vec2 g_aiPos;
Vec2 g_ballPos;
Vec2 g_ballDir;
float g_ballSpeed;

} // namespace exo2

exo2::Pong::Pong()
{
    // INITIALISATION
    jv::gpu::SetRenderSize({10, 10});

     // Initialisation du rendu
    jv::gpu::SetBackgroundColor(kBackgroundColor);
    jv::gpu::SetRenderSize(kArenaSize);

    // Joueur
    g_playerPos.x = 0;
    g_playerPos.y = (kArenaSize.y - kPlayerSize.y) / 2;

    // Balle
    g_ballPos.x = (kArenaSize.x - kBallSize.x) / 2;
    g_ballPos.y = (kArenaSize.y - kBallSize.y) / 2;
    g_ballDir = jv::util::RandomRotate({-1, 0}, -60, 60);
    g_ballSpeed = kBallInitSpeed;

}

void exo2::Pong::Update(double absTime, float deltaTime)
{
    // UPDATE
    m_color.r = 128;
    m_color.g = 127.5 + 127.5 * std::sin(absTime * 3);
    m_color.b = 127.5 + 127.5 * std::cos(absTime * 3);
    m_color.a = 255;

    {
        // Calcul de la position de la raquette de l'IA, à droite de l'écran.
        _UpdateAI(absTime, deltaTime);

        // Calcul de la position de la raquette du joueur, en utilisant les flèches du clavier.
        _UpdatePlayer(absTime, deltaTime);

        // Calcul de la position de la balle.

        // DEPLACEMENT DE LA BALLE
        _UpdateBall(absTime, deltaTime);

    }

}

void exo2::Pong::_UpdateAI(double absTime, float deltaTime)
{
    float f = std::fabsf(2 * std::fmodf(absTime, kAIPeriod) / kAIPeriod - 1);
    g_aiPos.x = kArenaSize.x - kAiSize.x;
    g_aiPos.y = (kArenaSize.y - kAiSize.y) * f;
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
        float y = g_playerPos.y + deltaTime * dir * kPlayerSpeed;

        g_playerPos.x = 0;
        g_playerPos.y = std::clamp(y, 0.f, kArenaSize.y - kPlayerSize.y);
    }
}

void exo2::Pong::_UpdateBall(double absTime, float deltaTime)
{
    g_ballPos.x = g_ballPos.x + g_ballDir.x * g_ballSpeed * deltaTime;
    g_ballPos.y = g_ballPos.y + g_ballDir.y * g_ballSpeed * deltaTime;

    bool bPlayerWin = false;
    bool bAiWin = false;

    if (g_ballPos.y < 0)
    {
        // Rebond mur du haut
        g_ballPos.y = -g_ballPos.y;
        g_ballDir.y = -g_ballDir.y;
    }
    else if (g_ballPos.y + kBallSize.y > kArenaSize.y)
    {
        // Rebond mur du bas
        g_ballPos.y = 2 * kArenaSize.y - g_ballPos.y - 2 * kBallSize.y;
        g_ballDir.y = -g_ballDir.y;
    }
    else if (g_ballPos.x < kPlayerSize.x)
    {
        // Rebond joueur
        g_ballPos.x = kPlayerSize.x;
        g_ballDir = jv::util::RandomRotate({1, 0}, -60, 60);
        g_ballSpeed *= kFactor;

        if (g_ballPos.y + kBallSize.y < g_playerPos.y ||
            g_ballPos.y > g_playerPos.y + kPlayerSize.y)
            bAiWin = true;
    }
    else if (g_ballPos.x + kBallSize.x > g_aiPos.x)
    {
        // Rebond AI
        g_ballPos.x = kArenaSize.x - kAiSize.x - kBallSize.x;
        g_ballDir = jv::util::RandomRotate({-1, 0}, -60, 60);
        g_ballSpeed *= kFactor;

        if (g_ballPos.y + kBallSize.y < g_aiPos.y || g_ballPos.y > g_aiPos.y + kAiSize.y)
            bPlayerWin = true;
    }

    if (bPlayerWin || bAiWin)
    {
        g_ballPos.x = (kArenaSize.x - kBallSize.x) / 2;
        g_ballPos.y = (kArenaSize.y - kBallSize.y) / 2;
        g_ballDir = jv::util::RandomRotate({-1, 0}, -10, 10);
        g_ballSpeed = kBallInitSpeed;
    }
}

void exo2::Pong::Draw() const
{
    {
        Vec2 delimPos = {kArenaSize.x / 2 - 1, 0};
        Vec2 delimSize = {2, kArenaSize.y};

        DrawRect({0, 0}, kArenaSize, kArenaColor);
        DrawRect(delimPos, delimSize, kDelimColor);
        DrawRect(g_aiPos, kAiSize, kAIColor);
        DrawRect(g_playerPos, kPlayerSize, kPlayerColor);
        DrawRect(g_ballPos, kBallSize, kBallColor);
    }
}

void exo2::DrawScore(Vec2 center, Vec2 pointSize, int scorePlayer, 
    Color colorPlayer, int scoreAI, Color colorAI)
{
    Vec2 pos;
    pos.x = center.x - pointSize.x * scorePlayer;
    pos.y = center.y - pointSize.y / 2;
    DrawRect(pos, {pointSize.x * scorePlayer, pointSize.y}, colorPlayer);

    pos.x = center.x;
    pos.y = center.y - pointSize.y / 2;
    DrawRect(pos, {pointSize.x * scoreAI, pointSize.y}, colorAI);
}
