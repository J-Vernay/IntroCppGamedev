#pragma once

#include <dino/dino_main.h>
#include <dino/dino_terrain.h>
#include <dino/Entity.h>

namespace dino
{

// Représente un joueur.
    class Player : public Entity {
    public:
        /// Initialise le joueur avec un type au hasard.
        Player(Vec2 pos, int32_t idxPlayer, jv::input::GamepadIdx gamepadIdx,
            jv::gpu::Texture* pTexture);

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
        void Update(double absTime, float deltaTime, Terrain const& terrain);

        void LassoUpdate();
        void LassoDraw() const;
        bool FindLassoLoop(std::vector<Vec2>& outLoop) const;
        bool PollStartPressed();
        void ClearLasso(){ m_lasso.clear(); }
        int GetId() const{ return id; }

        bool PollPauseToggle();

        void ResetState()
        {
            m_lasso.clear();
            m_hitTime = 0;
          
        }

        void Caught() override;

        /// Affiche le joueur
        void Draw() const;

        /// Détruit les ressources associées au joueur.
        ~Player();
           

    private:

        static constexpr size_t kLassoMaxPoints = 100;
        static constexpr float kLassoMinStep = 4.f;
        static constexpr float kLassoWidth = 4.f;
        Vec2 m_lassoPrevAnchor = {0, 0};
        jv::util::Color m_playerColor;
        bool m_bStartWasDown = false;
        bool m_bPauseWasDown = false;


        jv::util::Color m_playersColors[4] = {Color_BLUE, Color_RED, Color_YELLOW, Color_GREEN};
        int32_t m_idxPlayer;
        jv::input::GamepadIdx m_gamepadIdx;
        
        Vec2 m_dir;
        jv::gpu::Texture* m_pTexture;
        double m_hitTime = 0;
        double m_absTime = 0;
        bool m_bRunning = false;
        bool m_bLeft = false;

        int id;

        std::vector<Vec2> m_lasso;

    };

} // namespace dino