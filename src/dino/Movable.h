#pragma once

#include <dino/dino_main.h>
#include <jv/jv.h>
#include <dino/dino_terrain.h>

namespace dino {
	class Movable
	{
	public:
		static void ResolveCollision(Movable& a, Movable& b);
        static bool OrderByPosY(Movable const* a, Movable const* b);

		/// Affiche l'animal
		virtual void Draw() const = 0;

	    void CheckTerrain(Terrain const& m_Terrain);

		

	protected:
		Vec2 m_pos;
		Vec2 m_dir;
		double m_timeStart;
		uint8_t m_alpha = 0;
		jv::gpu::Texture* m_pTexture;
        virtual void _ReactTerrain();

	};
}

