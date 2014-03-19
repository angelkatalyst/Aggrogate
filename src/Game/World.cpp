#include "World.h"
#include "WorldCube.h"
#include "RenderData.h"
#include "PhysicsData.h"
#include "ObjectManager.h"
#include "App.h"
#include "Player.h"
#include <math.h>

using namespace gen;
using namespace ci;
using namespace ci::app;

World::World() : m_worldDim(11), m_origin(Vec3f(0,0,0)), m_rot(Quatf()), cMaxLevels(5)
{
    m_level = 0;
    
    // physics box to calculate spin
    m_pSpinObject = new ObjectData( NULL,
                                    new PhysicsData(new btBoxShape(btVector3(0.5f, 0.5f, 0.5f)), 10),
                                    Vec3f::zero(), Quatf(0,0,0), Vec3f(1.f,1.f,1.f));
//    m_pSpinObject->m_pPhysicsData->m_pBtRigidBody->setLinearFactor(btVector3(0,0,0));     // cannot translate
//    m_pSpinObject->m_pPhysicsData->m_pBtRigidBody->setAngularFactor(btVector3(1,1,1));    // can rotate
    m_pSpinObject->m_pPhysicsData->m_pBtRigidBody->setDamping(0, 0.5f);    // slowly stop rotating

    // null the world out
    m_worldCubes.resize(m_worldDim);
    for (int x = 0; x < m_worldDim; x++)
    {
        m_worldCubes[x].resize(m_worldDim);
        for (int y = 0; y < m_worldDim; y++)
        {
            m_worldCubes[x][y].resize(m_worldDim);
            for (int z = 0; z < m_worldDim; z++)
            {
                m_worldCubes[x][y][z] = NULL;
//                SetCube(Vec3i(x,y,z), BLACK);    // for performance testing
            }
        }
    }

    // add one cube to the middle
    Vec3i index = Vec3i(m_worldDim/2, m_worldDim/2, m_worldDim/2);
//    SetCube(index, WHITE);

    InitLevel();

    m_sfx1 = ci::audio::load(ci::app::loadResource(RES_SFX1_SOUND));
    m_sfx2 = ci::audio::load(ci::app::loadResource(RES_SFX2_SOUND));
    m_sfx3 = ci::audio::load(ci::app::loadResource(RES_SFX3_SOUND));
}

World::~World()
{
    delete m_pSpinObject;

    // null the world out
    for (int x = 0; x < m_worldDim; x++)
    {
        for (int y = 0; y < m_worldDim; y++)
        {
            for (int z = 0; z < m_worldDim; z++)
            {
                if (m_worldCubes[x][y][z])
                {
                    delete m_worldCubes[x][y][z];
                    m_worldCubes[x][y][z] = NULL;
                }
            }
        }
    }
}

