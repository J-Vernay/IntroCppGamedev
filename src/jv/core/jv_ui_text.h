#pragma once
#include "jv/core/jv_gameobject.h"
#include <functional>
#include <string>

namespace jv::core
{
class UiText : public GameObject
{
public:
    using TextBinding = std::function<std::string()>;
    using Color = jv::util::Color;

    UiText(Vec2 pos, jv::gpu::Texture* pFontTexture, Color textColor, Color backgroundColor);
    ~UiText() override = default;

    void Update(double absTime, float deltaTime) override;
    void Draw() const override;

    // Sert à afficher un texte statique
    void SetText(const std::string& text);

    // Sert à lier le texte à une variable
    void BindText(TextBinding binding);

private:
    jv::gpu::Texture* m_pTexture;
    std::string m_currentText;
    TextBinding m_textBinding = nullptr;
    jv::util::Color m_textColor;
    jv::util::Color m_backgroundColor;
};

} // namespace jv::core