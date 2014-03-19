#include "Player.h"
#include "PlayerCam.h"
#include "WorldCube.h"
#include "App.h"
#include "ObjectManager.h"
#include "World.h"
#include "RenderData.h"

using namespace gen;
using namespace ci;
using namespace ci::app;

Player::Player(const PLAYER_NUM& playerNum) : m_playerNum(playerNum), m_cam(PlayerCam()), m_cursorPos(Vec2i(g_pApp->getWindowWidth()/4, g_pApp->getWindowHeight()/2)), m_cursorDelta(Vec2f(0,0))
{
    if (playerNum == PLAYER_1)
    {
        m_cursor = gl::Texture(ci::loadImage(ci::app::loadResource(RES_P1_CURSOR_TEX)));
    }
    else if (playerNum == PLAYER_2)
    {
        m_cursor = gl::Texture(ci::loadImage(ci::app::loadResource(RES_P2_CURSOR_TEX)));
    }
    else
    {
        ASSERT(!"Shouldn't get here!");
    }

    m_cursor.setMinFilter(GL_NEAREST);    // disable bilinear filtering
    m_cursor.setMagFilter(GL_NEAREST);    // disable bilinear filtering

    m_pHighlightRenderData = new RenderData(ci::app::loadResource(RES_SQUARE_OBJ), ci::ColorAf(1.0f, 1.0f, 1.0f, 1.0f), ci::app::loadResource(RES_HIGHLIGHT_TEX));
}

Player::~Player()
{
    delete m_pHighlightRenderData;
}

void Player::Draw()
{
    if (g_pObjectManager->m_pWorld)
    {
        m_pickRay = m_cam.GetCamera().generateRay(m_cursorPos.x/(float)(g_pApp->getWindowWidth()/2), 
                                                  1 - (m_cursorPos.y/(float)g_pApp->getWindowHeight()), 
                                                  g_pApp->getWindowAspectRatio()/2);

        if (g_pObjectManager->m_pWorld->PickCPU(m_pickRay, m_pHighlightedCube, m_highlightedNormal))
        {
            ASSERT(m_pHighlightedCube);
            if (g_pObjectManager->m_pWorld->AjacentCubeAvailable(m_pHighlightedCube->m_worldIndex, m_highlightedNormal))
            {
                m_pHighlightedCube->DrawHighlight(m_pHighlightRenderData, m_highlightedNormal, true);
            }
            else
            {
                m_pHighlightedCube->DrawHighlight(m_pHighlightRenderData, m_highlightedNormal, false);
            }
        }
        else
        {
            m_pHighlightedCube = NULL;
            m_highlightedNormal = Vec3f::zero();
        }
    }
}

void Player::DrawCursor()
{
    gl::pushMatrices();

    Vec2i windowSize = getWindowSize();
    gl::setMatricesWindow(Vec2i(windowSize.x/2, windowSize.y));
    int dim = windowSize.y < windowSize.x/2 ? windowSize.y : windowSize.x/2;
    float scale = 0.05f;
    dim *= scale;

    Area bounds = Area(m_cursorPos.x - dim/2, m_cursorPos.y - dim/2, m_cursorPos.x + dim/2, m_cursorPos.y + dim/2);

    gl::draw(m_cursor, bounds);

    gl::popMatrices();
}

void Player::Update(float dt)
{
    m_cursorPos += m_cursorDelta;

    if (m_cursorPos.x < 0)
    {
        m_cursorPos.x = 0;
    }
    if (m_cursorPos.y < 0)
    {
        m_cursorPos.y = 0;
    }
    if (m_cursorPos.x > g_pApp->getWindowWidth()/2)
    {
        m_cursorPos.x = g_pApp->getWindowWidth()/2;
    }
    if (m_cursorPos.y > g_pApp->getWindowHeight())
    {
        m_cursorPos.y = g_pApp->getWindowHeight();
    }

    m_cam.Update(dt);
}

void Player::Reset()
{
    m_cam.Reset();
    m_cursorPos = Vec2i(g_pApp->getWindowWidth()/4, g_pApp->getWindowHeight()/2);
}

void Player::Shoot(const SHOOT_OP& op)
{
    if (g_pObjectManager->m_pWorld)
    {
        if (m_pHighlightedCube)
        {
            if (op == REMOVE)
            {
                g_pObjectManager->m_pWorld->RemoveCube(m_pHighlightedCube->m_worldIndex);
                m_pHighlightedCube = NULL;
            }
            else
            {
                CUBE_COLOR color = BLACK;
                if (op == ADD_DARK)
                {
                    if (m_playerNum == PLAYER_1)
                    {
                        color = P1_COLOR2;
                    }
                    else if (m_playerNum == PLAYER_2)
                    {
                        color = P2_COLOR2;
                    }
                }
                else if (op == ADD_LIGHT)
                {
                    if (m_playerNum == PLAYER_1)
                    {
                        color = P1_COLOR1;
                    }
                    else if (m_playerNum == PLAYER_2)
                    {
                        color = P2_COLOR1;
                    }
                }

                g_pObjectManager->m_pWorld->AddCube(m_pHighlightedCube, color, m_highlightedNormal, m_pickRay);
            }
        }
    }
}
