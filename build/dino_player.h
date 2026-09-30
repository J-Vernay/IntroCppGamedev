#pragma once

#include <dino/dino_main.h>

namespace dino
{

    // Représente un animal.
    class Player
    {
    public:
        /// Initialise l'animal avec un type au hasard.
        Player(Vec2 pos, double absTime, jv::gpu::Texture* tex);

        /// Déplace l'animal et met à jour son animation.
        void Update(double absTime, float deltaTime);

        /// Affiche l'animal
        void Draw() const;

        /// Détruit les ressources associées à l'animal.
        ~Player();

        Player(const Player&) = delete;

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