void World::InitLevel()
{
    if (m_level == 0)
    {
        SetCube(Vec3i(2,4,5), P1_COLOR1);
        SetCube(Vec3i(3,4,5), P1_COLOR1);
        SetCube(Vec3i(3,5,5), P1_COLOR2);
        SetCube(Vec3i(3,6,5), P1_COLOR2);
        SetCube(Vec3i(4,4,5), P1_COLOR1);
        SetCube(Vec3i(4,5,5), BLACK);
        SetCube(Vec3i(4,6,5), P1_COLOR2);
        SetCube(Vec3i(5,5,5), WHITE);
        SetCube(Vec3i(6,4,5), P2_COLOR1);
        SetCube(Vec3i(6,5,5), BLACK);
        SetCube(Vec3i(6,6,5), P2_COLOR2);
        SetCube(Vec3i(7,4,5), P2_COLOR1);
        SetCube(Vec3i(7,5,5), P2_COLOR2);
        SetCube(Vec3i(7,6,5), P2_COLOR2);
        SetCube(Vec3i(8,4,5), P2_COLOR1);
    }
    else if (m_level == 1)
    {
        SetCube(Vec3i(3,5,4), P2_COLOR2);
        SetCube(Vec3i(3,5,6), P1_COLOR2);
        SetCube(Vec3i(4,4,4), BLACK);
        SetCube(Vec3i(4,4,6), BLACK);
        SetCube(Vec3i(4,5,4), P2_COLOR1);
        SetCube(Vec3i(4,5,5), BLACK);
        SetCube(Vec3i(4,5,6), P1_COLOR1);
        SetCube(Vec3i(4,6,4), BLACK);
        SetCube(Vec3i(4,6,6), BLACK);
        SetCube(Vec3i(5,3,4), BLACK);
        SetCube(Vec3i(5,3,6), BLACK);
        SetCube(Vec3i(5,4,4), P2_COLOR2);
        SetCube(Vec3i(5,4,6), P1_COLOR2);
        SetCube(Vec3i(5,5,2), BLACK);
        SetCube(Vec3i(5,5,3), P2_COLOR1);
        SetCube(Vec3i(5,5,4), P2_COLOR2);
        SetCube(Vec3i(5,5,5), WHITE);
        SetCube(Vec3i(5,5,6), P1_COLOR2);
        SetCube(Vec3i(5,5,7), P1_COLOR1);
        SetCube(Vec3i(5,5,8), BLACK);
        SetCube(Vec3i(5,6,4), P2_COLOR1);
        SetCube(Vec3i(5,6,6), P1_COLOR1);
        SetCube(Vec3i(5,7,4), BLACK);
        SetCube(Vec3i(5,7,6), BLACK);
        SetCube(Vec3i(6,4,4), BLACK);
        SetCube(Vec3i(6,4,6), BLACK);
        SetCube(Vec3i(6,5,4), P2_COLOR1);
        SetCube(Vec3i(6,5,5), BLACK);
        SetCube(Vec3i(6,5,6), P1_COLOR1);
        SetCube(Vec3i(6,6,4), BLACK);
        SetCube(Vec3i(6,6,6), BLACK);
        SetCube(Vec3i(7,5,4), P2_COLOR2);
        SetCube(Vec3i(7,5,6), P1_COLOR2);
    }
    else if (m_level == 2)
    {
        SetCube(Vec3i(0,4,4), P1_COLOR2);
        SetCube(Vec3i(0,4,5), P1_COLOR2);
        SetCube(Vec3i(0,4,6), P1_COLOR2);
        SetCube(Vec3i(0,5,4), P1_COLOR1);
        SetCube(Vec3i(0,5,5), P1_COLOR1);
        SetCube(Vec3i(0,5,6), P1_COLOR1);
        SetCube(Vec3i(0,6,4), P1_COLOR2);
        SetCube(Vec3i(0,6,5), P1_COLOR2);
        SetCube(Vec3i(0,6,6), P1_COLOR2);
        SetCube(Vec3i(1,4,4), P1_COLOR1);
        SetCube(Vec3i(1,4,5), P1_COLOR1);
        SetCube(Vec3i(1,4,6), P1_COLOR1);
        SetCube(Vec3i(1,5,4), P1_COLOR2);
        SetCube(Vec3i(1,5,5), P1_COLOR2);
        SetCube(Vec3i(1,5,6), P1_COLOR2);
        SetCube(Vec3i(1,6,4), P1_COLOR1);
        SetCube(Vec3i(1,6,5), P1_COLOR1);
        SetCube(Vec3i(1,6,6), P1_COLOR1);
        SetCube(Vec3i(2,4,4), P1_COLOR2);
        SetCube(Vec3i(2,4,6), P1_COLOR2);
        SetCube(Vec3i(2,5,4), P1_COLOR1);
        SetCube(Vec3i(2,5,5), BLACK);
        SetCube(Vec3i(2,5,6), P1_COLOR1);
        SetCube(Vec3i(2,6,4), P1_COLOR2);
        SetCube(Vec3i(2,6,6), P1_COLOR2);
        SetCube(Vec3i(3,5,5), BLACK);
        SetCube(Vec3i(4,5,5), BLACK);
        SetCube(Vec3i(5,5,5), WHITE);
        SetCube(Vec3i(6,5,5), BLACK);
        SetCube(Vec3i(7,5,5), BLACK);
        SetCube(Vec3i(8,4,4), P2_COLOR2);
        SetCube(Vec3i(8,4,5), P2_COLOR2);
        SetCube(Vec3i(8,4,6), P2_COLOR2);
        SetCube(Vec3i(8,5,4), P2_COLOR1);
        SetCube(Vec3i(8,5,5), BLACK);
        SetCube(Vec3i(8,5,6), P2_COLOR1);
        SetCube(Vec3i(8,6,4), P2_COLOR2);
        SetCube(Vec3i(8,6,5), P2_COLOR2);
        SetCube(Vec3i(8,6,6), P2_COLOR2);
        SetCube(Vec3i(9,4,4), P2_COLOR1);
        SetCube(Vec3i(9,4,5), P2_COLOR1);
        SetCube(Vec3i(9,4,6), P2_COLOR1);
        SetCube(Vec3i(9,5,4), P2_COLOR2);
        SetCube(Vec3i(9,5,5), P2_COLOR2);
        SetCube(Vec3i(9,5,6), P2_COLOR2);
        SetCube(Vec3i(9,6,4), P2_COLOR1);
        SetCube(Vec3i(9,6,5), P2_COLOR1);
        SetCube(Vec3i(9,6,6), P2_COLOR1);
        SetCube(Vec3i(10,4,4), P2_COLOR2);
        SetCube(Vec3i(10,4,5), P2_COLOR2);
        SetCube(Vec3i(10,4,6), P2_COLOR2);
        SetCube(Vec3i(10,5,4), P2_COLOR1);
        SetCube(Vec3i(10,5,5), P2_COLOR1);
        SetCube(Vec3i(10,5,6), P2_COLOR1);
        SetCube(Vec3i(10,6,4), P2_COLOR2);
        SetCube(Vec3i(10,6,5), P2_COLOR2);
        SetCube(Vec3i(10,6,6), P2_COLOR2);
    }
    else if (m_level == 3)
    {
        SetCube(Vec3i(0,5,2), BLACK);
        SetCube(Vec3i(1,3,2), P2_COLOR2);
        SetCube(Vec3i(1,4,2), P2_COLOR2);
        SetCube(Vec3i(1,5,2), P2_COLOR1);
        SetCube(Vec3i(2,4,2), P2_COLOR2);
        SetCube(Vec3i(2,5,2), P2_COLOR1);
        SetCube(Vec3i(2,5,3), BLACK);
        SetCube(Vec3i(3,5,1), P2_COLOR1);
        SetCube(Vec3i(3,5,2), BLACK);
        SetCube(Vec3i(3,5,3), P2_COLOR1);
        SetCube(Vec3i(3,5,4), BLACK);
        SetCube(Vec3i(3,5,6), P1_COLOR2);
        SetCube(Vec3i(3,6,4), P2_COLOR2);
        SetCube(Vec3i(4,4,6), BLACK);
        SetCube(Vec3i(4,5,4), P2_COLOR2);
        SetCube(Vec3i(4,5,5), BLACK);
        SetCube(Vec3i(4,5,6), P1_COLOR2);
        SetCube(Vec3i(4,6,1), P2_COLOR1);
        SetCube(Vec3i(4,6,2), BLACK);
        SetCube(Vec3i(4,6,3), P2_COLOR1);
        SetCube(Vec3i(4,6,4), BLACK);
        SetCube(Vec3i(4,7,4), P2_COLOR2);
        SetCube(Vec3i(5,2,6), P1_COLOR2);
        SetCube(Vec3i(5,3,6), BLACK);
        SetCube(Vec3i(5,3,7), P1_COLOR1);
        SetCube(Vec3i(5,3,8), BLACK);
        SetCube(Vec3i(5,3,9), P1_COLOR1);
        SetCube(Vec3i(5,4,6), P1_COLOR2);
        SetCube(Vec3i(5,5,5), WHITE);
        SetCube(Vec3i(5,6,4), P2_COLOR2);
        SetCube(Vec3i(5,7,1), P2_COLOR1);
        SetCube(Vec3i(5,7,2), BLACK);
        SetCube(Vec3i(5,7,3), P2_COLOR1);
        SetCube(Vec3i(5,7,4), BLACK);
        SetCube(Vec3i(5,8,4), P2_COLOR2);
        SetCube(Vec3i(6,3,6), P1_COLOR2);
        SetCube(Vec3i(6,4,6), BLACK);
        SetCube(Vec3i(6,4,7), P1_COLOR1);
        SetCube(Vec3i(6,4,8), BLACK);
        SetCube(Vec3i(6,4,9), P1_COLOR1);
        SetCube(Vec3i(6,5,4), P2_COLOR2);
        SetCube(Vec3i(6,5,5), BLACK);
        SetCube(Vec3i(6,5,6), P1_COLOR2);
        SetCube(Vec3i(6,6,4), BLACK);
        SetCube(Vec3i(7,4,6), P1_COLOR2);
        SetCube(Vec3i(7,5,4), P2_COLOR2);
        SetCube(Vec3i(7,5,6), BLACK);
        SetCube(Vec3i(7,5,7), P1_COLOR1);
        SetCube(Vec3i(7,5,8), BLACK);
        SetCube(Vec3i(7,5,9), P1_COLOR1);
        SetCube(Vec3i(8,5,7), BLACK);
        SetCube(Vec3i(8,5,8), P1_COLOR1);
        SetCube(Vec3i(8,6,8), P1_COLOR2);
        SetCube(Vec3i(9,5,8), P1_COLOR1);
        SetCube(Vec3i(9,6,8), P1_COLOR2);
        SetCube(Vec3i(9,7,8), P1_COLOR2);
        SetCube(Vec3i(10,5,8), BLACK);
    }
    else if (m_level == 4)
    {
        SetCube(Vec3i(0,5,4), P1_COLOR1);
        SetCube(Vec3i(0,5,5), P1_COLOR1);
        SetCube(Vec3i(0,5,6), P1_COLOR1);
        SetCube(Vec3i(1,5,3), BLACK);
        SetCube(Vec3i(1,5,4), P1_COLOR2);
        SetCube(Vec3i(1,5,5), P1_COLOR2);
        SetCube(Vec3i(1,5,6), P1_COLOR2);
        SetCube(Vec3i(1,5,7), BLACK);
        SetCube(Vec3i(2,4,8), BLACK);
        SetCube(Vec3i(2,4,9), BLACK);
        SetCube(Vec3i(2,5,2), P1_COLOR2);
        SetCube(Vec3i(2,5,3), P1_COLOR2);
        SetCube(Vec3i(2,5,4), P1_COLOR1);
        SetCube(Vec3i(2,5,5), BLACK);
        SetCube(Vec3i(2,5,6), P1_COLOR1);
        SetCube(Vec3i(2,5,7), P1_COLOR2);
        SetCube(Vec3i(2,5,8), P1_COLOR2);
        SetCube(Vec3i(2,5,9), P1_COLOR1);
        SetCube(Vec3i(2,6,3), P1_COLOR2);
        SetCube(Vec3i(2,6,4), BLACK);
        SetCube(Vec3i(2,6,6), BLACK);
        SetCube(Vec3i(2,6,7), P1_COLOR2);
        SetCube(Vec3i(2,7,2), BLACK);
        SetCube(Vec3i(2,7,3), P1_COLOR1);
        SetCube(Vec3i(2,7,7), P1_COLOR1);
        SetCube(Vec3i(2,7,8), BLACK);
        SetCube(Vec3i(2,8,3), P1_COLOR1);
        SetCube(Vec3i(2,8,7), P1_COLOR1);
        SetCube(Vec3i(2,9,3), P1_COLOR2);
        SetCube(Vec3i(2,9,4), BLACK);
        SetCube(Vec3i(2,9,6), BLACK);
        SetCube(Vec3i(2,9,7), P1_COLOR2);
        SetCube(Vec3i(3,2,1), BLACK);
        SetCube(Vec3i(3,2,3), P2_COLOR2);
        SetCube(Vec3i(3,2,7), P2_COLOR2);
        SetCube(Vec3i(3,3,1), P2_COLOR2);
        SetCube(Vec3i(3,3,2), P2_COLOR2);
        SetCube(Vec3i(3,3,3), P2_COLOR1);
        SetCube(Vec3i(3,3,4), P2_COLOR1);
        SetCube(Vec3i(3,3,5), P2_COLOR2);
        SetCube(Vec3i(3,3,6), P2_COLOR1);
        SetCube(Vec3i(3,3,7), P2_COLOR1);
        SetCube(Vec3i(3,4,4), BLACK);
        SetCube(Vec3i(3,4,6), BLACK);
        SetCube(Vec3i(3,4,9), P1_COLOR1);
        SetCube(Vec3i(3,5,4), P1_COLOR1);
        SetCube(Vec3i(3,5,5), BLACK);
        SetCube(Vec3i(3,5,6), P1_COLOR1);
        SetCube(Vec3i(3,7,3), P1_COLOR1);
        SetCube(Vec3i(3,7,7), P1_COLOR1);
        SetCube(Vec3i(3,9,3), BLACK);
        SetCube(Vec3i(3,9,7), BLACK);
        SetCube(Vec3i(4,2,3), BLACK);
        SetCube(Vec3i(4,2,7), BLACK);
        SetCube(Vec3i(4,3,1), P2_COLOR1);
        SetCube(Vec3i(4,3,9), BLACK);
        SetCube(Vec3i(4,4,9), P1_COLOR1);
        SetCube(Vec3i(4,5,5), BLACK);
        SetCube(Vec3i(4,7,3), P1_COLOR2);
        SetCube(Vec3i(4,7,7), P1_COLOR2);
        SetCube(Vec3i(4,8,3), P1_COLOR2);
        SetCube(Vec3i(4,8,7), P1_COLOR2);
        SetCube(Vec3i(5,3,1), P2_COLOR1);
        SetCube(Vec3i(5,3,9), P1_COLOR1);
        SetCube(Vec3i(5,4,1), P2_COLOR2);
        SetCube(Vec3i(5,4,9), P1_COLOR2);
        SetCube(Vec3i(5,5,5), WHITE);
        SetCube(Vec3i(6,2,3), BLACK);
        SetCube(Vec3i(6,2,7), BLACK);
        SetCube(Vec3i(6,3,1), BLACK);
        SetCube(Vec3i(6,3,9), P1_COLOR1);
        SetCube(Vec3i(6,4,1), P2_COLOR1);
        SetCube(Vec3i(6,5,5), BLACK);
        SetCube(Vec3i(6,7,3), P2_COLOR2);
        SetCube(Vec3i(6,7,7), P2_COLOR2);
        SetCube(Vec3i(6,8,3), P2_COLOR2);
        SetCube(Vec3i(6,8,7), P2_COLOR2);
        SetCube(Vec3i(7,2,3), P1_COLOR2);
        SetCube(Vec3i(7,2,7), P1_COLOR2);
        SetCube(Vec3i(7,2,9), BLACK);
        SetCube(Vec3i(7,3,3), P1_COLOR1);
        SetCube(Vec3i(7,3,4), P1_COLOR1);
        SetCube(Vec3i(7,3,5), P1_COLOR2);
        SetCube(Vec3i(7,3,6), P1_COLOR1);
        SetCube(Vec3i(7,3,7), P1_COLOR1);
        SetCube(Vec3i(7,3,8), P1_COLOR2);
        SetCube(Vec3i(7,3,9), P1_COLOR2);
        SetCube(Vec3i(7,4,1), P2_COLOR1);
        SetCube(Vec3i(7,4,4), BLACK);
        SetCube(Vec3i(7,4,6), BLACK);
        SetCube(Vec3i(7,5,4), P2_COLOR1);
        SetCube(Vec3i(7,5,5), BLACK);
        SetCube(Vec3i(7,5,6), P2_COLOR1);
        SetCube(Vec3i(7,7,3), P2_COLOR1);
        SetCube(Vec3i(7,7,7), P2_COLOR1);
        SetCube(Vec3i(7,9,3), BLACK);
        SetCube(Vec3i(7,9,7), BLACK);
        SetCube(Vec3i(8,4,1), BLACK);
        SetCube(Vec3i(8,4,2), BLACK);
        SetCube(Vec3i(8,5,1), P2_COLOR1);
        SetCube(Vec3i(8,5,2), P2_COLOR2);
        SetCube(Vec3i(8,5,3), P2_COLOR2);
        SetCube(Vec3i(8,5,4), P2_COLOR1);
        SetCube(Vec3i(8,5,5), BLACK);
        SetCube(Vec3i(8,5,6), P2_COLOR1);
        SetCube(Vec3i(8,5,7), P2_COLOR2);
        SetCube(Vec3i(8,5,8), P2_COLOR2);
        SetCube(Vec3i(8,6,3), P2_COLOR2);
        SetCube(Vec3i(8,6,4), BLACK);
        SetCube(Vec3i(8,6,6), BLACK);
        SetCube(Vec3i(8,6,7), P2_COLOR2);
        SetCube(Vec3i(8,7,2), BLACK);
        SetCube(Vec3i(8,7,3), P2_COLOR1);
        SetCube(Vec3i(8,7,7), P2_COLOR1);
        SetCube(Vec3i(8,7,8), BLACK);
        SetCube(Vec3i(8,8,3), P2_COLOR1);
        SetCube(Vec3i(8,8,7), P2_COLOR1);
        SetCube(Vec3i(8,9,3), P2_COLOR2);
        SetCube(Vec3i(8,9,4), BLACK);
        SetCube(Vec3i(8,9,6), BLACK);
        SetCube(Vec3i(8,9,7), P2_COLOR2);
        SetCube(Vec3i(9,5,3), BLACK);
        SetCube(Vec3i(9,5,4), P2_COLOR2);
        SetCube(Vec3i(9,5,5), P2_COLOR2);
        SetCube(Vec3i(9,5,6), P2_COLOR2);
        SetCube(Vec3i(9,5,7), BLACK);
        SetCube(Vec3i(10,5,4), P2_COLOR1);
        SetCube(Vec3i(10,5,5), P2_COLOR1);
        SetCube(Vec3i(10,5,6), P2_COLOR1);
    }
    else
    {
        ASSERT(!"Shouldn't get here!");
    }
}

