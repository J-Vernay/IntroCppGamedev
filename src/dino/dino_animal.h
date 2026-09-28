#pragma once

#include <dino/dino_main.h>

namespace dino
{

// Représente un animal.
class Animal
{
public:
    /// Initialise l'animal avec un type au hasard.
    Animal(Vec2 pos, double absTime);
    
    void Update(double absTime, float deltaTime);

    void Draw() const;

    void Shut();

    /// Nom de la texture qui contient les animaux.
    static const std::string TEXTURE_NAME;

private:
    Vec2 m_pos;
    double m_timeStart;
    uint8_t m_alpha = 0;
    Vec2 m_dir;
    int32_t m_kind;
    int32_t m_idxFrame;
    jv::gpu::Texture* m_pTexture;
};

} // namespace dino