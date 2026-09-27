#pragma once

#include <jv/jv.h>

#define NOMINMAX
#include <windows.h>

namespace jv::win64
{

HWND InitWindow(HINSTANCE hInstance, jv::util::Vec2 windowInitSize);
LRESULT CALLBACK HandleEvent(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void ShutWindow(HWND hWindow);

void InitRenderer(HWND hWindow);
void BeginRendererDraw();
void EndRendererDraw();
void ResizeRenderer(jv::util::Vec2 size);
void ShutRenderer();

struct alignas(16) ShaderCBuffer
{
    jv::util::Vec2 half_vp_size;
    jv::util::Vec2 tex_size;
    jv::util::Vec2 offset;
    jv::util::Vec2 rot_cos_sin;
    jv::util::Vec2 scale;
};

extern const char kVertexShader[];
extern const char kPixelShader[];

} // namespace jv::win64