void World::Draw()
{
    for (int x = 0; x < m_worldDim; x++)
    {
        for (int y = 0; y < m_worldDim; y++)
        {
            for (int z = 0; z < m_worldDim; z++)
            {
                if (m_worldCubes[x][y][z])
                {
                    m_worldCubes[x][y][z]->Draw();
                }
            }
        }
    }
}

void World::Update(float dt)
{
    if (g_pObjectManager->m_gameState == TITLE ||
        g_pObjectManager->m_gameState == RULES)
    {
        m_rot *= Quatf(0.f, 1.0f*dt, 0.f);
    }
    else
    {
        m_pSpinObject->Update(dt);
        btTransform btTrans = m_pSpinObject->m_pPhysicsData->GetTransform();
        btQuaternion rot = btTrans.getRotation();
        m_rot = ci::Quatf(rot.w(), rot.x(), rot.y(), rot.z());

        bool p1Cubes = false;
        bool p2Cubes = false;

        for (int x = 0; x < m_worldDim; x++)
        {
            for (int y = 0; y < m_worldDim; y++)
            {
                for (int z = 0; z < m_worldDim; z++)
                {
                    if (m_worldCubes[x][y][z])
                    {
                        if (m_worldCubes[x][y][z]->m_cubeColor == P1_COLOR1 ||
                            m_worldCubes[x][y][z]->m_cubeColor == P1_COLOR2)
                        {
                            p1Cubes = true;
                        }

                        if (m_worldCubes[x][y][z]->m_cubeColor == P2_COLOR1 ||
                            m_worldCubes[x][y][z]->m_cubeColor == P2_COLOR2)
                        {
                            p2Cubes = true;
                        }

                        m_worldCubes[x][y][z]->Update(dt);
                        if (!m_worldCubes[x][y][z]->m_alive &&(
                            m_worldCubes[x][y][z]->cTimeToDie <= m_worldCubes[x][y][z]->m_deadTime))
                        {
                            delete m_worldCubes[x][y][z];
                            m_worldCubes[x][y][z] = NULL;
                        }
                    }
                }
            }
        }

        if (g_pObjectManager->m_gameState == PLAYING)
        {
            if (!p1Cubes)
            {
                g_pObjectManager->m_gameState = P1_WINS;
            }
            else if (!p2Cubes)
            {
                g_pObjectManager->m_gameState = P2_WINS;
            }
        }
    }
}

