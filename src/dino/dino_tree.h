#pragma once
#include <dino/dino_main.h>
#include <jv/jv.h>
#include <dino/Movable.h>

namespace dino
{

class dino_tree:public Movable
{
public:
    dino_tree(Vec2 pPos, jv::gpu::Texture* pTexture, int pIndex);
    void Draw() const override;
    int index;
};
} 
