#pragma once

#include <dino/dino_main.h>

namespace dino
{

// Représente un animal.
class DinoPlayer
{
public:
    /// Initialise l'animal avec un type au hasard.
    DinoPlayer(Vec2 pos, double absTime, jv::gpu::Texture* text);

    /// Deplace l'animal et met à jour son animation.
    void Update(double absTime, float deltaTime);

    /// Affiche l'animal
    void Draw() const;

    /// Detruit les ressources associées à l'animal.
    ~DinoPlayer();

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