void World::Reset()
{
    // null the world out
    for (int x = 0; x < m_worldDim; x++)
    {
        for (int y = 0; y < m_worldDim; y++)
        {
            for (int z = 0; z < m_worldDim; z++)
            {
                if (m_worldCubes[x][y][z])
                {
                    delete m_worldCubes[x][y][z];
                    m_worldCubes[x][y][z] = NULL;
                }
            }
        }
    }

    InitLevel();

    m_pSpinObject->SetRot(Quatf(0,0,0));
}

void World::NextLevel()
{
    m_level++;
    if (m_level >= cMaxLevels)
    {
        m_level = 0;
    }
    Reset();
}

void World::DumpWorld()
{
    ci::app::console() << "-------------------------------------------" << std::endl;
    for (int x = 0; x < m_worldDim; x++)
    {
        for (int y = 0; y < m_worldDim; y++)
        {
            for (int z = 0; z < m_worldDim; z++)
            {
                if (m_worldCubes[x][y][z])
                {
                    m_worldCubes[x][y][z]->m_pRenderData->m_color;
                    ci::app::console() << "SetCube(Vec3i(" << x << "," << y << "," << z << "), ";
                    
                    switch (m_worldCubes[x][y][z]->m_cubeColor)
                    {
                        case WHITE:
                            ci::app::console() << "WHITE";
                            break;
                        case BLACK:
                            ci::app::console() << "BLACK";
                            break;
                        case P1_COLOR1:
                            ci::app::console() << "P1_COLOR1";
                            break;
                        case P1_COLOR2:
                            ci::app::console() << "P1_COLOR2";
                            break;
                        case P2_COLOR1:
                            ci::app::console() << "P2_COLOR1";
                            break;
                        case P2_COLOR2:
                            ci::app::console() << "P2_COLOR2";
                            break;
                        default:
                            ASSERT(!"Shouldn't get here!");
                            break;
                    }
                        
                    ci::app::console() << ");" << std::endl;
                }
            }
        }
    }
}

