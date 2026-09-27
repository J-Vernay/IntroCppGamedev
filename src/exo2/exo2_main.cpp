#include <exo2/exo2_pong.h>

exo2::Pong* g_pPong = nullptr;

void jv::game::Init()
{
    g_pPong = new exo2::Pong;
}

void jv::game::Update(double absTime, float deltaTime)
{
    g_pPong->Update(absTime, deltaTime);
}

void jv::game::Draw()
{
    g_pPong->Draw();
}

void jv::game::Shut()
{
    delete g_pPong;
}
