#pragma once

#include <dino/dino_main.h>
#include <dino/dino_entity.h>
#include <utility>

namespace dino
{
class Terrain;
// Représente un animal.
class Animal : public Entity
{
public:
    /// Initialise l'animal avec un type au hasard.
    Animal(Vec2 pos, double absTime, jv::gpu::Texture* pTexture);
    
    //destruction par copie et affection impossible
    Animal(const Animal&) = delete;
    Animal& operator=(const Animal&) = delete;

    /// Déplace l'animal et met à jour son animation.
    void Update(double absTime, float deltaTime) override;

    /// Affiche l'animal
    void Draw() const override;


    ~Animal() override = default;

    void OnCaptured() override;

protected:

    void OnTerrainCollision(Vec2 correction) override;


private:
    double m_timeStart;
    uint8_t m_alpha = 0;
    Vec2 m_dir;
    int32_t m_kind;
    int32_t m_idxFrame;
    jv::gpu::Texture* m_pTexture;
};

} // namespace dino