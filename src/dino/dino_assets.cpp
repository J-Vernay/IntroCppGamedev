#include <dino/dino_assets.h>
#include <dino/dino_draw_utils.h>

void dino::AssetsHolder::LoadTextures()
{
    g_Textures["animals"] = dino::LoadImageAsset("animals.bmp");
    g_Textures["players"] = dino::LoadImageAsset("dinosaurs.bmp");
}

dino::AssetsHolder::~AssetsHolder()
{
    jv::gpu::DestroyTexture(g_Textures["animals"]);
    jv::gpu::DestroyTexture(g_Textures["players"]);
}