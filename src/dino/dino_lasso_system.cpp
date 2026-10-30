#include <dino/dino_geometry.h>
#include <dino/dino_lasso_system.h>

std::vector<dino::LassoLoop> dino::LassoSystem::UpdateIntersections(std::vector<Player*> const& players)
{
    std::vector<dino::LassoLoop> loops;

    for (size_t i = 0; i < players.size(); ++i)
    {
        Lasso* lassoA = players[i]->GetLasso();
        if (!lassoA)
            continue;

        auto const& trailA = lassoA->GetTrail();

        if (trailA.size() < 2)
            continue;

        // On récupére le dernier segment tracé par le joueur i
        size_t lastIdx = trailA.size() - 1;
        Vec2 C = trailA[lastIdx - 1].pos;
        Vec2 D = trailA[lastIdx].pos;

        for (size_t j = 0; j < players.size(); ++j)
        {
            if (i == j && trailA.size() < 4)
                continue;

            Lasso* lassoB = players[j]->GetLasso();
            if (!lassoB)
                continue;

            auto const& trailB = lassoB->GetTrail();

            if (trailB.size() < 2)
                continue;

            // Si c'est sur soit meme, on vérifie jusqu'au segment N-2 de notre lasso,
            // Sinon on verifie tout les segments du lasso de l'adversaire
            size_t maxK = (i == j) ? (lastIdx - 2) : (trailB.size() - 1);

            for (size_t k = 0; k < maxK; ++k)
            {
                Vec2 A = trailB[k].pos;
                Vec2 B = trailB[k + 1].pos;

                if (dino::IntersectSegment(A, B, C, D))
                {
                    if (i == j)
                    {
                        // Extraction du polygone
                        std::vector<Vec2> polygon;
                        for (size_t p = k + 1; p <= lastIdx; ++p)
                            polygon.push_back(trailA[p].pos);
                        loops.push_back({std::move(polygon), players[i]});

                        // suppression de la boucle
                        lassoA->CutLoop(k + 1);
                    }
                    else
                    {
                        lassoB->CutTrail(k + 1);
                    }
                    break;
                }
            }
        }
    }
    return loops;
}

void dino::LassoSystem::ProcessCaptures(std::vector<LassoLoop> const& closedLoops, std::vector<std::unique_ptr<jv::core::GameObject>> const& objects)
{
    if (closedLoops.empty())
        return;

    for (std::unique_ptr<jv::core::GameObject> const& obj : objects)
    {
        jv::core::GameObject* pObj = obj.get();

        if (!pObj || !pObj->IsActive() || pObj->IsDead()) continue;

        // filtre les objets qui sont des cibles valides
        ILassoTarget* pTarget = dynamic_cast<ILassoTarget*>(pObj);
        if (!pTarget) continue; // Si l'objet n'implémente pas l'interface

        for (LassoLoop const& loop : closedLoops)
        {
            // On ignore l'objet s'il est le propriétaire de la boucle
            if (pObj == loop.owner) continue;

            // Vérification de la capture
            if (dino::IsPointInPolygon(pObj->GetPos(), loop.polygon))
            {
                pTarget->OnCaughtInLasso(loop.owner);
                break;
            }
        }
    }
}
