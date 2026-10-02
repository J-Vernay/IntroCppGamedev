#include <dino/dino_entity.h>
#include <dino/dino_scene.h>

void dino::Entity::_Move(Scene const& scene, float deltaTime) {
    m_pos.x += m_dir.x * deltaTime * m_speed;
    m_pos.y += m_dir.y * deltaTime * m_speed;

    // Force player inside of terrain
    if (!scene.GetTerrain().IsInside(m_pos))
        _HandleTerrainCollision(scene.GetTerrain());
}