bool World::AjacentCubeAvailable(const Vec3i& index, const Vec3f& norm)
{
    bool retval = false;
    Vec3f newNorm = Vec3f(floor(norm.x + 0.5), floor(norm.y + 0.5), floor(norm.z + 0.5));
    Vec3i newIndex = index + newNorm;

    if ((newIndex.x >= 0 && newIndex.x < m_worldDim) &&
        (newIndex.y >= 0 && newIndex.y < m_worldDim) &&
        (newIndex.z >= 0 && newIndex.z < m_worldDim) && 
        NULL == m_worldCubes[newIndex.x][newIndex.y][newIndex.z])
    {
        retval = true;
    }
    return retval;
}

void World::SetCube(const Vec3i& index, const CUBE_COLOR& color)
{
    ASSERT(NULL == m_worldCubes[index.x][index.y][index.z]);

    Vec3f pos = (m_origin - Vec3i(m_worldDim/2, m_worldDim/2, m_worldDim/2) + index) * 1.2;

    Colorf col = Colorf(0,0,0);
    switch (color)
    {
        case WHITE:
            col = Colorf(1.0f, 1.0f, 1.0f);
            break;
        case BLACK:
            col = Colorf(0.1f, 0.1f, 0.1f);
            break;
        case P1_COLOR1:
            col = Colorf(163/255.f,  45/255.f, 168/255.f);
            break;
        case P1_COLOR2:
            col = Colorf( 74/255.f,  19/255.f,  85/255.f);
            break;
        case P2_COLOR1:
            col = Colorf(85/255.f, 170/255.f, 174/255.f);
            break;
        case P2_COLOR2:
            col = Colorf( 32/255.f, 83/255.f, 102/255.f);
            break;
        default:
            break;
    }

    WorldCube* pCube = new WorldCube(new RenderData(ci::app::loadResource(RES_CUBE_OBJ), col), 
                                     NULL,
                                     pos, Quatf(0,0,0), Vec3f(1.f,1.f,1.f), index, color);
    pCube->m_pRenderData->m_cellshadePercent = 0.2;

    m_worldCubes[index.x][index.y][index.z] = pCube;
}

