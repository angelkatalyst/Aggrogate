#include "WorldCube.h"
#include "ObjectManager.h"
#include "World.h"
#include "RenderData.h"

using namespace gen;
using namespace ci;
using namespace ci::app;

WorldCube::WorldCube(RenderData*        pRenderData,   // pRenderData can be NULL
                     PhysicsData*       pPhysicsData,  // pPhysicsData can be NULL
                     const Vec3f&       pos,
                     const Quatf&       rot,
                     const Vec3f&       scale,
                     const Vec3i&       index,
                     const CUBE_COLOR&  color) : 
                         ObjectData(pRenderData, pPhysicsData, pos, rot, scale),
                         m_worldIndex(index),
                         m_cubeColor(color), 
                         m_alive(true), 
                         m_deadTime(0.f),
                         cTimeToDie(0.5f)
{
}

WorldCube::~WorldCube()
{
}

void WorldCube::Draw()
{
    ASSERT(NULL != m_pRenderData);

    gl::pushMatrices();
    ci::Matrix44f modelView;
    gl::multModelView(modelView); // TODO: use this

    if (m_pRenderData->m_cellshadePercent > 0)
    {
        gl::pushMatrices();
        glCullFace(GL_FRONT);
        gl::color(0,0,0,1);
        gl::rotate(g_pObjectManager->m_pWorld->GetRot());
        gl::translate(m_pos);
        gl::rotate(m_rot);
        gl::scale(m_scale);

        gl::draw(m_pRenderData->m_mesh);
        gl::popMatrices();
    }

    glCullFace(GL_BACK);
    gl::color(m_pRenderData->m_color);
    gl::rotate(g_pObjectManager->m_pWorld->GetRot());
    gl::translate(m_pos);
    gl::rotate(m_rot);
    gl::scale(m_scale * Vec3f(1-m_pRenderData->m_cellshadePercent, 1-m_pRenderData->m_cellshadePercent, 1-m_pRenderData->m_cellshadePercent));

    if (m_pRenderData->m_tex && m_pRenderData->m_mesh.hasTexCoords())
    {
        gl::color(1,1,1,1);
        glEnable(GL_TEXTURE_2D);
        m_pRenderData->m_tex.enableAndBind();
    }

    gl::draw(m_pRenderData->m_mesh);

    if (m_pRenderData->m_tex && m_pRenderData->m_mesh.hasTexCoords())
    {
        m_pRenderData->m_tex.disable();
        m_pRenderData->m_tex.unbind();
    }

    gl::popMatrices();
}

void WorldCube::DrawHighlight(const RenderData* pRenderData, const Vec3f& normal, bool available)
{
    ASSERT(NULL != pRenderData);

    gl::pushMatrices();
    ci::Matrix44f modelView;
    gl::multModelView(modelView); // TODO: use this

    Vec3f highlightPos = m_pos;     // original cube pos
    highlightPos += normal*0.5f;         // distance of half the cube
    highlightPos += normal*0.0001f;      // a little extra to avoid Z fighting

    Quatf highlightRot = m_rot;
    highlightRot *= Quatf(-PI/2*normal.y, PI/2*normal.x, 0);
    if (normal.z == -1)
    {
        highlightRot *= Quatf(0, PI, 0);
    }

    glCullFace(GL_NONE);
    gl::color(pRenderData->m_color);
    gl::rotate(g_pObjectManager->m_pWorld->GetRot());
    gl::translate(highlightPos);
    gl::rotate(highlightRot);
    gl::scale(m_scale);

    if (pRenderData->m_tex && pRenderData->m_mesh.hasTexCoords())
    {
        if (available)
        {
            gl::color(1,1,1,1);
        }
        else
        {
            gl::color(0.2,0.2,0.2,1);
        }
        glEnable(GL_TEXTURE_2D);
        pRenderData->m_tex.enableAndBind();
    }

    gl::draw(pRenderData->m_mesh);

    if (pRenderData->m_tex && pRenderData->m_mesh.hasTexCoords())
    {
        pRenderData->m_tex.disable();
        pRenderData->m_tex.unbind();
    }

    gl::popMatrices();
}

void WorldCube::Update(float dt)
{
    if (!m_alive)
    {
        m_deadTime+=dt;
        SetRot(Quatf(m_deadTime, m_deadTime, m_deadTime));
        float deathRatio = (cTimeToDie-m_deadTime)/cTimeToDie;
        SetScale(Vec3f(deathRatio, deathRatio, deathRatio));
    }
}

