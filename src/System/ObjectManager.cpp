
#include "ObjectManager.h"
#include "App.h"
#include "SimpleGUI.h"
#include "ObjectData.h"
#include "RenderData.h"
#include "PhysicsData.h"
#include "SpriteSheet.h"
#include "Player.h"
#include "World.h"
#include "Particle.h"
#include "SquareParticle.h"

using namespace gen;
using namespace ci;
using namespace ci::app;

ObjectManager::ObjectManager()
{
    // Setup cameras
    CameraPersp m_camera;
    m_camera.setPerspective(60, getWindowAspectRatio()/2, 1, 1000);

    // Initialize Players
    m_pPlayer1 = new Player(PLAYER_1);
    m_camera.lookAt(Vec3f(0, 0, 20), Vec3f::zero());
    m_camera.setCenterOfInterestPoint(Vec3f::zero());
    m_pPlayer1->m_cam.SetCurrentCam(m_camera);

    m_pPlayer2 = new Player(PLAYER_2);
    m_camera.lookAt(Vec3f(0, 0, -20), Vec3f::zero());
    m_camera.setCenterOfInterestPoint(Vec3f::zero());
    m_pPlayer2->m_cam.SetCurrentCam(m_camera);

    // Initialize controls
    m_gameState     = TITLE;
    m_gravity       = false;
    m_displayStats  = false;
    m_displayGui    = false;
    m_fullscreen    = false;
    m_wireframe     = false;
    m_playMusic     = true;

    m_playtime      = 0;
    m_creditsTimer  = 0;

    g_pApp->setFullScreen(m_fullscreen);

    if (m_gravity)
    {
        g_pBtDynamicsWorld->setGravity(btVector3(0, GRAVITY_CONST, 0));
    }
    else
    {
        g_pBtDynamicsWorld->setGravity(btVector3(0, 0, 0));
    }


    // Add World
    m_pWorld = new World();


    // random square 2
    ObjectData* pSquare2 = new ObjectData(new RenderData(ci::app::loadResource(RES_SQUARE_OBJ), ci::ColorAf(0.4f, 0.4f, 0.4f, 1.0f)), 
//                                         new PhysicsData(new btStaticPlaneShape(btVector3(0,0,1), 0), 0, COL_GROUND, COL_EVERYTHING), 
                                          NULL,
                                          Vec3f(0,-10,0), Quatf(-PI/2,0,0), Vec3f(15.f,15.f,1.f));
//    m_objects.push_back(pSquare2);

    // FBO
    m_fboFormat.setSamples(4);
    m_fbo = gl::Fbo(g_pApp->getWindowWidth()*getWindow()->getContentScale(),
                    g_pApp->getWindowHeight()*getWindow()->getContentScale(),
                    m_fboFormat);

    // Load background music
    // TODO: Cinder looping doesn't work
    // TODO: Cinder getDuration doesn't work: m_bgTrack->getDuration();
/*
    float bpm = 180;
    float bars = 2;
    m_bgTrackLength = 60.f / bpm * 8 * bars;
    m_loopPlaytime = m_bgTrackLength;   // trick audio to think it's overdue for play

    m_sound = ci::audio::load(ci::app::loadResource(RES_BG_INTRO_SOUND));
*/

    // Load screens
    m_titleTex = gl::Texture(ci::loadImage(ci::app::loadResource(RES_TITLE_TEX)));
    m_rulesTex = gl::Texture(ci::loadImage(ci::app::loadResource(RES_RULES_TEX)));
    m_readyTex = gl::Texture(ci::loadImage(ci::app::loadResource(RES_READY_TEX)));
    m_winnerTex = gl::Texture(ci::loadImage(ci::app::loadResource(RES_WINNER_TEX)));
    m_loserTex = gl::Texture(ci::loadImage(ci::app::loadResource(RES_LOSER_TEX)));
    m_creditsTex = gl::Texture(ci::loadImage(ci::app::loadResource(RES_CREDITS_TEX)));

    // Setup GUI
/*
    g_pGui->lightColor = ColorA(1, 0, 0, 1);
    g_pGui->addLabel("OPTIONS:");
    g_pGui->addSeparator();
    g_pGui->addParam("Play music?", &m_playMusic, m_playMusic);
*/
    m_shader = gl::GlslProg(ci::app::loadResource(RES_VERT_SHADER), ci::app::loadResource(RES_FRAG_SHADER));
}

ObjectManager::~ObjectManager()
{
    for (auto i = m_objects.begin(); i != m_objects.end(); i++)
    {
        delete(*i);
    }

    delete m_pPlayer1;
    delete m_pPlayer2;


    if (m_pWorld)
    {
        delete m_pWorld;
    }
}

