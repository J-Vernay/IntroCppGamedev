#pragma once
#include <dino/dino_main.h>

namespace dino
{
class Scene;
class Pause
{
public:
    int8_t m_selectedOption = 0;
    /// Gere les inputs specific au menu
    void Update(Scene& scene, double absTime, float deltaTime);

    /// Affiche le menu
    void Draw(float timer) const;

private:
    
    bool m_pauseWasPressed = false;
    bool m_downWasPressed = false;
    bool m_upWasPressed = false;
    bool m_rightWasPressed = false;
    bool m_leftWasPressed = false;
};
}