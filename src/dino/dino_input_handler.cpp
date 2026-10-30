#include "dino_input_handler.h"

dino::InputHandler::InputHandler(jv::input::GamepadIdx gamepadIdx)
{
    m_gamepadIdx = gamepadIdx;
}

void dino::InputHandler::CaptureInputs()
{
    // sauvegarde de l'état avant capture
    m_lastGamepadState = m_gamepadState;

    if (jv::input::GetGamepad(m_gamepadIdx, m_gamepadState))
    {
        float dirx = 0, diry = 0;

        if (m_gamepadState.dpad_up)
            diry -= 1;
        if (m_gamepadState.dpad_down)
            diry += 1;
        if (m_gamepadState.dpad_left)
            dirx -= 1;
        if (m_gamepadState.dpad_right)
            dirx += 1;

        m_dir = {dirx, diry};
    }
    else
    {
        m_dir = {};
        m_gamepadState = {};
        m_isConnected = false; // si le peripherique est déconnecté
    }
}

void dino::InputHandler::CaptureConnectionInputs()
{
    if (m_gamepadState.start)
        m_isConnected = true;
    if (m_gamepadState.select)
        m_isConnected = false;
}
