#pragma once

#include <span>
#include <stdint.h>
#include <string_view>
#include <vector>

namespace jv
{

/// Fonctions à implémenter côté logique de jeu, qui seront appelés par le moteur (jv_engine).
namespace game
{

/// Doit initialiser les systèmes gameplay.
/// En cas d'erreur, il faut appeler `jv::util::Panic()`.
void Init();

/// Doit traiter les événements et mettre à jour la simulation gameplay.
/// En cas d'erreur, il faut appeler `jv::util::Panic()`.
void Update(double absTime, float deltaTime);

/// Doit émettre les commandes de dessin du jeu.
/// En cas d'erreur, il faut appeler `jv::util::Panic()`.
void Draw();

/// Doit libérer les ressources associées aux systèmes gameplay.
/// En cas d'erreur, il faut appeler `jv::util::Panic()`.
void Shut();

} // namespace game

/// Types et fonctions utilitaires pour la logique de jeu.
namespace util
{

/// Affiche une erreur et déclenche le debugger, puis ferme le jeu.
[[noreturn]] void Panic(std::string_view errorMessage) noexcept;

/// Représente une coordonnée 2D en pixels.
struct Vec2
{
    float x;
    float y;
};

/// Représente une couleur de pixels, utilisée pour moduler l'affichage d'une texture.
union Color {
    struct
    {
        uint8_t r;
        uint8_t g;
        uint8_t b;
        uint8_t a;
    };

    uint32_t rgba;
};

/// Génère un entier 32-bit non-signé pseudo-aléatoire entre 'min' et 'max' inclus.
uint32_t RandomUint32(uint32_t min, uint32_t max) noexcept;

/// Génère un entier 32-bit signé pseudo-aléatoire entre 'min' et 'max' inclus.
int32_t RandomInt32(int32_t min, int32_t max) noexcept;

/// Génère un nombre à virgule flottante pseudo-aléatoire entre 'min' inclus et 'max' exclus.
float RandomFloat(float min, float max) noexcept;

/// Tourne le vecteur donné en paramètre d'un angle aléatoire compris entre 'angleMin' inclus et
/// 'angleMax' exclus, en degrés.
Vec2 RandomRotate(Vec2 vec, float angleMin, float angleMax) noexcept;

/// Lit un fichier du dossier "assets" et le stocke en mémoire.
/// Retourne un fichier vide en cas d'erreur.
std::vector<unsigned char> LoadAsset(std::string_view fileName);

} // namespace util

namespace gpu
{

/// Change les dimensions en pixels de la résolution de rendu.
void SetRenderSize(util::Vec2 renderSize) noexcept;

/// Retourne la précédente valeur de "SetRenderSize".
util::Vec2 GetRenderSize() noexcept;

/// Change la couleur par défaut de l'arrière-plan du rendu.
void SetBackgroundColor(util::Color color) noexcept;

/// Type qui représente une texture chargée côté GPU.
struct Texture;

/// Crée une texture avec les dimensions et pixels donnés, et envoit les données à la carte
/// graphique.
/// @param label Nom donné à la texture, utilisé pour le debug.
/// @param pixels Doit contenir une liste de `largeur * hauteur` couleurs représentant
///               les valeurs RGBA de chaque pixel, de gauche à droite et de bas en haut.
/// @note Après le retour de la fonction, 'pPixelsRGBA' ne sera plus accédé et peut être libéré.
Texture* CreateTexture(
    std::string_view label, util::Vec2 textureSize, std::span<util::Color const> pixels);

/// Détruit la texture et libère la mémoire consommée par cette texture.
void DestroyTexture(Texture* pTexture);

/// Type qui représente un sommet de triangle texturé.
struct Vertex
{
    util::Vec2 pos;
    util::Vec2 uv;
    util::Color color = {255, 255, 255, 255};
};

/// Type qui représente une liste de sommets chargé côté GPU.
struct VertexBuffer;

/// Crée un vertex buffer avec la taille et contenu de vertices donnés, et les envoie à la carte
/// graphique.
/// @param label Nom donné au vertex buffer, utilisé pour le debug.
/// @note Après le retour de la fonction, 'pVertices' ne sera plus acédé et peut être libéré.
VertexBuffer* CreateVertexBuffer(std::string_view label, std::span<Vertex const> vertices);

/// Détruit le vertex buffer et libère la mémoire consommée par ce vertex buffer.
void DestroyVertexBuffer(VertexBuffer* pVertexBuffer);

/// Représente une transformation à appliquer à tous les sommets d'un vertex buffer avant de
/// commencer l'affichage.
struct Transform
{
    util::Vec2 translation = {0, 0};
    float rotation = 0;
    util::Vec2 scale = {1, 1};
};

/// Envoit une demande de rendu de triangles texturés à la carte graphique.
void Draw(VertexBuffer* pVertices, Texture* pTexture, Transform transform = {});

} // namespace gpu

namespace input
{

/// Identifiant de manette (pour ce cours, le clavier est considéré comme un type de manettes).
enum class GamepadIdx : int32_t
{
    Keyboard,
    Gamepad1,
    Gamepad2,
    Gamepad3,
    Gamepad4,
};

constexpr GamepadIdx GamepadIdx_ALL[] = {
    GamepadIdx::Gamepad1,
    GamepadIdx::Gamepad2,
    GamepadIdx::Gamepad3,
    GamepadIdx::Gamepad4,
    GamepadIdx::Keyboard,
};

/// Structure contenant l'état d'une manette (ou du clavier utilisé comme manette).
struct Gamepad
{
    bool dpad_up : 1;        ///< Si clavier : Flèche du haut
    bool dpad_left : 1;      ///< Si clavier : Flèche gauche
    bool dpad_right : 1;     ///< Si clavier : Flèche droite
    bool dpad_down : 1;      ///< Si clavier : Flèche du bas
    bool btn_up : 1;         ///< Si clavier : Z ou W
    bool btn_left : 1;       ///< Si clavier : Q ou A
    bool btn_right : 1;      ///< Si clavier : D
    bool btn_down : 1;       ///< Si clavier : S
    bool start : 1;          ///< Si clavier : ESPACE ou ENTREE
    bool select : 1;         ///< Si clavier : SHIFT
    bool shoulder_left : 1;  ///< Si clavier : CTRL
    bool shoulder_right : 1; ///< Si clavier : ALT

    util::Vec2 stick_left;  ///< Entre -1 et 1. Si clavier : dérivé de 'dpad'
    util::Vec2 stick_right; ///< Entre -1 et 1. Si clavier : dérivé de 'btn'

    util::Vec2 mouse; ///< Si clavier : position en pixels.
};

/// Récupère l'état de la manette concernée.
/// @param idx Quelle manette regarder, ou le clavier avec `jv::input::GamepadIdx::Keyboard`.
/// @param outGamepad Où sont stockées les valeurs récupérées.
/// @return `true` quand réussit, `false` si non-disponiIble.
bool GetGamepad(GamepadIdx idx, Gamepad& outGamepad) noexcept;

} // namespace input

} // namespace jv
