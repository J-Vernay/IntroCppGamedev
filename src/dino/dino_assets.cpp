#include <dino/dino_assets.h>
#include <dino/dino_draw_utils.h>

void dino::AssetsHolder::LoadTextures()
{
    g_Textures["animals"] = dino::LoadImageAsset("animals.bmp");
    g_Textures["players"] = dino::LoadImageAsset("dinosaurs.bmp");
    g_Textures["terrain"] = dino::LoadImageAsset("terrain.bmp");
    g_Textures["text"] = dino::LoadImageAsset("monogram-bitmap.bmp");
    g_Textures["white"] = jv::gpu::CreateTexture("white", {1, 1}, {&Color_WHITE, 1});
}

dino::AssetsHolder::~AssetsHolder()
{
    jv::gpu::DestroyTexture(g_Textures["animals"]);
    jv::gpu::DestroyTexture(g_Textures["players"]);
    jv::gpu::DestroyTexture(g_Textures["white"]);
}