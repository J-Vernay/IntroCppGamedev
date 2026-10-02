#pragma once

#include <dino/dino_main.h>
#include <dino/dino_terrain.h>
#include <dino/dino_entity.h>

namespace dino
{

// Représente un animal.
class Animal : public Entity
{
public:
    /// Initialise l'animal avec un type au hasard.
    Animal(Vec2 pos, double absTime, jv::gpu::Texture* pTexture);

    /// Constructeur d'Animal à partir d'un autre Animal par copie
    ///
    /// "Animal a = b;" ou "Animal a{b};" (on construit "a" à partir de "b")
    Animal(Animal const& other) = delete;

    /// Opérateur d'assignement par copie
    ///
    /// Animal a, b;
    /// a = b; // Pas un constructeur, car 'a' et 'b' existe déjà ; c'est une affectation
    Animal& operator=(Animal const& b) = delete;
    
    /// Déplace l'animal et met à jour son animation.
    void Update(double absTime, float deltaTime);

    /// Affiche l'animal
    void Draw() const;

    /// Détruit les ressources associées à l'animal.
    ~Animal();

private:
    double m_timeStart;
    uint8_t m_alpha = 0;
    Vec2 m_dir;
    int32_t m_kind;
    int32_t m_idxFrame;
    jv::gpu::Texture* m_pTexture;

    void _ReactTerrain() override;
};

} // namespace dino