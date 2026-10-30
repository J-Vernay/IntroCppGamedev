#pragma once

#include <dino/dino_main.h>
#include <jv/core/jv_gameobject.h>
#include <vector>

namespace dino
{

/// Définit un point créé à un instant T depuis le début de la partie
struct TrailPoint
{
    jv::util::Vec2 pos;
    double time;
};

class Lasso : public jv::core::GameObject
{
public:
    Lasso(Color color);

    Lasso(Lasso const&) = delete;
    Lasso& operator=(Lasso const&) = delete;

    ~Lasso() override = default;

    /// Met à jour la durée de vie des points de la traînée
    void Update(double absTime, float deltaTime) override;

    /// Dessine la ligne du lasso
    void Draw() const override;

    /// Ajoute la position actuelle du propriétaire au lasso
    void AddPoint(jv::util::Vec2 pos, double absTime);

    /// Accès en lecture seule à la traînée
    std::vector<TrailPoint> const& GetTrail() const { return m_trail; }

    /// Supprime les points du début jusqu'à idxEnd (exclu)
    void CutTrail(int idxEnd);

    /// Supprime la boucle formée du point idxBegin jusqu'à l'avant-dernier point
    void CutLoop(size_t idxBegin);

private:
    Color m_color;
    double m_absTime = 0.0;
    std::vector<TrailPoint> m_trail;
};

} // namespace dino