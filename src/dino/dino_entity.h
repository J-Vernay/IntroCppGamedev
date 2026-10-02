#pragma once

#include <dino/dino_main.h>

namespace dino
{

class Entity
{
public:
    static void ResolveCollision(Entity& a, Entity& b);

protected:
    Vec2 m_pos;
};

}