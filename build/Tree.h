#pragma once
#pragma once

#include <dino/dino_main.h>
#include <dino/dino_terrain.h>
#include <dino/dino_entity.h>

namespace dino
{
    class Tree : public Entity
    {
    public:
        int m_TreeType;

        void Draw() const override;

        void HandleCapture() override;
        Tree(Vec2 pos, float absTime, int treeType, jv::gpu::Texture* tex);

        Tree(const Tree&) = delete;

        void Update(double absTime, float deltaTime) override;
        void HandleTerrainClamp(dino::Terrain* terrain) override;

        ~Tree();

    };

}