void World::RemoveCube(const Vec3i index)
{
    ASSERT(NULL != m_worldCubes[index.x][index.y][index.z]);
    delete m_worldCubes[index.x][index.y][index.z];
    m_worldCubes[index.x][index.y][index.z] = NULL;
}


void World::AddCube(WorldCube* pCube, const CUBE_COLOR& color, const Vec3f& norm, const Ray& ray)
{
    ASSERT(pCube != NULL);

    Vec3f newNorm = Vec3f(floor(norm.x + 0.5), floor(norm.y + 0.5), floor(norm.z + 0.5));
    Vec3i newIndex = pCube->m_worldIndex + newNorm;

    if ((newIndex.x >= 0 && newIndex.x < m_worldDim) &&
        (newIndex.y >= 0 && newIndex.y < m_worldDim) &&
        (newIndex.z >= 0 && newIndex.z < m_worldDim) && 
        NULL == m_worldCubes[newIndex.x][newIndex.y][newIndex.z])
    {

        SetCube(newIndex, color);
#ifndef _DEBUG
        ApplyImpulse(ray, newIndex);
#endif
        if (ClearCubes(newIndex, color))
        {
            ci::audio::Output::play(m_sfx2);
        }
        else
        {
            ci::audio::Output::play(m_sfx1);
        }
    }
}

