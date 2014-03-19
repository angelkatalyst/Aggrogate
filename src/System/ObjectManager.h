
#pragma once

#include "Common.h"
#include "Stats.h"
#include "Input.h"
#include <vector>

namespace gen
{
// protos
class ObjectData;
class SpriteSheet;
class Player;
class Particle;
class World;

class ObjectManager
{
public:
    ObjectManager();
    ~ObjectManager();
    void Update(float dt);
    void Draw();
    void UpdateSoundTimer(float dt);    // this is its own function because the timing is very sensitive
    Player* AddPlayer();
    void Start();                       // transition from Ready to Playing state.
    void Restart();

    GameState       m_gameState;
    bool            m_gravity;          // toggle to enable or disable gravity TODO: physics state?
    bool            m_displayStats;
    bool            m_displayGui;
    bool            m_fullscreen;
    bool            m_wireframe;
    bool            m_playMusic;

    std::vector<ObjectData*> m_objects; // collection of assets
    Player*         m_pPlayer1;
    Player*         m_pPlayer2;
    World*          m_pWorld;

    ci::gl::Fbo     m_fbo;
    ci::gl::Fbo::Format m_fboFormat;

    ci::gl::Texture m_titleTex;
    ci::gl::Texture m_rulesTex;
    ci::gl::Texture m_readyTex;
    ci::gl::Texture m_winnerTex;
    ci::gl::Texture m_loserTex;
    ci::gl::Texture m_creditsTex;
//    ci::audio::SourceRef m_sound;

    double          m_playtime;
    double          m_creditsTimer;

    ci::gl::GlslProg    m_shader;
};
}
