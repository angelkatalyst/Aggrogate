
#pragma once

#include "Common.h"
#include "PlayerCam.h"
#include "World.h"
#include <vector>

namespace gen
{
// protos
class PlayerCam;
class WorldCube;
class WorldCubeHighlight;

enum PLAYER_NUM
{
    PLAYER_1,
    PLAYER_2,
};

class Player
{
public:
    Player(const PLAYER_NUM& playerNum);
    ~Player();
    virtual void Draw();
    virtual void DrawCursor();
    virtual void Update(float dt);
    virtual void Reset();

    virtual void Shoot(const SHOOT_OP& op);

    PLAYER_NUM m_playerNum;
    PlayerCam m_cam;
    ci::gl::Texture m_cursor;    
    ci::Vec2f m_cursorPos;
    ci::Vec2f m_cursorDelta;

private:

    RenderData* m_pHighlightRenderData;
    WorldCube* m_pHighlightedCube;
    ci::Vec3f m_highlightedNormal;
    ci::Ray m_pickRay;
};
}
