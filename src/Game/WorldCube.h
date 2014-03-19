
#pragma once

#include "Common.h"
#include "ObjectData.h"
#include "World.h"
#include <vector>
#include <algorithm>

namespace gen
{

class WorldCube: public ObjectData
{
public:
    WorldCube(RenderData*          pRenderData,
              PhysicsData*         pPhysicsData,
              const ci::Vec3f&     pos,
              const ci::Quatf&     rot,
              const ci::Vec3f&     scale,
              const ci::Vec3i&     index,
              const CUBE_COLOR&    color);
    ~WorldCube();
    virtual void Draw();
    virtual void Update(float dt);

    void DrawHighlight(const RenderData* pRenderData, const ci::Vec3f& normal, bool available);

    ci::Vec3i m_worldIndex;
    ci::gl::Texture m_highlightTex;
    RenderData* m_pHighlightRenderData;
    CUBE_COLOR m_cubeColor;

    bool m_alive;   // signifies dying animation
    float m_deadTime;
    const float cTimeToDie;

private:

};
}
