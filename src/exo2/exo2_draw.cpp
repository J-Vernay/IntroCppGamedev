#include <exo2/exo2_draw.h>

void exo2::DrawRect(Vec2 pos, Vec2 size, Color color)
{
    jv::gpu::Vertex vs[6];
    vs[0].pos = {pos.x, pos.y};
    vs[1].pos = {pos.x, pos.y + size.y};
    vs[2].pos = {pos.x + size.x, pos.y + size.y};
    vs[3].pos = {pos.x, pos.y};
    vs[4].pos = {pos.x + size.x, pos.y};
    vs[5].pos = {pos.x + size.x, pos.y + size.y};

    vs[0].color = color;
    vs[1].color = color;
    vs[2].color = color;
    vs[3].color = color;
    vs[4].color = color;
    vs[5].color = color;

    jv::gpu::VertexBuffer* vb = jv::gpu::CreateVertexBuffer("drawRect", vs);
    jv::gpu::Draw(vb, nullptr, {});
    jv::gpu::DestroyVertexBuffer(vb);
}

void exo2::DrawScore(
    Vec2 center, Vec2 pointSize, int scorePlayer, Color colorPlayer, int scoreAI, Color colorAI)
{
    
    for (int i = 0; i < scorePlayer; i++)
    {
        DrawRect(
            Vec2{center.x - pointSize.x * (i + 1), center.y}, pointSize, colorPlayer);
    }
    for (int i = 0; i < scoreAI; i++)
    {
        DrawRect(Vec2{center.x + pointSize.x * i, center.y}, pointSize, colorAI);
    }

}