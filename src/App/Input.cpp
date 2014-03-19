#include "Input.h"
#include "ObjectManager.h"
#include "App.h"
#include "ObjectData.h"
#include "PhysicsData.h"
#include "RenderData.h"
#include "Player.h"
#include "World.h"

using namespace ci;

std::map<unsigned int, gen::Input*> gen::Input::gamepadDeviceMap;

/**************************************
Gamepad code
**************************************/
#ifdef _DEBUG
//bool verbose = true;
#else
//bool verbose = false;
#endif
bool verbose = true;

bool gen::Input::onButtonDown(void * sender, const char * eventID, void * eventData, void * context) {
	struct Gamepad_buttonEvent * event;
	
	event = (Gamepad_buttonEvent*)eventData;
	if (verbose) {
//        printf("Button %u down (%d) on device %u at %f\n", event->buttonID, (int) event->down, event->device->deviceID, event->timestamp);
//        ci::app::console() << "Button " << event->buttonID << " down " << event->down << " on device " << event->device->deviceID << " at " << event->timestamp << std::endl;
	}

    gen::Input* pPlayerInput = NULL;
    pPlayerInput = gamepadDeviceMap[event->device->deviceID];

    if (pPlayerInput)
    {
        if (g_pObjectManager->m_gameState == TITLE)
        {
            g_pObjectManager->m_gameState = RULES;
        }
        else if (g_pObjectManager->m_gameState == RULES)
        {
            g_pObjectManager->m_gameState = READY;
            g_pObjectManager->Restart();
        }
        else if (g_pObjectManager->m_gameState == P1_WINS ||
                 g_pObjectManager->m_gameState == P2_WINS)
        {
            g_pObjectManager->m_gameState = READY;
            g_pObjectManager->Restart();
        }
        else
        {

#ifdef __APPLE__
            if (event->device->vendorID == 1356)    // PS3 Controller
            {
                //            if (event->buttonID == 14) // "Cross"
                //            if (event->buttonID == 15) // "Square"
                
                if (g_pObjectManager->m_gameState == PLAYING)
                {
                    if (event->buttonID == 10 || event->buttonID == 15) // LB or "Cross"
                    {
                        pPlayerInput->m_pPlayer->Shoot(ADD_LIGHT);
                    }
                    if (event->buttonID == 11 || event->buttonID == 14) // RB or "Square
                    {
                        pPlayerInput->m_pPlayer->Shoot(ADD_DARK);
                    }
                }
                if (event->buttonID == 0) // "Select"
                {
                    if (g_pObjectManager->m_gameState == READY)
                    {
                        g_pObjectManager->m_pWorld->NextLevel();
                        g_pObjectManager->Restart();
                    }
                }
                if (event->buttonID == 3) // "Start"
                {
                    if (g_pObjectManager->m_gameState == READY)
                    {
                        g_pObjectManager->Restart();
                        g_pObjectManager->m_gameState = PLAYING;
                    }
                }
            }
            else                                    // 360 Controller
            {
                if (g_pObjectManager->m_gameState == PLAYING)
                {
                    if (event->buttonID == 13 || event->buttonID == 16) // LB or A
                    {
                        pPlayerInput->m_pPlayer->Shoot(ADD_LIGHT);
                    }
                    if (event->buttonID == 14 || event->buttonID == 18) // RB or X
                    {
                        pPlayerInput->m_pPlayer->Shoot(ADD_DARK);
                    }
                }
                if (event->buttonID == 10) // "Select"
                {
                    if (g_pObjectManager->m_gameState == READY)
                    {
                        g_pObjectManager->m_pWorld->NextLevel();
                        g_pObjectManager->Restart();
                    }
                }
                if (event->buttonID == 9) // "Start"
                {
                    if (g_pObjectManager->m_gameState == READY)
                    {
                        g_pObjectManager->Restart();
                        g_pObjectManager->m_gameState = PLAYING;
                    }
                }
            }
#else
#ifdef _DEBUG
            if (event->buttonID == 0) // "A"
            {
                pPlayerInput->m_pPlayer->Shoot(ADD_DARK);
            }
            if (event->buttonID == 1) // "B"
            {
                pPlayerInput->m_pPlayer->Shoot(REMOVE);
            }
            if (event->buttonID == 2) // "X"
            {
                pPlayerInput->m_pPlayer->Shoot(ADD_LIGHT);
            }
            if (event->buttonID == 3) // "Y"
            {
                pPlayerInput->m_pPlayer->Shoot(ADD_BLACK);
            }
            if (event->buttonID == 7) // "Start"
            {
                g_pObjectManager->m_pWorld->DumpWorld();
            }
#endif
            if (g_pObjectManager->m_gameState == PLAYING)
            {
                if (event->buttonID == 4) // LB
                {
                    pPlayerInput->m_pPlayer->Shoot(ADD_LIGHT);
                }
                if (event->buttonID == 5) // RB
                {
                    pPlayerInput->m_pPlayer->Shoot(ADD_DARK);
                }
            }
            if (event->buttonID == 6) // "Select"
            {
                if (g_pObjectManager->m_gameState == READY)
                {
                    g_pObjectManager->m_pWorld->NextLevel();
                    g_pObjectManager->Restart();
                }
            }
            if (event->buttonID == 7) // "Start"
            {
                if (g_pObjectManager->m_gameState == READY)
                {
                    g_pObjectManager->Restart();
                    g_pObjectManager->m_gameState = PLAYING;
                }
            }
#endif
        }
    }

    return true;
}

