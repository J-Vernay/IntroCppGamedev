#include <jv/x64-windows/jv_win64_impl.h>

// COMMENTAIRE
// Fonction qui implémente de la logique de création de la fenêtre de jeu.
HWND jv::win64::InitWindow(HINSTANCE hInstance, jv::util::Vec2 windowInitSize)
{
    WNDCLASSEXA wcx{};
    wcx.cbSize = sizeof(WNDCLASSEXA);
    wcx.lpfnWndProc = &jv::win64::HandleEvent;
    wcx.hInstance = hInstance;
    wcx.style = CS_OWNDC;
    wcx.lpszClassName = "IntroCppGamedev";

    ATOM classWindow = RegisterClassExA(&wcx);
    if (classWindow == 0)
        jv::util::Panic("RegisterClassExA failed");

    RECT windowRect{0, 0, (int32_t)windowInitSize.x, (int32_t)windowInitSize.y};
    AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);
    int32_t width = windowRect.right - windowRect.left;
    int32_t height = windowRect.bottom - windowRect.top;

    // COMMENTAIRE
    // Appel au système d'exploitation pour créer la fenêtre de jeu.
    HWND hWindow = CreateWindowExA(0, "IntroCppGamedev", "IntroCppGamedev", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, width, height, nullptr, nullptr, hInstance, nullptr);
    if (hWindow == nullptr)
        jv::util::Panic("CreateWindowEx failed");

    ShowWindow(hWindow, SW_SHOWNORMAL);
    UpdateWindow(hWindow);
    return hWindow;
}

// COMMENTAIRE
LRESULT CALLBACK jv::win64::HandleEvent(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (uMsg == WM_DESTROY)
    {
        PostQuitMessage(0);
        return 0;
    }
    if (uMsg == WM_SIZE)
    {
        PostMessageA(hWnd, uMsg, wParam, lParam);
        return 0;
    }
    if (uMsg == WM_PAINT)
    {
        PAINTSTRUCT ps;
        BeginPaint(hWnd, &ps);
        EndPaint(hWnd, &ps);
        return 0;
    }
    return DefWindowProcA(hWnd, uMsg, wParam, lParam);
}

void jv::win64::ShutWindow(HWND hWindow)
{
    DestroyWindow(hWindow);
}
