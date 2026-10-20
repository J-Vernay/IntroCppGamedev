#pragma once
#include <dino/dino_main.h>
#include <jv/jv.h>
#include <dino/Movable.h>

namespace dino
{

class dino_tree:public Movable
{
private: 
    uint8_t m_alpha = 0;
    double m_aliveTime = 0;

public:
    dino_tree(Vec2 pPos, jv::gpu::Texture* pTexture, int pIndex, double pAbsTime);
    void Draw() const override;
    bool IsActive();
    void Update(double absTime, float deltaTime);
    int index;
};
} 
