#pragma once

#include <dino/dino_player.h>
#include <jv/core/jv_gameobject.h>
#include <dino/dino_lasso_target.h>
#include <vector>

namespace dino
{
struct LassoLoop
{
    std::vector<Vec2> polygon;
    Player* owner; // pointeur vers le joueur qui a fermé la boucle
};

class LassoSystem
{
public:
    /// Calcule et résout toutes les intersections de lassos.
    /// Renvoi la liste de polygones représentant les boucles fermées
    static std::vector<LassoLoop> UpdateIntersections(std::vector<Player*> const& players);

    /// Pour chaque objet, vérifie qu'il est attrapable par le lasso et le notifie s'il est capturé
    static void ProcessCaptures(std::vector<LassoLoop> const& closedLoops, std::vector<std::unique_ptr<jv::core::GameObject>> const& objects);
};

} // namespace dino