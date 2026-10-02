
#include <dino/dino_draw_utils.h>
#include <dino/dino_scene.h>
#include <format>
#include <algorithm>

dino::Scene::Scene() : m_Terrain{24, 16}
{
    m_pTextureText = dino::LoadImageAsset("monogram-bitmap.bmp");

    m_Terrain.SetSeason(jv::util::RandomInt32(0, 3));

    _SetupPlayers();
}

dino::Scene::~Scene() {}

void dino::Scene::_SetupPlayers() 
{
    jv::input::GamepadIdx playersToCreate[4] = {
        jv::input::GamepadIdx::Keyboard,
        jv::input::GamepadIdx::Gamepad1, 
        jv::input::GamepadIdx::Gamepad2, 
        jv::input::GamepadIdx::Gamepad3
    };

    for (char i = 0; i < sizeof(playersToCreate) / sizeof(playersToCreate[0]); i++)
    {
        Vec2 spawnPos = m_Terrain.GenerateRandomSpawn();
        dino::Player* player = new dino::Player(playersToCreate[i], spawnPos, i);

        m_entities.push_back(player);
    }
}

void dino::Scene::Update(double absTime, float deltaTime)
{
    m_lastDeltaTime = deltaTime;

    m_Terrain.Update(absTime, deltaTime);

    _UpdateEntities(absTime, deltaTime);
    _HandleCollisions();
}

void dino::Scene::_UpdateEntities(double absTime, float deltaTime)
{
    // Spawner un animal si besoin.

    constexpr double kSpawnTime = 0.3;
    if (absTime - m_animalSpawnTime >= kSpawnTime)
    {
        m_animalSpawnTime = absTime;
        Vec2 spawnPos = m_Terrain.GenerateRandomSpawn();
        dino::Animal* animal = new dino::Animal(spawnPos, absTime);
        m_entities.push_back(animal);
    }

    for (Entity* entityPtr : m_entities)
    {
        Entity& entity = *entityPtr;
        entity.Update(*this, absTime, deltaTime);
    }
}

void dino::Scene::_HandleCollisions()
{
    // Check every pairs for collisions
    for (Entity* entityPtr1 : m_entities)
    {
        for (Entity* entityPtr2 : m_entities)
        {
            // Don't collide with yourself
            if (entityPtr1 == entityPtr2)
                continue;

            Entity& entity1 = *entityPtr1;
            Entity& entity2 = *entityPtr2;

            float dist = dino::math::Distance(entity1.m_pos, entity2.m_pos);
            float radiusTotal = entity1.m_collisionRadius + entity2.m_collisionRadius;

            // The 2 entities are too close (closer than combined collision radius) and not perfectly overlapping
            if (dist < radiusTotal && dist > FLT_EPSILON)
            {
                // Get displacement direction
                jv::util::Vec2 displacement = dino::math::NormalizeVector(dino::math::VectorSubtract(entity1.m_pos, entity2.m_pos));
                // Get Displacement amount
                float delta = radiusTotal - dist;
                entity1.m_pos = {entity1.m_pos.x + displacement.x * delta, entity1.m_pos.y + displacement.y * delta};
                entity2.m_pos = {entity2.m_pos.x - displacement.x * delta, entity2.m_pos.y - displacement.y * delta};
            }
            
        }
    }
}

bool _SortByY(dino::Entity* aPtr, dino::Entity* bPtr)
{
    dino::Entity& a = *aPtr;
    dino::Entity& b = *bPtr;
    return a.m_pos.y > b.m_pos.y;
}

void dino::Scene::Draw() const
{
    m_Terrain.Draw();

    // Sort entities by height (Y)
    std::vector<Entity*> sortedEntities(m_entities.size());
    std::partial_sort_copy(m_entities.begin(), m_entities.end(), sortedEntities.begin(), sortedEntities.end(), _SortByY);

    for (Entity const* entityPtr : sortedEntities)
    {
        Entity const& entity = *entityPtr;
        entity.Draw();
    }

    // Nombre de millisecondes qu'il a fallu pour afficher la frame précédente.
    {
        std::string text = std::format("dTime={:04.1f}ms", m_lastDeltaTime * 1000.0);
        std::vector<jv::gpu::Vertex> vs;
        dino::GenVertices_Text(vs, text, Color_WHITE, Color_GREY);
        jv::gpu::VertexBuffer* pVBuf = jv::gpu::CreateVertexBuffer("dTime", vs);
        jv::gpu::Draw(pVBuf, m_pTextureText);
        jv::gpu::DestroyVertexBuffer(pVBuf);
    }
}

dino::Terrain& dino::Scene::GetTerrain()
{
    return m_Terrain;
}
