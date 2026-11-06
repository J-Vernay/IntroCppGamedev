#include <dino/dino_scene.h>

dino::Scene* g_pDinoScene;

void jv::game::Init()
{
    jv::util::Vec2 rdrSize = {480, 360};
    jv::gpu::SetRenderSize(rdrSize);

    g_pDinoScene = new dino::Scene;
}

void jv::game::Update(double absTime, float deltaTime)
{
    g_pDinoScene->Update(absTime, deltaTime);
}

void jv::game::Draw()
{
    g_pDinoScene->SortEntities();
    g_pDinoScene->Draw();
}

void jv::game::Shut()
{
    delete g_pDinoScene;
}