bool World::ClearCubes(const ci::Vec3i& index, const CUBE_COLOR& color)
{
    bool retval = false;
    BuildClearCubesRecursive(index, color);
    if (m_clearCubes.size() >= 4)
    {
        retval = true;
        for (auto i = m_clearCubes.begin(); i != m_clearCubes.end(); i++)
        {
            m_worldCubes[(*i)->m_worldIndex.x][(*i)->m_worldIndex.y][(*i)->m_worldIndex.z]->m_alive = false;
        }
    }
    m_clearCubes.clear();
    return retval;
}


void World::BuildClearCubesRecursive(const Vec3i& index, const CUBE_COLOR& color)
{
    if (m_worldCubes[index.x][index.y][index.z] &&
        m_worldCubes[index.x][index.y][index.z]->m_alive &&
        m_worldCubes[index.x][index.y][index.z]->m_cubeColor == color)
    {
        WorldCube* pCube = m_worldCubes[index.x][index.y][index.z];
        bool duplicate = false;
        for (auto i = m_clearCubes.begin(); i != m_clearCubes.end(); i++)
        {
            if ((*i)->m_worldIndex == pCube->m_worldIndex)
            {
                duplicate = true;
                break;
            }
        }
        if (!duplicate)
        {
            m_clearCubes.push_back(pCube);
        }
        else
        {
            return;
        }
    }
    else
    {
        return;
    }

    if (index.x - 1 >= 0)
    {
        BuildClearCubesRecursive(Vec3i(index.x - 1, index.y, index.z), color);
    }
    if (index.x + 1 < m_worldDim)
    {
        BuildClearCubesRecursive(Vec3i(index.x + 1, index.y, index.z), color);
    }
    if (index.y - 1 >= 0)
    {
        BuildClearCubesRecursive(Vec3i(index.x, index.y - 1, index.z), color);
    }
    if (index.y + 1 < m_worldDim)
    {
        BuildClearCubesRecursive(Vec3i(index.x, index.y + 1, index.z), color);
    }
    if (index.z - 1 >= 0)
    {
        BuildClearCubesRecursive(Vec3i(index.x, index.y, index.z - 1), color);
    }
    if (index.z + 1 < m_worldDim)
    {
        BuildClearCubesRecursive(Vec3i(index.x, index.y, index.z + 1), color);
    }
}

