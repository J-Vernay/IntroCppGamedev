
#include <dino/dino_animal.h>
#include <dino/dino_scene.h>
#include <dino/dino_assets.h>
#include <dino/dino_draw_utils.h>
#include <math.h>

dino::Animal::Animal(Vec2 pos, double absTime)
{
    m_pos = pos;
    m_kind = jv::util::RandomInt32(0, 7);
    m_dir = jv::util::RandomRotate({1, 0}, 0, 360);
    m_timeStart = absTime;
    m_speed = m_baseSpeed;
}

dino::Animal::~Animal() {}

void dino::Animal::_HandleTerrainCollision(Terrain const& terrain) 
{
    m_dir = jv::util::RandomRotate({1, 0}, 0, 360);
    m_pos = terrain.ClampPos(m_pos);
}

void dino::Animal::OnLassoHit(Player& player_origin, Scene& scene)
{
    scene.RemoveEntity(this);
    delete this;
}

void dino::Animal::Update(dino::Scene const& scene, double absTime, float deltaTime)
{
    _Move(scene, deltaTime);

    m_idxFrame = int32_t(absTime * 8) % 4;

    double aliveTime = absTime - m_timeStart;
    if (aliveTime < 1)
        m_alpha = uint8_t(UINT8_MAX * aliveTime);
    else
        m_alpha = UINT8_MAX;
}

void dino::Animal::Draw() const
{
    float u1 = 0, u2 = 32, v1 = 0, v2 = 32;

    u1 += 32 * m_idxFrame + 128 * m_kind;
    u2 += 32 * m_idxFrame + 128 * m_kind;

    if (m_dir.x > 0.7f)
    {
        std::swap(u1, u2);
    }
    else if (m_dir.y > 0.7f)
    {
        v1 = 32, v2 = 64;
    }
    else if (m_dir.y < -0.7f) // Sprite mirroir vertical si mouvement vers la droite
    {
        v1 = 64, v2 = 96;
    }

    jv::util::Color color = Color_WHITE;
    color.a = m_alpha;

    std::vector<jv::gpu::Vertex> vs;
    vs.emplace_back(Vec2{m_pos.x - 16, m_pos.y - 32}, Vec2{u1, v1}, color);
    vs.emplace_back(Vec2{m_pos.x + 16, m_pos.y - 32}, Vec2{u2, v1}, color);
    vs.emplace_back(Vec2{m_pos.x - 16, m_pos.y}, Vec2{u1, v2}, color);
    vs.emplace_back(Vec2{m_pos.x + 16, m_pos.y - 32}, Vec2{u2, v1}, color);
    vs.emplace_back(Vec2{m_pos.x - 16, m_pos.y}, Vec2{u1, v2}, color);
    vs.emplace_back(Vec2{m_pos.x + 16, m_pos.y}, Vec2{u2, v2}, color);

    jv::gpu::VertexBuffer* pVBuf = jv::gpu::CreateVertexBuffer("Animal", vs);
    jv::gpu::Draw(pVBuf, (&dino::AssetsHolder::getInstance())->g_Textures["animals"]);
    jv::gpu::DestroyVertexBuffer(pVBuf);
}
