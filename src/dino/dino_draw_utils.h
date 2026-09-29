/// @file dino_draw_utils.h
/// @brief Fonctions pour dessiner des primitives de rendu.
/// @author Julien Vernay

#pragma once

#include <dino/dino_main.h>

namespace dino
{

/// Essaye de charger une image BMP depuis le dossier "assets'.
/// En cas d'erreur, retourne 'nullptr'.
/// En cas de succès, l'appelant doit appeler 'jv::gpu::DestroyTexture'.
jv::gpu::Texture* LoadImageAsset(std::string_view imageName);

/// @name Fonctions GenVertices
/// Ces fonctions génèrent des sommets dans 'out', pour être ensuite ajouté à des draw calls avec
/// des texIDs particuliers, et avec `translation`, `rotation` et `scale`.
/// @{

/// Produit un dessin contenant du texte, avec éventuellement une couleur de fond.
/// Les sommets doivent être dessiné avec m_pTextureText.
///
/// @param out Destination dans laquelle sont ajoutés les sommets.
/// @param text Caractères à afficher.
/// @param color Couleur du texte.
/// @param colorBackground Couleur du rectangle affiché derrière le texte.
/// @param pos Position du coin supérieur gauche, en pixels.
/// @return La taille en pixels de la zone de texte.
Vec2 GenVertices_Text(std::vector<jv::gpu::Vertex>& out, std::string_view text,
    jv::util::Color color = Color_WHITE, jv::util::Color colorBackground = Color_INVISIBLE,
    Vec2 pos = {0, 0});

/// Produit un dessin contenant une liste de segments, tous reliés.
/// Les sommets doivent être dessiné avec XDino_TEXID_WHITE.
///
/// @param out Destination dans laquelle sont ajoutés les sommets.
/// @param points Liste de points par lesquels la polyligne passe.
/// @param width Epaisseur du trait, en pixels.
/// @param color Couleur du trait.
void GenVertices_Polyline(std::vector<jv::gpu::Vertex>& out, std::vector<Vec2> const& points,
    float width, jv::util::Color color = Color_WHITE);

/// Ajoute le dessin d'un rectangle au drawcall donné.
/// @param topLeft Coordonnées en haut à gauche à l'écran.
/// @param size Nombre de pixels de largeur et hauteur.
/// @param topLeftUV Coordonnées en haut à gauche sur la texture d'origine.
/// @param color Couleur qui module le sprite, WHITE pour le laisser tel quel.
void GenVertices_Rect(std::vector<jv::gpu::Vertex>& vertices, Vec2 topLeft, Vec2 size,
    Vec2 topLeftUV, jv::util::Color color = Color_WHITE);

/// @}

} // namespace dino