bool World::PickCPU(const ci::Ray& ray, WorldCube*& pPickedCube, ci::Vec3f& pickedNormal)
{
    bool retval = false;

    // approximate the world space bounding box like this:
    AxisAlignedBox3f worldBounds = AxisAlignedBox3f(Vec3f(-100,-100,-100), Vec3f(100,100,100));

    // set initial distance to something far, far away
    float result = 1000.f;

    for (int x = 0; x < m_worldDim; x++)
    {
        for (int y = 0; y < m_worldDim; y++)
        {
            for (int z = 0; z < m_worldDim; z++)
            {
                if (m_worldCubes[x][y][z] && m_worldCubes[x][y][z]->m_alive)
                {
                    WorldCube* pCube = m_worldCubes[x][y][z];
                    // traverse triangle list and find the picked triangle
                    size_t polycount = pCube->m_pRenderData->m_mesh.getNumTriangles();
                    float distance = 0.0f;
                    for (size_t i=0; i<polycount; ++i)
                    {
                        Vec3f meshV0, meshV1, meshV2;
                        Vec3f v0, v1, v2;
                        // get a single triangle from the mesh
                        pCube->m_pRenderData->m_mesh.getTriangleVertices(i, &meshV0, &meshV1, &meshV2);

                        // transform triangle to world space
                        Matrix44f localTransform;
                        Matrix44f globalTransform;
                        localTransform.setToIdentity();
                        localTransform.translate(pCube->GetPos());
                        localTransform *= (pCube->GetRot().toMatrix44());
                        localTransform.scale(pCube->GetScale());

                        globalTransform.setToIdentity();
                        globalTransform *= (m_rot.toMatrix44());
                        globalTransform *= localTransform;

                        v0 = globalTransform.transformPointAffine(meshV0);
                        v1 = globalTransform.transformPointAffine(meshV1);
                        v2 = globalTransform.transformPointAffine(meshV2);

                        // test to see if the ray intersects with this triangle
                        if (ray.calcTriangleIntersection(v0, v1, v2, &distance)) 
                        {
                            // set our result to this if its closer than any intersection we've had so far
                            if (distance < result) 
                            {
                                result = distance;
                                // assuming this is the closest triangle, we'll set our normal
                                // while we've got all the points handy
                                v0 = localTransform.transformPointAffine(meshV0);
                                v1 = localTransform.transformPointAffine(meshV1);
                                v2 = localTransform.transformPointAffine(meshV2);
                                pickedNormal = (v1 - v0).cross(v2 - v0).normalized();
                                pPickedCube = pCube;
                                retval = true;
                            }
                        }
                    }
                }
            }
        }
    }
    return retval;
}

void World::ApplyImpulse(const Ray& ray, const Vec3i& index)
{
    Vec3f relPos = Vec3f(index - Vec3i(m_worldDim/2, m_worldDim/2, m_worldDim/2));
    relPos /= (m_worldDim);

    Vec3f orient = ray.getDirection().normalized();

    if (m_rot.getAngle() != 0)
    {
        relPos.rotate(m_rot.getAxis(), m_rot.getAngle());
    }

    btVector3 impulse = btVector3(orient.x,orient.y,orient.z);
    impulse = impulse * 15;

    m_pSpinObject->m_pPhysicsData->m_pBtRigidBody->applyImpulse(impulse, btVector3(relPos.x, relPos.y, relPos.z));
}
