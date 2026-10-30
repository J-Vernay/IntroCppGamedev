#include <dino/dino_game.h>

dino::DinoGame* g_pDinoGame;

void jv::game::Init()
{
    jv::util::Vec2 rdrSize = {480, 360};
    jv::gpu::SetRenderSize(rdrSize);

    g_pDinoGame = new dino::DinoGame();
}

void jv::game::Update(double absTime, float deltaTime)
{
    g_pDinoGame->Update(absTime, deltaTime);
}

void jv::game::Draw()
{
    g_pDinoGame->Draw();
}

void jv::game::Shut()
{
    delete g_pDinoGame;
}