#pragma once
#include <dino/dino_main.h>
#include <jv/jv.h>
#include <dino/Movable.h>

namespace dino
{

class dino_tree: Movable
{
public:
    dino_tree(Vec2 pPos, jv::gpu::Texture* pTexture, int pIndex);
    void Draw() const override;

private:
    int index;
    jv::gpu::Texture* m_texture;
    Vec2 m_pos;
};
} 
