#include <dino/dino_pause.h>
#include <dino/dino_scene.h>
#include <dino/dino_draw_utils.h>
#include <dino/dino_assets.h>
#include <format>

void dino::Pause::Update(Scene& scene, double absTime, float deltaTime)
{
    jv::input::GamepadIdx gamePads[4] = {
        jv::input::GamepadIdx::Keyboard,
        jv::input::GamepadIdx::Gamepad1, 
        jv::input::GamepadIdx::Gamepad2,
        jv::input::GamepadIdx::Gamepad3
    };
    for (jv::input::GamepadIdx idx : gamePads)
    {
        jv::input::Gamepad gamepad;
        if (jv::input::GetGamepad(idx, gamepad))
        {
            if (gamepad.start && scene.m_game && !m_pauseWasPressed && !scene.m_pause) // Pause game (not in lobby)
            {
                scene.SetPause();
            }
            if (scene.m_pause) // Handle pause menu inputs
            {
                if (gamepad.dpad_down && !m_downWasPressed)
                {
                    m_selectedOption++;
                    if (m_selectedOption > 3)
                        m_selectedOption = 0;
                }
                else if (gamepad.dpad_up && !m_upWasPressed)
                {
                    m_selectedOption--;
                    if (m_selectedOption < 0)
                        m_selectedOption = 3;
                }
                if (m_selectedOption == 2)
                {
                    if (gamepad.dpad_left && !m_leftWasPressed)
                    {
                        scene.m_timer -= 10;
                        if (scene.m_timer < 0)
                            scene.m_timer = 0;
                    }
                    else if (gamepad.dpad_right && !m_rightWasPressed)
                    {
                        scene.m_timer += 10;
                    }
                }
                else if (gamepad.btn_right)
                {
                    if (m_selectedOption == 0)
                        scene.StartGame(scene.GetTerrain().GetSeason());
                    if (m_selectedOption == 1)
                        scene.StartLobby(absTime);
                    if (m_selectedOption == 3)
                        scene.m_pause = false;
                }
                
            }
            
        }
    }
    m_pauseWasPressed = false; m_downWasPressed = false; m_upWasPressed = false; m_rightWasPressed = false; m_leftWasPressed = false;
    for (jv::input::GamepadIdx idx : gamePads)
    {
        jv::input::Gamepad gamepad;
        if (jv::input::GetGamepad(idx, gamepad))
        {
            m_pauseWasPressed = m_pauseWasPressed || gamepad.start;
            m_downWasPressed = m_downWasPressed || gamepad.dpad_down;
            m_upWasPressed = m_upWasPressed || gamepad.dpad_up;
            m_rightWasPressed = m_rightWasPressed || gamepad.dpad_right;
            m_leftWasPressed = m_leftWasPressed || gamepad.dpad_left;
        }
    }

    }


void dino::Pause::Draw(float timer) const
{
    {
        std::vector<jv::gpu::Vertex> vs;
        jv::util::Color color = {Color_WHITE};
        color.a = 128;
        Vec2 screenSize = jv::gpu::GetRenderSize();
        vs.emplace_back(Vec2{0, 0}, Vec2{0, 0}, color);
        vs.emplace_back(Vec2{screenSize.x, 0}, Vec2{0, 0}, color);
        vs.emplace_back(Vec2{0, screenSize.y}, Vec2{0, 0}, color);
        vs.emplace_back(Vec2{screenSize.x, 0}, Vec2{0, 0}, color);
        vs.emplace_back(Vec2{0, screenSize.y}, Vec2{0, 0}, color);
        vs.emplace_back(Vec2{screenSize.x, screenSize.y}, Vec2{0, 0}, color);
        jv::gpu::VertexBuffer* pVBuf = jv::gpu::CreateVertexBuffer("white_bg", vs);
        jv::gpu::Draw(pVBuf, (&dino::AssetsHolder::getInstance())->g_Textures["white"]);
        jv::gpu::DestroyVertexBuffer(pVBuf);
    }
    {
        std::vector<jv::gpu::Vertex> vs;
        dino::GenVertices_Text(vs, "Recommencer", m_selectedOption == 0 ? Color_RED : Color_WHITE,
            Color_GREY, {jv::gpu::GetRenderSize().x / 2 - 30, 100});
        jv::gpu::VertexBuffer* pVBuf = jv::gpu::CreateVertexBuffer("optionText1", vs);
        jv::gpu::Draw(pVBuf, (&dino::AssetsHolder::getInstance())->g_Textures["text"]);
        jv::gpu::DestroyVertexBuffer(pVBuf);
    }
    {
        std::vector<jv::gpu::Vertex> vs;
        dino::GenVertices_Text(vs, "Retour au lobby",
            m_selectedOption == 1 ? Color_RED : Color_WHITE, Color_GREY,
            {jv::gpu::GetRenderSize().x / 2 - 45, 150});
        jv::gpu::VertexBuffer* pVBuf = jv::gpu::CreateVertexBuffer("optionText2", vs);
        jv::gpu::Draw(pVBuf, (&dino::AssetsHolder::getInstance())->g_Textures["text"]);
        jv::gpu::DestroyVertexBuffer(pVBuf);
    }
    {
        std::vector<jv::gpu::Vertex> vs;
        std::string text = std::format("Chrono: {:04.1f}sec", timer);
        dino::GenVertices_Text(vs, text, m_selectedOption == 2 ? Color_RED : Color_WHITE,
            Color_GREY, {jv::gpu::GetRenderSize().x / 2 - 45, 200});
        jv::gpu::VertexBuffer* pVBuf = jv::gpu::CreateVertexBuffer("optionText3", vs);
        jv::gpu::Draw(pVBuf, (&dino::AssetsHolder::getInstance())->g_Textures["text"]);
        jv::gpu::DestroyVertexBuffer(pVBuf);
    }
    {
        std::vector<jv::gpu::Vertex> vs;
        dino::GenVertices_Text(vs, "Reprendre", m_selectedOption == 3 ? Color_RED : Color_WHITE,
            Color_GREY, {jv::gpu::GetRenderSize().x / 2 - 25, 250});
        jv::gpu::VertexBuffer* pVBuf = jv::gpu::CreateVertexBuffer("optionText4", vs);
        jv::gpu::Draw(pVBuf, (&dino::AssetsHolder::getInstance())->g_Textures["text"]);
        jv::gpu::DestroyVertexBuffer(pVBuf);
    }
}