bool gen::Input::onButtonUp(void * sender, const char * eventID, void * eventData, void * context) {
	struct Gamepad_buttonEvent * event;
	
	event = (Gamepad_buttonEvent*)eventData;
	if (verbose) {
//        printf("Button %u up (%d) on device %u at %f\n", event->buttonID, (int) event->down, event->device->deviceID, event->timestamp);
//        ci::app::console() << "Button " << event->buttonID << " up " << event->down << " on device " << event->device->deviceID << " at " << event->timestamp << std::endl;
	}

    gen::Input* pPlayerInput = NULL;
    pPlayerInput = gamepadDeviceMap[event->device->deviceID];

    if (pPlayerInput)
    {
#ifdef __APPLE__
        if (event->device->vendorID == 1356)    // PS3 Controller
        {
//            if (event->buttonID == 14) // "Cross"
//            if (event->buttonID == 15) // "Square"
        }
        else                                    // 360 Controller
        {
//            if (event->buttonID == 16) // "A"
//            if (event->buttonID == 18) // "X"
        }
#else
//        if (event->buttonID == 0) // "A"
//        if (event->buttonID == 2) // "X"
#endif
    }

	return true;
}

bool gen::Input::onAxisMoved(void * sender, const char * eventID, void * eventData, void * context) {
	struct Gamepad_axisEvent * event;
	
	event = (Gamepad_axisEvent*)eventData;
	if (verbose) {
//        printf("Axis %u moved to %f on device %u at %f\n", event->axisID, event->value, event->device->deviceID, event->timestamp);
        if (event->value <= -0.3f || event->value >= 0.3f)
        {
            ci::app::console() << "Axis " << event->axisID << " moved to " << event->value << " on device " << event->device->deviceID << " at " << event->timestamp << std::endl;
        }
	}

    gen::Input* pPlayerInput = NULL;
    pPlayerInput = gamepadDeviceMap[event->device->deviceID];

    if (pPlayerInput)
    {
#ifdef __APPLE__
        if (event->device->vendorID == 1356)    // PS3 Controller
        {
            if (g_pObjectManager->m_gameState == PLAYING)
            {
                if (event->axisID == 0) // Left Stick or Right Stick, X axis
                {
                    if (event->value <= -0.3f || event->value >= 0.3f)
                    {
                        pPlayerInput->m_pPlayer->m_cursorDelta.x = event->value * fabs(event->value) * 0.7;
                    }
                    else
                    {
                        pPlayerInput->m_pPlayer->m_cursorDelta.x = 0;
                    }
                }
                if (event->axisID == 1) // Left Stick or Right Stick, y axis
                {
                    if (event->value <= -0.3f || event->value >= 0.3f)
                    {
                        pPlayerInput->m_pPlayer->m_cursorDelta.y = event->value * fabs(event->value) * 0.7;
                    }
                    else
                    {
                        pPlayerInput->m_pPlayer->m_cursorDelta.y = 0;
                    }
                }
            }
        }
        else                                    // 360 Controller
        {
            if (g_pObjectManager->m_gameState == PLAYING)
            {
                if (event->axisID == 0) // Right Stick, X axis
                {
                    if (event->value <= -0.3f || event->value >= 0.3f)
                    {
                        pPlayerInput->m_pPlayer->m_cursorDelta.x = event->value*.5f;
                    }
                    else
                    {
                        pPlayerInput->m_pPlayer->m_cursorDelta.x = 0;
                    }
                }
                if (event->axisID == 1) // Right Stick, y axis
                {
                    if (event->value <= -0.3f || event->value >= 0.3f)
                    {
                        pPlayerInput->m_pPlayer->m_cursorDelta.y = event->value*.5f;
                    }
                    else
                    {
                        pPlayerInput->m_pPlayer->m_cursorDelta.y = 0;
                    }
                }
            }
        }
#else
#ifdef _DEBUG
        if (g_pObjectManager->m_gameState == READY || g_pObjectManager->m_gameState == PLAYING)
        {
            if (event->axisID == 0) // Left Stick, X axis
            {
                if (event->value <= -0.3f || event->value >= 0.3f)
                {
                    pPlayerInput->m_pPlayer->m_cam.dx = event->value;
                }
                else
                {
                    pPlayerInput->m_pPlayer->m_cam.dx = 0;
                }
            }
            if (event->axisID == 1) // Left Stick, y axis
            {
                if (event->value <= -0.3f || event->value >= 0.3f)
                {
                    pPlayerInput->m_pPlayer->m_cam.dy = -event->value;
                }
                else
                {
                    pPlayerInput->m_pPlayer->m_cam.dy = 0;
                }
            }
        }
#elsif 0
        if (g_pObjectManager->m_gameState == PLAYING)
        {
            if (event->axisID == 0) // Right Stick, X axis
            {
                if (event->value <= -0.3f || event->value >= 0.3f)
                {
                    pPlayerInput->m_pPlayer->m_cursorDelta.x = event->value*.5f;
                }
                else
                {
                    pPlayerInput->m_pPlayer->m_cursorDelta.x = 0;
                }
            }
            if (event->axisID == 1) // Right Stick, y axis
            {
                if (event->value <= -0.3f || event->value >= 0.3f)
                {
                    pPlayerInput->m_pPlayer->m_cursorDelta.y = event->value*.5f;
                }
                else
                {
                    pPlayerInput->m_pPlayer->m_cursorDelta.y = 0;
                }
            }
        }
#endif

        if (g_pObjectManager->m_gameState == PLAYING)
        {
            if (event->axisID == 4) // Right Stick, X axis
            {
                if (event->value <= -0.3f || event->value >= 0.3f)
                {
                    pPlayerInput->m_pPlayer->m_cursorDelta.x = event->value*.5f;
                }
                else
                {
                    pPlayerInput->m_pPlayer->m_cursorDelta.x = 0;
                }
            }
            if (event->axisID == 3) // Right Stick, y axis
            {
                if (event->value <= -0.3f || event->value >= 0.3f)
                {
                    pPlayerInput->m_pPlayer->m_cursorDelta.y = event->value*.5f;
                }
                else
                {
                    pPlayerInput->m_pPlayer->m_cursorDelta.y = 0;
                }
            }
        }

        if (event->axisID == 2) // Shoulder triggers
        {
            if (event->value <= -0.2f)      // right shoulder
            {
//                pPlayerInput->m_pPlayer->Shoot();
            }
            else if (event->value >= 0.2f)  // left shoulder                
            {
            }
        }
#endif
    }
    
    return true;
}

