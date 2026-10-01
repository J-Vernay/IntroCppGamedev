#pragma once

#include <dino/dino_main.h>

namespace dino
{

// Représente un joueur.
    class Player
    {
    public:
        /// Initialise le joueur avec un type au hasard.
        Player(Vec2 pos, double absTime, jv::gpu::Texture* pTexture);

        /// Constructeur d'un Player à partir d'un autre Player par copie
        ///
        /// "Player a = b;" ou "Player a{b};" (on construit "a" à partir de "b")
        Player(Player const& other) = delete;

        /// Opérateur d'assignement par copie
        ///
        /// Player a, b;
        /// a = b; // Pas un constructeur, car 'a' et 'b' existe déjà ; c'est une affectation
        Player& operator=(Player const& b) = delete;

        /// Déplace le joueur et met à jour son animation.
        void Update(double absTime, float deltaTime);

        /// Affiche le joueur
        void Draw() const;

        /// Détruit les ressources associées au joueur.
        ~Player();

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