#pragma once
#include "dino_main.h"
#include <dino/dino_terrain.h>

namespace dino
{

class Entity
{
public: 
	static void ResolveCollision(Entity& a, Entity& b);

	static bool OrderByPosY(Entity const* a, Entity const* b);

	virtual void Draw() const = 0;

	void CheckTerrain(Terrain const& terrain);

protected:
    Vec2 m_pos;

	virtual void _ReactTerrain();
};
}