bool gen::Input::onDeviceAttached(void * sender, const char * eventID, void * eventData, void * context) {
	struct Gamepad_device * device;
	
	device = (Gamepad_device*)eventData;
	if (verbose) {
//		printf("Device ID %u attached (vendor = 0x%X; product = 0x%X)\n", device->deviceID, device->vendorID, device->productID);
        ci::app::console() << "Device ID " << device->deviceID << " attached (vendor = " << device->vendorID << "; product = " << device->productID <<")" << std::endl;
    }
	device->eventDispatcher->registerForEvent(device->eventDispatcher, GAMEPAD_EVENT_BUTTON_DOWN, onButtonDown, device);
	device->eventDispatcher->registerForEvent(device->eventDispatcher, GAMEPAD_EVENT_BUTTON_UP, onButtonUp, device);
	device->eventDispatcher->registerForEvent(device->eventDispatcher, GAMEPAD_EVENT_AXIS_MOVED, onAxisMoved, device);

    if (gamepadDeviceMap.size() == 0)
    {
        // g_pInputP1 is initialized by default
        gamepadDeviceMap[device->deviceID] = g_pInputP1;
    }
    else if (gamepadDeviceMap.size() == 1)
    {
        // g_pInputP2 is initialized by default
        gamepadDeviceMap[device->deviceID] = g_pInputP2;
    }
    else
    {
        ci::app::console() << "Only 2 players are supported!" << std::endl;
    }


	return true;
}

