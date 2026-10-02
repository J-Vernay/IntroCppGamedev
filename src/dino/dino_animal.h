#pragma once

#include <dino/dino_main.h>
#include <dino/dino_entity.h>

namespace dino
{
// Forward declaration
class Scene;
// Représente un animal.
class Animal : public Entity
{
public:
    /// Initialise l'animal avec un type au hasard.
    Animal(Vec2 pos, double absTime);
    
    /// Déplace l'animal et met à jour son animation.
    void Update(dino::Scene& scene, double absTime, float deltaTime) override;

    /// Affiche l'animal
    void Draw() const override;

    // Enlève l'animal de la partie et donne les points au joueur
    void OnLassoHit(Player& player_origin, Scene& scene) override;

    /// Détruit les ressources associées à l'animal.
    ~Animal();
protected:
    void _HandleTerrainCollision(Terrain const& terrain) override;
private:
    double m_timeStart;
    uint8_t m_alpha = 0;

    const float m_baseSpeed = 30;
};

} // namespace dino