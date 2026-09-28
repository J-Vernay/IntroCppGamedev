#include <jv/jv.h>

#include <algorithm>
#include <cmath>
#include <numbers>

using jv::util::Color;
using jv::util::Vec2;

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

void DrawRect(Vec2 pos, Vec2 size, Color color)
{
    jv::gpu::Vertex vs[6];
    vs[0].pos = {pos.x, pos.y};
    vs[1].pos = {pos.x, pos.y + size.y};
    vs[2].pos = {pos.x + size.x, pos.y + size.y};
    vs[3].pos = {pos.x, pos.y};
    vs[4].pos = {pos.x + size.x, pos.y};
    vs[5].pos = {pos.x + size.x, pos.y + size.y};

    vs[0].color = color;
    vs[1].color = color;
    vs[2].color = color;
    vs[3].color = color;
    vs[4].color = color;
    vs[5].color = color;

    jv::gpu::VertexBuffer* vb = jv::gpu::CreateVertexBuffer("drawRect", vs);
    jv::gpu::Draw(vb, nullptr, {});
    jv::gpu::DestroyVertexBuffer(vb);
}

void jv::game::Init()
{
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

void jv::game::Update(double absTime, float deltaTime)
{
    // Calcul de la position de la raquette de l'IA, à droite de l'écran.
    float f = std::fabsf(2 * std::fmodf(absTime, kAIPeriod) / kAIPeriod - 1);
    g_aiPos.x = kArenaSize.x - kAiSize.x;
    g_aiPos.y = (kArenaSize.y - kAiSize.y) * f;

    // Calcul de la position de la raquette du joueur, en utilisant les flèches du clavier.
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

    // Calcul de la position de la balle.

    // DEPLACEMENT DE LA BALLE
    g_ballPos.x += g_ballDir.x * g_ballSpeed * deltaTime;
    g_ballPos.y += g_ballDir.y * g_ballSpeed * deltaTime;


    // DETECTION DES SORTIES DE BALLE ET RELANCE
    bool bPlayerWin = g_ballPos.x > kArenaSize.x;
    bool bAiWin = g_ballPos.x < 0;
    if (bPlayerWin || bAiWin)
    {
        g_ballPos.x = (kArenaSize.x - kBallSize.x) / 2;
        g_ballPos.y = (kArenaSize.y - kBallSize.y) / 2;
        g_ballDir = jv::util::RandomRotate({-1, 0}, -10, 10);
        g_ballSpeed = kBallInitSpeed;
        return;
    }

    // GESTION DES REBONDS
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
    else if (
        g_ballPos.x < g_playerPos.x + kPlayerSize.x
        && g_ballPos.y + kBallSize.y > g_playerPos.y 
        && g_ballPos.y < g_playerPos.y + kPlayerSize.y
        )
    {
        // Rebond joueur
        g_ballPos.x = kPlayerSize.x;
        g_ballDir = jv::util::RandomRotate({1, 0}, -60, 60);
        g_ballSpeed *= kFactor;

        
    }
    else if (
        g_ballPos.x > g_aiPos.x - kAiSize.x 
        && g_ballPos.y + kBallSize.y > g_aiPos.y 
        && g_ballPos.y < g_aiPos.y + kAiSize.y
        )
    {
        // Rebond AI
        g_ballPos.x = kArenaSize.x - kAiSize.x - kBallSize.x;
        g_ballDir = jv::util::RandomRotate({-1, 0}, -60, 60);
        g_ballSpeed *= kFactor;
    }
}

void jv::game::Draw()
{
    Vec2 delimPos = {kArenaSize.x / 2 - 1, 0};
    Vec2 delimSize = {2, kArenaSize.y};

    DrawRect({0, 0}, kArenaSize, kArenaColor);
    DrawRect(delimPos, delimSize, kDelimColor);
    DrawRect(g_aiPos, kAiSize, kAIColor);
    DrawRect(g_playerPos, kPlayerSize, kPlayerColor);
    DrawRect(g_ballPos, kBallSize, kBallColor);
}

void jv::game::Shut()
{
}
