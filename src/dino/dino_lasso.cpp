#include <dino/dino_draw_utils.h>
#include <dino/dino_geometry.h>
#include <dino/dino_lasso.h>

dino::Lasso::Lasso(Color color) : jv::core::GameObject({0,0})
{
    SetSortingOrder(1);

    m_color = color;
}

void dino::Lasso::AddPoint(jv::util::Vec2 pos, double absTime)
{
    m_trail.push_back({pos, absTime});
}

void dino::Lasso::Update(double absTime, float deltaTime)
{
    m_absTime = absTime;

    // Tronque à une longueur maximale de deux secondes
    int currentIdx = 0;
    while (currentIdx < m_trail.size() && (absTime - m_trail[currentIdx].time) > 2.0)
    {
        currentIdx++;
    }
    CutTrail(currentIdx);
}

void dino::Lasso::Draw() const
{
    if (m_trail.size() >= 2)
    {
        std::vector<Vec2> points;
        points.reserve(m_trail.size());
        for (TrailPoint const& point : m_trail)
        {
            points.push_back(point.pos);
        }

        std::vector<jv::gpu::Vertex> vsLasso;
        dino::GenVertices_Polyline(vsLasso, points, 4.0f, m_color);

        jv::gpu::VertexBuffer* pVBufLasso = jv::gpu::CreateVertexBuffer("Lasso", vsLasso);
        jv::gpu::Draw(pVBufLasso, nullptr);
        jv::gpu::DestroyVertexBuffer(pVBufLasso);
    }
}

void dino::Lasso::CutTrail(int idxEnd)
{
    if (idxEnd > m_trail.size())
        idxEnd = m_trail.size();

    m_trail.erase(m_trail.begin(), m_trail.begin() + idxEnd);
}

void dino::Lasso::CutLoop(size_t idxBegin)
{
    if (idxBegin < m_trail.size() && m_trail.size() >= 2)
    {
        m_trail.erase(m_trail.begin() + idxBegin, m_trail.end() - 1);
    }
}