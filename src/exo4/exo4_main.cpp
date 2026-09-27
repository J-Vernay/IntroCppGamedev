
#include <algorithm>
#include <array>
#include <exo4/exo4_image.h>

std::array<exo4::Image, 6> g_images;
bool g_bKeyPressed;
int32_t g_imageSelected;

void jv::game::Init()
{
    g_images[0] = exo4::LoadImageAsset("image_1.bmp");
    g_images[1] = exo4::LoadImageAsset("image_2.bmp");
    g_images[2] = exo4::LoadImageAsset("terrain.bmp");
    g_images[3] = exo4::LoadImageAsset("dinosaurs.bmp");
    g_images[4] = exo4::LoadImageAsset("animals.bmp");
    g_images[5] = exo4::LoadImageAsset("monogram-bitmap.bmp");
    g_bKeyPressed = 0;
    g_imageSelected = 0;
}

void jv::game::Update(double absTime, float deltaTime)
{
    jv::input::Gamepad keyboard;
    if (jv::input::GetGamepad(jv::input::GamepadIdx::Keyboard, keyboard))
    {
        if (!g_bKeyPressed)
        {
            if (keyboard.dpad_left)
                --g_imageSelected;
            if (keyboard.dpad_right)
                ++g_imageSelected;
        }
        g_bKeyPressed = (keyboard.dpad_left || keyboard.dpad_right);
    }
    if (g_imageSelected >= (int32_t)g_images.size())
        g_imageSelected = 0;
    if (g_imageSelected < 0)
        g_imageSelected = (int32_t)g_images.size() - 1;
}

void jv::game::Draw()
{
    exo4::Image const& img = g_images[g_imageSelected];
    jv::util::Vec2 pxSize = img.pxSize;

    jv::gpu::SetRenderSize(pxSize);

    jv::gpu::Vertex vs[6];
    vs[0].pos = {0, 0};
    vs[1].pos = {0, pxSize.y};
    vs[2].pos = {pxSize.x, pxSize.y};
    vs[3].pos = {0, 0};
    vs[4].pos = {pxSize.x, 0};
    vs[5].pos = {pxSize.x, pxSize.y};

    vs[0].uv = {0, 0};
    vs[1].uv = {0, pxSize.y};
    vs[2].uv = {pxSize.x, pxSize.y};
    vs[3].uv = {0, 0};
    vs[4].uv = {pxSize.x, 0};
    vs[5].uv = {pxSize.x, pxSize.y};

    vs[0].color = {255, 255, 255, 255};
    vs[1].color = {255, 255, 255, 255};
    vs[2].color = {255, 255, 255, 255};
    vs[3].color = {255, 255, 255, 255};
    vs[4].color = {255, 255, 255, 255};
    vs[5].color = {255, 255, 255, 255};

    jv::gpu::VertexBuffer* vb = jv::gpu::CreateVertexBuffer("exo4_main", vs);
    jv::gpu::Draw(vb, img.pTexture);
    jv::gpu::DestroyVertexBuffer(vb);
}

void jv::game::Shut()
{
    for (exo4::Image const& img : g_images)
        jv::gpu::DestroyTexture(img.pTexture);
}
