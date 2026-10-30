#include "jv/core/jv_ui_text.h"
#include "dino/dino_draw_utils.h"

namespace jv::core
{

UiText::UiText(
    Vec2 pos, jv::gpu::Texture* pFontTexture, jv::util::Color textColor, jv::util::Color backgroundColor)
    : GameObject(pos)
{
    m_pTexture = pFontTexture;
    m_textColor = textColor;
    m_backgroundColor = backgroundColor;

    SetSortingOrder(100); // élément d'UI donc à afficher au dessus du reste
}

void UiText::SetText(const std::string& text)
{
    m_textBinding = nullptr;
    m_currentText = text;
}

void UiText::BindText(TextBinding binding)
{
    m_textBinding = std::move(binding);
}

void UiText::Update(double absTime, float deltaTime)
{
    if (m_textBinding)
    {
        m_currentText = m_textBinding();
    }
}

void UiText::Draw() const
{
    if (m_currentText.empty()) return;

    std::vector<jv::gpu::Vertex> vs;

    dino::GenVertices_Text(vs, m_currentText, m_textColor, m_backgroundColor, GetPos());

    jv::gpu::VertexBuffer* pVBuf = jv::gpu::CreateVertexBuffer("UiText", vs);
    jv::gpu::Draw(pVBuf, m_pTexture);
    jv::gpu::DestroyVertexBuffer(pVBuf);
}

} // namespace jv::core