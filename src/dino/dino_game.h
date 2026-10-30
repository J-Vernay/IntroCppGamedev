#pragma once

#include <dino/dino_animal.h>
#include <dino/dino_lasso.h>
#include <dino/dino_player.h>
#include <dino/dino_terrain.h>
#include <dino/dino_tree.h>
#include <jv/core/jv_scene.h>
#include <dino/dino_input_handler.h>
#include <vector>

namespace dino
{

enum class GameState // etats de jeu possibles
{
    LOBBY,
    GAME
};

class DinoGame
{
public:
    /// Charge les ressouces et objets nécessaires au jeu
    DinoGame();
    ~DinoGame();

    void Update(double absTime, float deltaTime);
    void Draw() const;

    void ChangeState(GameState newState); // gestion propre du changement d'état

private:
    jv::core::Scene m_scene;

    // Attribution d'une couleur selon l'index du joueur (0 à 3)
    const jv::util::Color m_playerColors[4] = {Color_BLUE, Color_RED, Color_YELLOW, Color_GREEN};

    // Ressources graphiques
    jv::gpu::Texture* m_pTextureText = nullptr;
    jv::gpu::Texture* m_pTextureAnimal = nullptr;
    jv::gpu::Texture* m_pTexturePlayer = nullptr;
    jv::gpu::Texture* m_pTextureEnvironment = nullptr;

    float m_lastDeltaTime = 0.0f;
    int m_currentSeason = 0;

    // Listes de pointeurs non propriétaires pour garder un accès rapide
    std::vector<Player*> m_players;
    std::vector<Animal*> m_animals;
    std::vector<Tree*> m_trees;
    Terrain* m_terrain;
    std::vector<jv::core::GameObject*> m_scoreTexts; //je les stocke sous forme de GameObject car je n'ai pas besoin d'avoir accès à leur surcouche UiText ici
    jv::core::GameObject* m_timerText;

    // gestion propre des infos envoyés par les périphériques connectés ou non
    InputHandler m_allGamepads[4] = {
        InputHandler(jv::input::GamepadIdx::Keyboard),
        InputHandler(jv::input::GamepadIdx::Gamepad1),
        InputHandler(jv::input::GamepadIdx::Gamepad2),
        InputHandler(jv::input::GamepadIdx::Gamepad3)
    };

    void CleanDeadReferences();

    GameState m_currentState = GameState::GAME;

    // LOBBY
    void EnterLobby();
    void ExitLobby();
    void UpdateLobby(double absTime, float deltaTime);

    // GAME
    bool isInPause = false;
    const float m_timerDuration = 60.0f;
    float m_currentTimer;
    double m_currentAnimalSpawnTime = 0.0;
    const double kMaxSpawnTime = 1.5;
    const double kMinSpawnTime = 0.25;
    void EnterGame();
    void ExitGame();
    void UpdateGame(double absTime, float deltaTime);
    void SpawnAnimal(double absTime);
};

} // namespace dino