#pragma once
#include <jv/jv.h>

namespace exo4
{

struct Image
{
    jv::gpu::Texture* pTexture;
    jv::util::Vec2 pxSize;
};

/// Essaye de charger une image BMP depuis le dossier "assets'.
/// En cas d'erreur, retourne 'pTexture == nullptr'.
/// En cas de succès, l'appelant doit appeler 'jv::gpu::DestroyTexture'.
Image LoadImageAsset(std::string_view imageName);

} // namespace exo3