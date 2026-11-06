#pragma once
#include <dino/dino_entity.h>
#include <dino/dino_main.h>
#include <dino/dino_draw_utils.h>


namespace dino{
class Scene;
class Tree : public dino::Entity
{
public:
    Tree(Vec2 pos, double absTime, int index,Scene* ptr);
    ~Tree();

    void Draw() const override;
    void CatchByPlayer() override;
    void ResolveTerrainPos(Terrain& terrain) override;
    void Update(double absTime, float deltaTime) override;
    void UpdateTimerSpawn(double absTime, float deltaTime);

private:
    int index;
    Scene* scenePtr;
    bool m_isTreeEnable;
    float m_timerSpawn;
    float m_currentAlpha;
};
}
