#pragma once

#include <dino/dino_main.h>


namespace dino
{

class Terrain;

class Entity
{
public:
    Entity(Vec2 pos, float radius);

    Entity(const Entity&) = delete;
    Entity& operator=(const Entity&) = delete;

    virtual ~Entity() = default;

    virtual void Update(double absTime, float deltaTime) = 0;
    virtual void Draw() const = 0;

    Vec2 GetPosition() const;
    void SetPosition(Vec2 pos);

    float GetRadius() const;

    void ClampToTerrain(const Terrain& terrain);

    virtual void OnCaptured() = 0;

    bool IsMarkedForRemoval() const
    {
        return m_markedForRemoval;
    }

protected:
    virtual void OnTerrainCollision(Vec2 correction);

    Vec2 m_pos;

    void MarkForRemoval()
    {
        m_markedForRemoval = true;
    }

private:
    float m_radius;
    bool m_markedForRemoval = false;
};

}