bool gen::Input::onDeviceRemoved(void * sender, const char * eventID, void * eventData, void * context) {
	struct Gamepad_device * device;
	
	device = (Gamepad_device*)eventData;
	if (verbose) {
		printf("Device ID %u removed\n", device->deviceID);
	}
    // TODO : put gamepad/input delete here?
	return true;
}


gen::Input::Input(Player* pPlayer) : m_pPlayer(pPlayer), m_keyState(), m_selectState(false), m_selectStart(0.0f), m_selectPos(0,0), m_timeTap(0)
{
}

#define POLL_ITERATION_INTERVAL 10

void gen::Input::Update(float dt)
{
	static unsigned int iterationsToNextPoll = POLL_ITERATION_INTERVAL;
	
	iterationsToNextPoll--;
	if (iterationsToNextPoll == 0) {
		Gamepad_detectDevices();
		iterationsToNextPoll = POLL_ITERATION_INTERVAL;
	}
    
    Gamepad_processEvents();

    // handle constant "button down" events, like flying

//    if (g_pObjectManager->m_gameState == PLAYING && m_pPlayer->m_alive)
    {
        if (m_keyState.MOVE_LEFT_STATE)
        {
        }
        else if (m_keyState.MOVE_RIGHT_STATE)
        {
        }
    }
    
//    if (g_pObjectManager->m_gameState == PLAYING  && m_pPlayer->m_alive)
    {
        if (m_keyState.MOVE_UP_STATE)
        {
        }
        else if (m_keyState.MOVE_DOWN_STATE)
        {
        }
    }
}

