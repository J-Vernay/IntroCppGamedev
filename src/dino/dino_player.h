#pragma once

#include <jv/core/jv_gameobject.h>
#include <dino/dino_lasso_target.h>
#include <dino/dino_lasso.h>
#include <dino/dino_main.h>
#include <dino/dino_input_handler.h>

namespace dino
{

class Player : public jv::core::GameObject, public ILassoTarget
{
public:
    /// Initialise le joueur
    Player(jv::util::Vec2 pos, int32_t idxPlayer, InputHandler* pInputHandler,
        jv::gpu::Texture* pTexture, Lasso* pLasso);

    /// Constructeur par copie désactivé : application de la sémantique d'entitée
    Player(Player const& other) = delete;

    /// Opérateur d'assignation par copie désactivé : application de la sémantique d'entitée
    Player& operator=(Player const& b) = delete;

    ~Player() override = default;

    /// Récupére les commandes et déplace le joueur + ajoute sa position dans le lasso
    void Update(double absTime, float deltaTime) override;

    /// Anime et affiche le joueur
    void Draw() const override;

    Lasso* GetLasso() const { return m_pLasso; }

    void OnCaughtInLasso(Player* pLassoOwner) override { m_hitTime = 3.0; } // Met KO le joueur 3 secondes

    void ResetScore() { m_currentScore = 0; }
    int GetScore() { return m_currentScore; }
    void AddScore(int amount) { m_currentScore += amount; }

private:
    int32_t m_idxPlayer;
    InputHandler* m_pInputHandler;
    jv::util::Vec2 m_dir;
    jv::gpu::Texture* m_pTexture;
    Lasso* m_pLasso = nullptr;

    double m_hitTime = 0.0;
    double m_absTime = 0.0;
    bool m_bRunning = false;
    bool m_bLeft = false;

    int m_currentScore = 0;
};

} // namespace dino