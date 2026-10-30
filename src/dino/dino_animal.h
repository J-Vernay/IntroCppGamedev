#pragma once

#include <dino/dino_main.h>
#include <dino/dino_terrain.h>
#include <dino/Entity.h>

namespace dino
{

// Représente un animal.
class Animal : public Entity
{
public:
    /// Initialise l'animal avec un type au hasard.
    Animal(Vec2 pos,jv::gpu::Texture* m_pTexture, double absTime);
    
    /// Déplace l'animal et met à jour son animation.
    void Update(double absTime, float deltaTime, Terrain const& terrain);
    bool IsExpired(float lifetime) const
    {
        return m_age >= lifetime;
    }


    /// Affiche l'animal
    void Draw() const;

    void _ReactTerrain() override;

    void Caught() override;
    bool IsCaught() const
    {
        return m_state == AnimalState::catched;
    }

    enum AnimalState
    {
        Alive,
        catched,
    };

    /// Détruit les ressources associées à l'animal.
    ~Animal();


private:
    AnimalState m_state = Alive;
    double m_timeStart;
    uint8_t m_alpha = 0;
    Vec2 m_dir;
    int32_t m_kind;
    int32_t m_idxFrame;
    jv::gpu::Texture* m_pTextureptr;
    double m_age = 0;
};

} // namespace dino