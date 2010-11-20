/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Charlie C.

    Contributor(s): none yet.
-------------------------------------------------------------------------------
  This software is provided 'as-is', without any express or implied
  warranty. In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
-------------------------------------------------------------------------------
*/
#ifndef _gkApplication_h_
#define _gkApplication_h_

#include "gkMathUtils.h"

class gkGameLevel;
class gkGamePlayer;
class gkGameController;

#define GK_CPP_VERBOSE_LOG false

namespace gkAppData
{
static const gkScalar gkJoyMax            = 32767.f;
static const gkScalar gkJoyDeadZone       = gkJoyMax / 5.f;
static const gkScalar gkJoyReboundFac     = 2.95f;
static const gkScalar gkJoyWalkToRunTol   = 0.875f;
static const gkScalar gkFixedTickDelta    = 1.f / 60.f;
static const gkScalar gkFixedTickDelta2   = 2.f* gkFixedTickDelta;
static const gkScalar gkAnimationFast     = 1.27f;
static const gkScalar gkAnimationAttack   = 1.35f;
static const gkScalar gkPlayerHeadZ       = 0.25f;
static const gkScalar gkPlayerImpulseZ    = 8.25f;
static const gkScalar gkCameraTol         = 0.25f;
static const gkScalar gkGlobalActionBlend = 35.98765f;
};


enum gkLevel
{
	GK_LEVEL_EXIT,
	GK_LEVEL_PICKUP,
	GK_LEVEL_KILL_THEM_ALL,
};


enum gkPlayerStates
{
	GK_PLAY_IDLE,
	GK_PLAY_WALK,
	GK_PLAY_RUN,
	GK_PLAY_JUMP,
	GK_PLAY_ATTACK_0,
	GK_PLAY_ATTACK_1,
	GK_PLAY_RUN_ATTACK_0,
	GK_PLAY_RUN_ATTACK_1,
	GK_PLAY_ATTACK_COMBO,
	GK_PLAY_LEDGE,
	GK_PLAY_LAND,
};

enum gkAnimationStates
{
	GK_ANIM_IDLE,
	GK_ANIM_IDLE_NASTY,
	GK_ANIM_IDLE_CURRENT,
	GK_ANIM_WALK,
	GK_ANIM_RUN,
	GK_ANIM_JUMP,
	GK_ANIM_WHIP,
	GK_ANIM_KICK,
	GK_ANIM_WALL_FLIP,
	GK_ANIM_COMBO_ATTACK,
	GK_ANIM_MAX,
};

class gkAnimationPlayer;
typedef gkAnimationPlayer* gkAnimations[GK_ANIM_MAX];


struct gkJoystickAxisState
{
	gkVector2 m_absolute;
	gkVector2 m_normal;
	gkScalar  m_length;
	gkScalar  m_nlen;



	GK_INLINE bool isInFactor(const gkScalar& fac) const
	{
		return gkAbs(m_nlen) < fac;
	}

	GK_INLINE void normalize(void)
	{
		m_normal = m_absolute / gkAppData::gkJoyMax;
		m_normal.x = gkClampf(m_normal.x, -1.f, 1.f);
		m_normal.y = gkClampf(m_normal.y, -1.f, 1.f);

		m_length = m_absolute.length();
		m_nlen = m_normal.length();
	}

	GK_INLINE bool isDead(void) const
	{
		return gkAbs(m_length) < gkAppData::gkJoyDeadZone;
	}

	GK_INLINE bool isDeadUD(void) const
	{
		return gkAbs(m_absolute.x) < gkAppData::gkJoyDeadZone;
	}

	GK_INLINE bool isDeadLR(void) const
	{
		return gkAbs(m_absolute.y) < gkAppData::gkJoyDeadZone;
	}

};

enum gkJoystickButtons
{
	GK_JOY_BUTTON_1,
	GK_JOY_BUTTON_2,
	GK_JOY_BUTTON_3,
	GK_JOY_BUTTON_4,
	GK_JOY_BUTTON_5,
	GK_JOY_BUTTON_6,
	GK_JOY_BUTTON_7,
	GK_JOY_BUTTON_8,
	GK_JOY_BUTTON_9,
	GK_JOY_BUTTON_10,
	GK_JOY_BUTTON_11,
	GK_JOY_BUTTON_12,
};


#define GK_RESOURCE_MAPS    "Maps.blend"
#define GK_RESOURCE_PLAYER    "Momo.blend"
#define GK_RESOURCE_NPC        "Rat.blend"

#define GK_RESOURCE_PLAYER_MESH "MeshMomo"
#define GK_RESOURCE_PLAYER_SKEL "RigMomo"
#define GK_RESOURCE_PLAYER_PHYS "MomoPhysics"
#define GK_RESOURCE_PLAYER_VIEW "MainCamera"
#define GK_RESOURCE_PLAYER_ZROT "zRot"
#define GK_RESOURCE_PLAYER_XROT "xRot"

#define FSM_TRANSITION_WHEN(fsm, f, t, e) \
    fsm->addTransition(f, t)->when(new gkFSM::LogicEvent<gkGamePlayer>(this, &gkGamePlayer::e))
#define FSM_TRANSITION_WHEN_DELAY(fsm, f, t, d, e) \
    fsm->addTransition(f, t, d)->when(new gkFSM::LogicEvent<gkGamePlayer>(this, &gkGamePlayer::e))

#define FSM_START_TRIG(fsm, s, e) \
    fsm->addStartTrigger(s, new gkFSM::LogicTrigger<gkGamePlayer>(this, &gkGamePlayer::e))
#define FSM_END_TRIG(fsm, s, e) \
    fsm->addEndTrigger(s, new gkFSM::LogicTrigger<gkGamePlayer>(this, &gkGamePlayer::e))


class gkHUD;
class gkHUDElement;


#endif//_gkApplication_h_
