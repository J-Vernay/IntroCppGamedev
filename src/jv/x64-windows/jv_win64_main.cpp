#include <exception>
#include <jv/x64-windows/jv_win64_impl.h>
#include <string>

#include <cmath>
#include <winuser.h>
#include <xinput.h>

// COMMENTAIRE
HWND g_hWindow;

// COMMENTAIRE
int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow)
{
    // COMMENTAIRE
    g_hWindow = jv::win64::InitWindow(hInst, {640, 480});
    jv::win64::InitRenderer(g_hWindow);

    // COMMENTAIRE
    LARGE_INTEGER tickPeriod, tickStart;
    QueryPerformanceFrequency(&tickPeriod);
    QueryPerformanceCounter(&tickStart);
    double tickFrequency = 1.0 / tickPeriod.QuadPart;
    double lastAbsTime = 0;

    // COMMENTAIRE
    jv::game::Init();

    // COMMENTAIRE
    bool bContinue = true;
    while (bContinue)
    {

        // COMMENTAIRE
        MSG msg;
        BOOL fGotMessage;
        while ((fGotMessage = PeekMessageA(&msg, nullptr, 0, 0, PM_REMOVE)))
        {
            if (fGotMessage == -1)
                jv::util::Panic("PeekMessageW failed");

            if (msg.message == WM_QUIT)
            {
                bContinue = false;
            }
            else if (msg.hwnd == g_hWindow && msg.message == WM_SIZE)
            {
                // COMMENTAIRE
                jv::util::Vec2 windowSize(LOWORD(msg.lParam), HIWORD(msg.lParam));
                OutputDebugStringA("ResizeRenderer\n");
                jv::win64::ResizeRenderer(windowSize);
            }
            else
            {
                TranslateMessage(&msg);
                DispatchMessageA(&msg);
            }
        }
        if (!bContinue)
            break;

        // COMMENTAIRE
        LARGE_INTEGER tickNow;
        QueryPerformanceCounter(&tickNow);
        double absTime = (tickNow.QuadPart - tickStart.QuadPart) * tickFrequency;
        float deltaTime = std::min(absTime - lastAbsTime, 0.033);
        lastAbsTime = absTime;

        // COMMENTAIRE
        jv::game::Update(absTime, deltaTime);

        // COMMENTAIRE
        jv::win64::BeginRendererDraw();
        jv::game::Draw();
        jv::win64::EndRendererDraw();
    }

    // COMMENTAIRE
    jv::game::Shut();

    // COMMENTAIRE
    jv::win64::ShutRenderer();
    jv::win64::ShutWindow(g_hWindow);

    return EXIT_SUCCESS;
}

// COMMENTAIRE
bool jv::input::GetGamepad(GamepadIdx idx, Gamepad& outGamepad) noexcept
{
    outGamepad = {};
    if (idx == GamepadIdx::Keyboard)
    {
        outGamepad.dpad_up = GetAsyncKeyState(VK_UP);
        outGamepad.dpad_left = GetAsyncKeyState(VK_LEFT);
        outGamepad.dpad_right = GetAsyncKeyState(VK_RIGHT);
        outGamepad.dpad_down = GetAsyncKeyState(VK_DOWN);
        outGamepad.btn_up = GetAsyncKeyState('Z') || GetAsyncKeyState('W');
        outGamepad.btn_left = GetAsyncKeyState('Q') || GetAsyncKeyState('A');
        outGamepad.btn_right = GetAsyncKeyState('D');
        outGamepad.btn_down = GetAsyncKeyState('S');
        outGamepad.start = GetAsyncKeyState(VK_RETURN) || GetAsyncKeyState(VK_SPACE);
        outGamepad.select = GetAsyncKeyState(VK_SHIFT);
        outGamepad.shoulder_left = GetAsyncKeyState(VK_CONTROL);
        outGamepad.shoulder_right = GetAsyncKeyState(VK_MENU);

        float x, y, hypot;

        x = outGamepad.dpad_right - outGamepad.dpad_left;
        y = outGamepad.dpad_down - outGamepad.dpad_up;
        hypot = std::hypot(x, y);
        if (hypot > 0)
            outGamepad.stick_left = {x / hypot, y / hypot};

        x = outGamepad.btn_right - outGamepad.btn_left;
        y = outGamepad.btn_down - outGamepad.btn_up;
        hypot = std::hypot(x, y);
        if (hypot > 0)
            outGamepad.stick_right = {x / hypot, y / hypot};

        POINT p;
        if (GetCursorPos(&p) && ScreenToClient(g_hWindow, &p))
            outGamepad.mouse = {(float)p.x, (float)p.y};

        return true;
    }

    int32_t idxNumber = (int32_t)idx;
    if (idxNumber < 0 || idxNumber >= XUSER_MAX_COUNT)
        return false;

    XINPUT_STATE state;
    if (XInputGetState(idxNumber, &state) != ERROR_SUCCESS)
        return false;
    outGamepad.dpad_up = state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP;
    outGamepad.dpad_left = state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT;
    outGamepad.dpad_right = state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT;
    outGamepad.dpad_down = state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN;
    outGamepad.btn_up = state.Gamepad.wButtons & XINPUT_GAMEPAD_Y;
    outGamepad.btn_left = state.Gamepad.wButtons & XINPUT_GAMEPAD_X;
    outGamepad.btn_right = state.Gamepad.wButtons & XINPUT_GAMEPAD_B;
    outGamepad.btn_down = state.Gamepad.wButtons & XINPUT_GAMEPAD_A;
    outGamepad.start = state.Gamepad.wButtons & XINPUT_GAMEPAD_START;
    outGamepad.select = state.Gamepad.wButtons & XINPUT_GAMEPAD_BACK;
    outGamepad.shoulder_left = state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER;
    outGamepad.shoulder_right = state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER;

    SHORT x, y;
    x = state.Gamepad.sThumbLX;
    y = state.Gamepad.sThumbLY;
    if (x <= -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE || x >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
        outGamepad.stick_left.x = (float)x / 32768.f;
    if (y <= -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE || y >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
        outGamepad.stick_left.y = (float)y / -32768.f;
    x = state.Gamepad.sThumbRX;
    y = state.Gamepad.sThumbRY;
    if (x <= -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE || x >= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
        outGamepad.stick_right.x = (float)x / 32768.f;
    if (y <= -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE || y >= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
        outGamepad.stick_right.y = (float)y / -32768.f;

    return true;
}

// COMMENTAIRE
[[noreturn]] void jv::util::Panic(std::string_view errorMessage) noexcept
{
    std::string msg{errorMessage};
    fputs(msg.c_str(), stderr);
    MessageBoxA(
        g_hWindow, msg.c_str(), "Error", MB_SYSTEMMODAL | MB_SETFOREGROUND | MB_ICONERROR | MB_OK);
#ifdef _MSC_VER
    __debugbreak();
#else
    //__builtin_trap();
#endif
    TerminateProcess(GetCurrentProcess(), -1);
    std::terminate();
}
