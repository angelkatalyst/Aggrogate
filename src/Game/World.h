
#pragma once

#include "Common.h"
#include "ObjectData.h"
#include <vector>
#include <algorithm>

namespace gen
{
// protos
class WorldCube;
class PhysicsData;

enum CUBE_COLOR
{
    WHITE       = 0,
    BLACK       = 1,
    P1_COLOR1   = 2,
    P2_COLOR1   = 3,
    P1_COLOR2   = 4,
    P2_COLOR2   = 5,
};

enum SHOOT_OP
{
    ADD_DARK,
    ADD_LIGHT,
    ADD_BLACK,
    REMOVE,
};

class World
{
public:
    World();
    ~World();
    void Draw();
    void Update(float dt);
    void Reset();
    bool PickCPU(const ci::Ray& ray, WorldCube*& pPickedCube, ci::Vec3f& pickedNormal);
    void AddCube(WorldCube* pCube, const CUBE_COLOR& color, const ci::Vec3f& norm, const ci::Ray& ray);
    void RemoveCube(const ci::Vec3i index);
    void DumpWorld();
    void NextLevel();

    ci::Quatf GetRot();
    bool AjacentCubeAvailable(const ci::Vec3i& index, const ci::Vec3f& norm);

private:
    int m_worldDim;
    ci::Vec3f m_origin;
    ci::Quatf m_rot;
    const int cMaxLevels;
    int m_level;

    ObjectData* m_pSpinObject;

    // the grid
    // x goes from left to right
    // y goes from top to bottom
    // z goes from front to back
    std::vector<std::vector<std::vector<WorldCube*>>> m_worldCubes;

    void SetCube(const ci::Vec3i& index, const CUBE_COLOR& color);
    void ApplyImpulse(const ci::Ray& ray, const ci::Vec3i& index);
    bool ClearCubes(const ci::Vec3i& index, const CUBE_COLOR& color);
    void BuildClearCubesRecursive(const ci::Vec3i& index, const CUBE_COLOR& color);
    std::vector<WorldCube*> m_clearCubes;
    void InitLevel();

    ci::audio::SourceRef m_sfx1;
    ci::audio::SourceRef m_sfx2;
    ci::audio::SourceRef m_sfx3;
};

inline ci::Quatf World::GetRot()
{
    return m_rot;
}

}