void ObjectManager::Start()
{

}

void ObjectManager::Restart()
{
    m_playtime = 0;

    m_pWorld->Reset();
    m_pPlayer1->Reset();
    m_pPlayer2->Reset();

    srand(time(NULL));

    g_pObjectManager->m_gameState = READY;
}

void ObjectManager::UpdateSoundTimer(float dt)
{
    m_playtime += dt;

//    if (m_gameState == PLAYING)
    {
#if PLAY_SOUND
//        ci::audio::Output::play(m_sound);
#endif
    }
}


void ObjectManager::Update(float dt)
{
    // Update assets
    for (auto i = m_objects.begin(); i != m_objects.end(); i++)
    {
        (*i)->Update(dt);
    }

    m_pPlayer1->Update(dt);
    m_pPlayer2->Update(dt);

    if (m_pWorld)
    {
        m_pWorld->Update(dt);
    }

    if (m_gameState == CREDITS)
    {
        m_creditsTimer += dt;
        if (m_creditsTimer >= 2)
        {
            g_pApp->quit();
        }
    }
}

void ObjectManager::Draw()
{
    // Render state
#if ! defined(CINDER_GLES)
    m_wireframe ? gl::enableWireframe() : gl::disableWireframe();
#endif

//    m_fbo.bindFramebuffer();

    gl::enableVerticalSync();   // TODO: why can't this be set once at setup?
    gl::enableAlphaBlending();
    gl::enableDepthRead();
    gl::enable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    gl::clear(ColorA(178/255.f,178/255.f,178/255.f,1.0f));

    // Draw Player 1
    gl::pushMatrices();
    gl::setViewport(Area(0, 0, g_pApp->getWindowWidth()*getWindow()->getContentScale()/2, g_pApp->getWindowHeight()*getWindow()->getContentScale()));
    gl::setMatrices(m_pPlayer1->m_cam.GetCamera());

    for (auto i = m_objects.begin(); i != m_objects.end(); i++)
    {
        (*i)->Draw();
    }

    if (m_pWorld)
    {
        m_pWorld->Draw();
    }

    m_pPlayer1->Draw();
    if (m_gameState == PLAYING)
    {
        m_pPlayer1->DrawCursor();
    }
    gl::popMatrices();

    // Draw Player 2
    gl::pushMatrices();
    gl::setViewport(Area(g_pApp->getWindowWidth()*getWindow()->getContentScale()/2, 0, g_pApp->getWindowWidth()*getWindow()->getContentScale(), g_pApp->getWindowHeight()*getWindow()->getContentScale()));
    gl::setMatrices(m_pPlayer2->m_cam.GetCamera());

    for (auto i = m_objects.begin(); i != m_objects.end(); i++)
    {
        (*i)->Draw();
    }

    if (m_pWorld)
    {
        m_pWorld->Draw();
    }

    m_pPlayer2->Draw();
    if (m_gameState == PLAYING)
    {
        m_pPlayer2->DrawCursor();
    }
    gl::popMatrices();
/*
    // Render framebuffer
    m_fbo.unbindFramebuffer();
    gl::enableVerticalSync();   // TODO: why can't this be set once at setup?
    gl::enableAlphaBlending();
    gl::enableDepthRead(false);
    gl::enableDepthWrite(false);
    gl::disable(GL_CULL_FACE);

    // Post process shader on FBO
    gl::color(1.f, 1.f, 1.f, 1.f);
    gl::setMatricesWindow(getWindowSize(), false);
    gl::Texture fboTex = m_fbo.getTexture();

    fboTex.enableAndBind();
    m_shader.bind();
    m_shader.uniform("tex0", 0);
    m_shader.uniform("resolution", Vec2f(fboTex.getWidth(), fboTex.getHeight()));
    m_shader.uniform("timer", static_cast<float>(m_playtime));
    m_shader.uniform("resolutionScale", getWindow()->getContentScale());

    gl::drawSolidRect(getWindowBounds());

    m_shader.unbind();
    fboTex.unbind();
    fboTex.disable();
*/
    // Draw game screens
//    gl::pushMatrices();
     gl::color(1,1,1,1);
    Vec2i windowSize = getWindowSize();

    int dim = windowSize.y < windowSize.x ? windowSize.y : windowSize.x;
    float scale = 1.4f;
    dim *= scale;

    float texRatio = 1.f;   // height / width
    int xoffset = 0;
    int yoffset = 0;
    Area bounds;

    switch (m_gameState)
    {
        case TITLE:
            texRatio = 0.5;
            xoffset = (windowSize.x - dim) / 2;
            yoffset = (windowSize.y - (dim*texRatio)) / 2;
            bounds.set(xoffset, yoffset, windowSize.x - xoffset, windowSize.y - yoffset);

            gl::setViewport(Area(0, 0, g_pApp->getWindowWidth()*getWindow()->getContentScale(), g_pApp->getWindowHeight()*getWindow()->getContentScale()));
            gl::setMatricesWindow(windowSize);
            gl::draw(m_titleTex, bounds);
            break;
        case RULES:
            texRatio = 0.5;
            xoffset = (windowSize.x - dim) / 2;
            yoffset = (windowSize.y - (dim*texRatio)) / 2;
            bounds.set(xoffset, yoffset, windowSize.x - xoffset, windowSize.y - yoffset);

            gl::setViewport(Area(0, 0, g_pApp->getWindowWidth()*getWindow()->getContentScale(), g_pApp->getWindowHeight()*getWindow()->getContentScale()));
            gl::setMatricesWindow(windowSize);
            gl::draw(m_rulesTex, bounds);
            break;
        case READY:
            texRatio = 0.5;
            xoffset = (windowSize.x - dim) / 2;
            yoffset = (windowSize.y - (dim*texRatio)) / 2;
            bounds.set(xoffset, yoffset, windowSize.x - xoffset, windowSize.y - yoffset);

            gl::setViewport(Area(0, 0, g_pApp->getWindowWidth()*getWindow()->getContentScale(), g_pApp->getWindowHeight()*getWindow()->getContentScale()));
            gl::setMatricesWindow(windowSize);
            gl::draw(m_readyTex, bounds);
            break;
        case PLAYING:
            break;
        case P1_WINS:
            texRatio = 1;
            xoffset = (windowSize.x - dim*.7f) / 2;
            yoffset = (windowSize.y - (dim*.7f*texRatio)) / 2;
            bounds.set(xoffset, yoffset, windowSize.x - xoffset, windowSize.y - yoffset);

            gl::setViewport(Area(0, 0, g_pApp->getWindowWidth()*getWindow()->getContentScale()/2, g_pApp->getWindowHeight()*getWindow()->getContentScale()));
            gl::setMatricesWindow(windowSize);;
            gl::draw(m_winnerTex, bounds);

            gl::setViewport(Area(g_pApp->getWindowWidth()*getWindow()->getContentScale()/2, 0, g_pApp->getWindowWidth()*getWindow()->getContentScale(), g_pApp->getWindowHeight()*getWindow()->getContentScale()));
            gl::setMatricesWindow(windowSize);;
            gl::draw(m_loserTex, bounds);
            break;
        case P2_WINS:
            texRatio = 1;
            xoffset = (windowSize.x - dim*.7f) / 2;
            yoffset = (windowSize.y - (dim*.7f*texRatio)) / 2;
            bounds.set(xoffset, yoffset, windowSize.x - xoffset, windowSize.y - yoffset);

            gl::setViewport(Area(0, 0, g_pApp->getWindowWidth()*getWindow()->getContentScale()/2, g_pApp->getWindowHeight()*getWindow()->getContentScale()));
            gl::setMatricesWindow(windowSize);;
            gl::draw(m_loserTex, bounds);

            gl::setViewport(Area(g_pApp->getWindowWidth()*getWindow()->getContentScale()/2, 0, g_pApp->getWindowWidth()*getWindow()->getContentScale(), g_pApp->getWindowHeight()*getWindow()->getContentScale()));
            gl::setMatricesWindow(windowSize);;
            gl::draw(m_winnerTex, bounds);
            break;
        case CREDITS:
            texRatio = 0.5;
            xoffset = (windowSize.x - dim) / 2;
            yoffset = (windowSize.y - (dim*texRatio)) / 2;
            bounds.set(xoffset, yoffset, windowSize.x - xoffset, windowSize.y - yoffset);

            gl::setViewport(Area(0, 0, g_pApp->getWindowWidth()*getWindow()->getContentScale(), g_pApp->getWindowHeight()*getWindow()->getContentScale()));
            gl::setMatricesWindow(windowSize);
            gl::draw(m_creditsTex, bounds);
            break;
        default:
            break;
    }
//    gl::pushMatrices();


#if ! defined(CINDER_GLES)
    gl::disableWireframe();
#endif

    // Draw statistics
    if (m_displayStats)
    {
        g_pStats->display();
    }

    // Draw gui
    if (m_displayGui)
    {
//        g_pGui->draw();
    }
}