bool gen::Input::KeyDown(ci::app::KeyEvent event)
{

    if (g_pObjectManager->m_gameState == TITLE)
    {
        g_pObjectManager->m_gameState = RULES;
    }
    else if (g_pObjectManager->m_gameState == RULES)
    {
        g_pObjectManager->m_gameState = READY;
        g_pObjectManager->Restart();
    }
    else
    {
        switch (event.getCode())
        {
#ifdef _DEBUG
            case TOGGLE_STATS:
                g_pObjectManager->m_displayStats = !g_pObjectManager->m_displayStats;
                break;

            case TOGGLE_WIREFRAME:
                g_pObjectManager->m_wireframe = !g_pObjectManager->m_wireframe;
                break;

            case ADVANCE_GAME_STATE:
                switch (g_pObjectManager->m_gameState)
                {
                    case TITLE:
                        g_pObjectManager->m_gameState = RULES;
                        break;
                    case RULES:
                        g_pObjectManager->m_gameState = READY;
                        break;
                    case READY:
                        g_pObjectManager->m_gameState = PLAYING;
                        break;
                    case PLAYING:
                        g_pObjectManager->m_gameState = P1_WINS;
                        break;
                    case P1_WINS:
                        g_pObjectManager->m_gameState = P2_WINS;
                        break;
                    case P2_WINS:
                        g_pObjectManager->m_gameState = CREDITS;
                        break;
                    case CREDITS:
                        g_pObjectManager->m_gameState = TITLE;
                        break;
                    default:
                        break;
                }
                break;
#endif
            case START:
                if (g_pObjectManager->m_gameState == READY)
                {
                    g_pObjectManager->Restart();
                    g_pObjectManager->m_gameState = PLAYING;
                }
                else if (g_pObjectManager->m_gameState == P1_WINS ||
                         g_pObjectManager->m_gameState == P2_WINS)
                {
                    g_pObjectManager->m_gameState = READY;
                    g_pObjectManager->Restart();
                }
                break;

            case NEXT_LEVEL:
                if (g_pObjectManager->m_gameState == READY)
                {
                    g_pObjectManager->m_pWorld->NextLevel();
                    g_pObjectManager->Restart();
                }
                break;

            case TOGGLE_FULLSCREEN:
                g_pObjectManager->m_fullscreen = !g_pObjectManager->m_fullscreen;
                g_pApp->setFullScreen(g_pObjectManager->m_fullscreen);
                g_pObjectManager->m_pPlayer1->Reset();
                g_pObjectManager->m_pPlayer2->Reset();
                g_pObjectManager->m_fbo = gl::Fbo(g_pApp->getWindowWidth()*ci::app::getWindow()->getContentScale(),
                                                  g_pApp->getWindowHeight()*ci::app::getWindow()->getContentScale(),
                                                  g_pObjectManager->m_fboFormat);
                break;

            case TOGGLE_GUI:
                g_pObjectManager->m_displayGui = !g_pObjectManager->m_displayGui;
                break;

            case SHOOT_LIGHT_P1:
                g_pObjectManager->m_pPlayer1->Shoot(ADD_LIGHT);
                break;

            case SHOOT_DARK_P1:
                g_pObjectManager->m_pPlayer1->Shoot(ADD_DARK);
                break;

            case MOVE_UP_P1:
                g_pInputP1->m_keyState.MOVE_UP_STATE = true;
                if (g_pObjectManager->m_gameState == PLAYING)
                    g_pObjectManager->m_pPlayer1->m_cursorDelta.y = -.5f;
                break;

            case MOVE_DOWN_P1:
                g_pInputP1->m_keyState.MOVE_DOWN_STATE = true;
                if (g_pObjectManager->m_gameState == PLAYING)
                    g_pObjectManager->m_pPlayer1->m_cursorDelta.y = .5f;
                break;

            case MOVE_LEFT_P1:
                g_pInputP1->m_keyState.MOVE_LEFT_STATE = true;
                if (g_pObjectManager->m_gameState == PLAYING)
                    g_pObjectManager->m_pPlayer1->m_cursorDelta.x = -.5f;
                break;

            case MOVE_RIGHT_P1:
                g_pInputP1->m_keyState.MOVE_RIGHT_STATE = true;
                if (g_pObjectManager->m_gameState == PLAYING)
                    g_pObjectManager->m_pPlayer1->m_cursorDelta.x = .5f;
                break;

            case SHOOT_LIGHT_P2:
                g_pObjectManager->m_pPlayer2->Shoot(ADD_LIGHT);
                break;

            case SHOOT_DARK_P2:
                g_pObjectManager->m_pPlayer2->Shoot(ADD_DARK);
                break;

            case MOVE_UP_P2:
                g_pInputP2->m_keyState.MOVE_UP_STATE = true;
                if (g_pObjectManager->m_gameState == PLAYING)
                    g_pObjectManager->m_pPlayer2->m_cursorDelta.y = -.5f;
                break;

            case MOVE_DOWN_P2:
                g_pInputP2->m_keyState.MOVE_DOWN_STATE = true;
                if (g_pObjectManager->m_gameState == PLAYING)
                    g_pObjectManager->m_pPlayer2->m_cursorDelta.y = .5f;
                break;

            case MOVE_LEFT_P2:
                g_pInputP2->m_keyState.MOVE_LEFT_STATE = true;
                if (g_pObjectManager->m_gameState == PLAYING)
                    g_pObjectManager->m_pPlayer2->m_cursorDelta.x = -.5f;
                break;

            case MOVE_RIGHT_P2:
                g_pInputP2->m_keyState.MOVE_RIGHT_STATE = true;
                if (g_pObjectManager->m_gameState == PLAYING)
                    g_pObjectManager->m_pPlayer2->m_cursorDelta.x = .5f;
                break;

            case RESTART:
                g_pObjectManager->Restart();
                break;

            case ci::app::KeyEvent::KEY_ESCAPE:
                g_pObjectManager->m_gameState = CREDITS;
                break;

            default:
                return false;   // the input was not handled
                break;
        }
    }
    return true;    // the input was handled
}

