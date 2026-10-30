#pragma once
#include <jv/jv.h>

namespace dino
{

class InputHandler
{
public:
    InputHandler(jv::input::GamepadIdx gamepadIdx);
    ~InputHandler() = default;

    /// Prend une snapshot de l'état du controller sur cette frame
    /// Cela comprend le gamepad state, la direction et l'état de connection
    void CaptureInputs();

    /// Met à jour l'état de connexion du joueur selon ses inputs sur cette frame
    void CaptureConnectionInputs();

    /// État complet de la manette en lecture seule
    jv::input::Gamepad const& GetGamepadState() const { return m_gamepadState; }

    jv::util::Vec2 GetDir() const { return m_dir; }

    bool IsConnected() const { return m_isConnected; }

    /// Dit si le bouton viens juste d'être préssé à cette frame
    /// @example
    /// IsButtonJustPressed([](auto const& pad){ return pad.start; })
    template <typename Func>
    bool IsButtonJustPressed(Func buttonSelector) const
    {
        return buttonSelector(m_gamepadState) && !buttonSelector(m_lastGamepadState);
    }

private:
    jv::input::GamepadIdx m_gamepadIdx;
    jv::input::Gamepad m_gamepadState{};
    jv::input::Gamepad m_lastGamepadState{};
    jv::util::Vec2 m_dir{};
    bool m_isConnected = false;
};

} // namespace dino