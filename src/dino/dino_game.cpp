#include <algorithm>
#include <dino/dino_draw_utils.h>
#include <dino/dino_game.h>
#include <dino/dino_geometry.h>
#include <dino/dino_lasso_system.h>
#include <dino/dino_main.h>
#include <jv/core/jv_ui_text.h>
#include <format>

dino::DinoGame::DinoGame()
{
    m_pTextureText = dino::LoadImageAsset("monogram-bitmap.bmp");
    m_pTexturePlayer = dino::LoadImageAsset("dinosaurs.bmp");
    m_pTextureAnimal = dino::LoadImageAsset("animals.bmp");
    m_pTextureEnvironment = dino::LoadImageAsset("terrain.bmp");

    // Création du terrain
    m_terrain = m_scene.CreateObject<Terrain>(24, 16, m_pTextureEnvironment);
    m_scene.SetRestrictedSpace(m_terrain);

    // Création des 4 joueurs et de leurs lassos
    for (int32_t i = 0; i < 4; ++i)
    {
        Lasso* pLasso = m_scene.CreateObject<Lasso>(m_playerColors[i]);

        Player* pPlayer = m_scene.CreateObject<Player>( m_terrain->GenerateRandomSpawn(), i, &m_allGamepads[i], m_pTexturePlayer, pLasso);
        pPlayer->AddChild(pLasso);

        m_players.push_back(pPlayer); //on garde un accès non propriétaire
    }

    // Création des arbres du lobby
    for (int32_t i = 0; i < 4; ++i)
    {
        Tree* pTree = m_scene.CreateObject<Tree>(m_terrain->GenerateRandomSpawn(), i, m_pTextureEnvironment);

        m_trees.push_back(pTree);
    }

    // Création du texte affichant les fps (+ lien dynamique avec lastDeltaTime)
    jv::core::UiText* pFpsText =
        m_scene.CreateObject<jv::core::UiText>(Vec2(10.0f, 10.0f), m_pTextureText, Color_WHITE, Color_GREY);
    pFpsText->BindText(
        [this]() { return std::format("dTime={:04.1f}ms", m_lastDeltaTime * 1000.0); });

    // Création du texte affichant le score pour chaque joueur (+ lien dynamique avec leur score)
    for (int32_t i = 0; i < 4; ++i)
    {
        Vec2 scorePos = Vec2(10.0f, (i+1) * (jv::gpu::GetRenderSize().y / 5));

        jv::core::UiText* pScoreText = m_scene.CreateObject<jv::core::UiText>(scorePos, m_pTextureText, Color_WHITE, m_playerColors[i]);

        Player* pPlayer = m_players[i]; // on récupére l'adresse mémoire du joueur
        
        pScoreText->BindText([pPlayer, i]() -> std::string {
            if (pPlayer->IsActive())
            {
                return std::format("P{}={:04}", i + 1, pPlayer->GetScore());
            }
            else
            {
                return std::format("P{}=DECONNECTE", i + 1);
            }
        });

        m_scoreTexts.push_back(pScoreText); //on garde un accès non propriétaire
    }

    // Création du texte affichant le timer (+ lien dynamique avec currentTimer)
    Vec2 timerPos = Vec2(jv::gpu::GetRenderSize().x / 2, 10.0f);
    jv::core::UiText* pTimerText = m_scene.CreateObject<jv::core::UiText>(timerPos, m_pTextureText, Color_WHITE, Color_GREY);
    pTimerText->BindText(
        [this]() -> std::string { return std::format("{:.0f}s", m_currentTimer); });
    m_timerText = pTimerText;//on garde un accès non propriétaire

    // LANCEMENT GLOBAL DU JEU
    ChangeState(GameState::LOBBY); // on demarre le jeu sur le lobby
}

dino::DinoGame::~DinoGame()
{
    jv::gpu::DestroyTexture(m_pTexturePlayer);
    jv::gpu::DestroyTexture(m_pTextureAnimal);
    jv::gpu::DestroyTexture(m_pTextureText);
    jv::gpu::DestroyTexture(m_pTextureEnvironment);
}