bool gen::Input::KeyUp(ci::app::KeyEvent event)
{
    switch (event.getCode())
    {
        case MOVE_UP_P1:
            g_pInputP1->m_keyState.MOVE_UP_STATE = false;
            if (g_pInputP1->m_keyState.MOVE_DOWN_STATE == true)
            {
                g_pObjectManager->m_pPlayer1->m_cursorDelta.y = .5f;
            }
            else
            {
                g_pObjectManager->m_pPlayer1->m_cursorDelta.y = 0.f;
            }
            break;

        case MOVE_DOWN_P1:
            g_pInputP1->m_keyState.MOVE_DOWN_STATE = false;
            if (g_pInputP1->m_keyState.MOVE_UP_STATE == true)
            {
                g_pObjectManager->m_pPlayer1->m_cursorDelta.y = -.5f;
            }
            else
            {
                g_pObjectManager->m_pPlayer1->m_cursorDelta.y = 0.f;
            }
            break;

        case MOVE_LEFT_P1:
            g_pInputP1->m_keyState.MOVE_LEFT_STATE = false;
            if (g_pInputP1->m_keyState.MOVE_RIGHT_STATE == true)
            {
                g_pObjectManager->m_pPlayer1->m_cursorDelta.x = .5f;
            }
            else
            {
                g_pObjectManager->m_pPlayer1->m_cursorDelta.x = 0.f;
            }
            break;

        case MOVE_RIGHT_P1:
            g_pInputP1->m_keyState.MOVE_RIGHT_STATE = false;
            if (g_pInputP1->m_keyState.MOVE_LEFT_STATE == true)
            {
                g_pObjectManager->m_pPlayer1->m_cursorDelta.x = -.5f;
            }
            else
            {
                g_pObjectManager->m_pPlayer1->m_cursorDelta.x = 0.f;
            }
            break;

        case MOVE_UP_P2:
            g_pInputP2->m_keyState.MOVE_UP_STATE = false;
            if (g_pInputP2->m_keyState.MOVE_DOWN_STATE == true)
            {
                g_pObjectManager->m_pPlayer2->m_cursorDelta.y = .5f;
            }
            else
            {
                g_pObjectManager->m_pPlayer2->m_cursorDelta.y = 0.f;
            }
            break;

        case MOVE_DOWN_P2:
            g_pInputP2->m_keyState.MOVE_DOWN_STATE = false;
            if (g_pInputP2->m_keyState.MOVE_UP_STATE == true)
            {
                g_pObjectManager->m_pPlayer2->m_cursorDelta.y = -.5f;
            }
            else
            {
                g_pObjectManager->m_pPlayer2->m_cursorDelta.y = 0.f;
            }
            break;

        case MOVE_LEFT_P2:
            g_pInputP2->m_keyState.MOVE_LEFT_STATE = false;
            if (g_pInputP2->m_keyState.MOVE_RIGHT_STATE == true)
            {
                g_pObjectManager->m_pPlayer2->m_cursorDelta.x = .5f;
            }
            else
            {
                g_pObjectManager->m_pPlayer2->m_cursorDelta.x = 0.f;
            }
            break;

        case MOVE_RIGHT_P2:
            g_pInputP2->m_keyState.MOVE_RIGHT_STATE = false;
            if (g_pInputP2->m_keyState.MOVE_LEFT_STATE == true)
            {
                g_pObjectManager->m_pPlayer2->m_cursorDelta.x = -.5f;
            }
            else
            {
                g_pObjectManager->m_pPlayer2->m_cursorDelta.x = 0.f;
            }
            break;

        default:
            return false;   // the input was not handled
            break;
    }

    return true;
}

bool gen::Input::MouseDown(ci::app::MouseEvent event)
{
    return true;
}

bool gen::Input::MouseDrag(ci::app::MouseEvent event)
{
    return false;
}

bool gen::Input::MouseUp(ci::app::MouseEvent event)
{
    return false;
}

bool gen::Input::MouseWheel(ci::app::MouseEvent event)
{
    return false;
}

bool gen::Input::TouchesBegan(ci::app::TouchEvent event)
{
    return false;
}

bool gen::Input::TouchesMoved(ci::app::TouchEvent event)
{
    return false;
}

bool gen::Input::TouchesEnded(ci::app::TouchEvent event)
{
    return false;
}
