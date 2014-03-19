#pragma once
#include "cinder/CinderResources.h"

//#define RES_MY_RES			CINDER_RESOURCE( ../resources/, image_name.png, 128, IMAGE )
#define RES_SGUI_FONT           CINDER_RESOURCE( ../../../libraries/cinder_0.8.5_vc2010/blocks/SimpleGUI/resources/, pf_tempesta_seven.ttf, 128, CI_FONT )
#define RES_STATS_FONT          CINDER_RESOURCE( ../resources/, ProggySquare.ttf,       129, CI_FONT )

#define RES_CUBE_OBJ            CINDER_RESOURCE( ../resources/, cube.obj,               130, CI_OBJ )
#define RES_SPHERE_OBJ          CINDER_RESOURCE( ../resources/, sphere.obj,             132, CI_OBJ )
#define RES_SQUARE_OBJ          CINDER_RESOURCE( ../resources/, square.obj,             133, CI_OBJ )
#define RES_TORUS_OBJ           CINDER_RESOURCE( ../resources/, torus.obj,              134, CI_OBJ )

#define RES_HIGHLIGHT_TEX       CINDER_RESOURCE( ../resources/, select_face.png,        141, CI_TEX )
#define RES_TITLE_TEX           CINDER_RESOURCE( ../resources/, title.png,              142, CI_TEX )
#define RES_RULES_TEX           CINDER_RESOURCE( ../resources/, rules.png,              143, CI_TEX )
#define RES_READY_TEX           CINDER_RESOURCE( ../resources/, ready.png,              144, CI_TEX )
#define RES_WINNER_TEX          CINDER_RESOURCE( ../resources/, winner.png,             145, CI_TEX )
#define RES_LOSER_TEX           CINDER_RESOURCE( ../resources/, loser.png,              146, CI_TEX )
#define RES_CREDITS_TEX         CINDER_RESOURCE( ../resources/, credits.png,            147, CI_TEX )
#define RES_P1_CURSOR_TEX       CINDER_RESOURCE( ../resources/, cursor_p1.png,          148, CI_TEX )
#define RES_P2_CURSOR_TEX       CINDER_RESOURCE( ../resources/, cursor_p2.png,          149, CI_TEX )



//#ifdef __APPLE__    // Apple devices only load MP3's via cinder
//#define RES_HIT1_SOUND          CINDER_RESOURCE( ../resources/, sound.mp3,              160, CI_AUDIO )
//#else               // Windows only loads WAV's via cinder
#define RES_SFX1_SOUND          CINDER_RESOURCE( ../resources/, sfx1.wav,               160, CI_AUDIO )
#define RES_SFX2_SOUND          CINDER_RESOURCE( ../resources/, sfx2.wav,               161, CI_AUDIO )
#define RES_SFX3_SOUND          CINDER_RESOURCE( ../resources/, sfx3.wav,               162, CI_AUDIO )
//#endif

#define RES_VERT_SHADER         CINDER_RESOURCE( ../resources/, vert.glsl,            190, CI_SHADER )
#define RES_FRAG_SHADER         CINDER_RESOURCE( ../resources/, frag.glsl,            191, CI_SHADER )