void dino::DinoGame::Update(double absTime, float deltaTime)
{
    m_lastDeltaTime = deltaTime;

    // INPUTS
    for (InputHandler& inputHandler : m_allGamepads)
    {
        inputHandler.CaptureInputs(); // Récupèration des inputs sur cette frame
        if (m_currentState == GameState::GAME && inputHandler.IsButtonJustPressed([](auto const& pad) { return pad.start; }))
            isInPause = !isInPause;
    }

    for (size_t i = 0; i < m_players.size(); ++i)
        m_players[i]->SetActive(m_allGamepads[i].IsConnected()); // l'activation ou non d'un joueur dépend de si son controller est connecté

    if (isInPause) return; // ON STOPPE LE RESTE DE L'EXECUTION

    // Nettoyage des pointeurs morts dans les listes d'accès rapide
    CleanDeadReferences();

    // Update de la scène (met à jour tous les GameObjects actifs et gère la destruction)
    m_scene.Update(absTime, deltaTime);

    // Résolution physique de la scène
    m_scene.PhysicsUpdate();

    // Application des règles du lasso
    std::vector<LassoLoop> closedLoops = LassoSystem::UpdateIntersections(m_players);
    LassoSystem::ProcessCaptures(closedLoops, m_scene.GetObjects());

    // Mise à jour des systèmes propres à l'état en cours
    switch (m_currentState)
    {
    case GameState::LOBBY:
        UpdateLobby(absTime, deltaTime);
        break;
    case GameState::GAME:
        UpdateGame(absTime, deltaTime);
        break;
    }
}

void dino::DinoGame::SpawnAnimal(double absTime)
{
    Vec2 spawnPos = m_terrain->GenerateRandomSpawn();
    Animal* pAnimal = m_scene.CreateObject<Animal>(spawnPos, absTime, m_pTextureAnimal);
    m_animals.push_back(pAnimal);
}

void dino::DinoGame::CleanDeadReferences()
{
    // Retire les objets qui ont été marqués morts et détruits par la scène
    std::erase_if(m_animals, [](Animal const* a) { return a->IsDead(); });
    // Je ne met que les animaux car ce sont les seuls suceptibles d'etre détruit complétement en mémoire pour l'instant
    // Mais une logique similaire pourrait etre mise en place pour d'autres objets
}

void dino::DinoGame::ChangeState(GameState newState)
{
    if (m_currentState == newState) return;

    // On quitte l'état actuel
    switch (m_currentState)
    {
    case GameState::LOBBY:
        ExitLobby();
        break;
    case GameState::GAME:
        ExitGame();
        break;
    }

    // Changement d'état
    m_currentState = newState;

    // On entre dans le nouvel état
    switch (m_currentState)
    {
    case GameState::LOBBY:
        EnterLobby();
        break;
    case GameState::GAME:
        EnterGame();
        break;
    }
}

void dino::DinoGame::EnterLobby()
{
    for (Tree* const& pTree : m_trees)
    {
        pTree->ResetState();
        pTree->SetActive(true);
        pTree->DisableSelectionDuring(5.0f);
    }

    // Load default background season
    m_terrain->SetSeason(0);
}

void dino::DinoGame::ExitLobby()
{
    for (Tree* const& pTree : m_trees)
        pTree->SetActive(false);
}

void dino::DinoGame::UpdateLobby(double absTime, float deltaTime)
{
    for (Tree* const& pTree : m_trees)
    {
        if (pTree->IsSelected()) // si un arbre est selectionné
        {
            m_currentSeason = pTree->SeasonIdx; // on enregistre sa saison
            ChangeState(GameState::GAME); // puis on lance la partie
        }
    }

    // CONNEXION
    for (InputHandler& inputHandler : m_allGamepads)
        inputHandler.CaptureConnectionInputs(); // Laisse le joueur mettre à jour sa connexion via les inputs
}

void dino::DinoGame::EnterGame()
{
    // TIMER
    m_currentTimer = m_timerDuration;
    m_timerText->SetActive(true);

    // Reset scores
    for (Player* const& pPlayer : m_players)
        pPlayer->ResetScore();

    // Load choisen background season
    m_terrain->SetSeason(m_currentSeason);
}

void dino::DinoGame::ExitGame()
{
    // TIMER
    m_timerText->SetActive(false);

    // Disable pause security
    isInPause = false;

    // On tue tout les animaux restant
    for (Animal* const& pAnimal : m_animals)
        pAnimal->Destroy();

}

void dino::DinoGame::UpdateGame(double absTime, float deltaTime)
{
    // TIMER
    m_currentTimer -= deltaTime;
    if (m_currentTimer < 0)
        ChangeState(GameState::LOBBY);

    // SPAWNER UN ANIMAL SI BESOIN
    double timeRatio = std::max(0.0, (double)m_currentTimer / (double)m_timerDuration);
    double currentSpawnTime = kMinSpawnTime + timeRatio * (kMaxSpawnTime - kMinSpawnTime);// Interpolation linéaire
    if (absTime - m_currentAnimalSpawnTime >= currentSpawnTime)
    {
        m_currentAnimalSpawnTime = absTime;
        SpawnAnimal(absTime);
    }
}

void dino::DinoGame::Draw() const
{
    // La scène trie l'ordre de rendu (avec les layers "sorting order" et Y) puis dessine tout les objets de la scene de manière homogéne
    m_scene.Draw();
}
