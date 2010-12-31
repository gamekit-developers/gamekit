/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Xavier T..

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
#include "gkStats.h"
#include "gkCommon.h"


gkStats::gkStats()
	:	m_start(0),
		m_render(0),
		m_logicBricks(0),
		m_logicNodes(0),
		m_physics(0),
		m_sound(0),
		m_bufswaplod(0),
		m_animations(0),
		m_lastRender(0),
		m_lastLogicBricks(0),
		m_lastLogicNodes(0),
		m_lastPhysics(0),
		m_lastDbvt(0),
		m_lastSound(0),
		m_lastBufswaplod(0),
		m_lastAnimations(0),
		m_lastTotal(0)
{
	m_clock = new Ogre::Timer();
	resetClock();
}

void gkStats::resetClock(void)
{
	m_start = 0;
	m_render = 0;
	m_logicBricks = 0;
	m_logicNodes = 0;
	m_physics = 0;
	m_dbvt = 0;
	m_sound = 0;
	m_bufswaplod = 0;
	m_animations = 0;
}

void gkStats::startClock(void)
{
	m_start = m_clock->getMicroseconds();
}

void gkStats::nextFrame(void)
{
	m_lastRender = m_render;
	m_lastLogicBricks = m_logicBricks;
	m_lastLogicNodes = m_logicNodes;
	m_lastPhysics = m_physics;
	m_lastDbvt = m_dbvt;
	m_lastSound = m_sound;
	m_lastBufswaplod = m_bufswaplod;
	m_lastAnimations = m_animations;

	resetClock();

	m_lastTotal = m_clock->getMicroseconds();
	m_clock->reset();
}

void gkStats::stopRenderClock(void)
{
	m_render += m_clock->getMicroseconds() - m_start;
}

void gkStats::stopLogicBricksClock(void)
{
	m_logicBricks += m_clock->getMicroseconds() - m_start;
}

void gkStats::stopLogicNodesClock(void)
{
	m_logicNodes += m_clock->getMicroseconds() - m_start;
}

void gkStats::stopPhysicsClock(void)
{
	m_physics += m_clock->getMicroseconds() - m_start;
}

void gkStats::stopDbvtClock(void)
{
	m_dbvt += m_clock->getMicroseconds() - m_start;
}

void gkStats::stopSoundClock(void)
{
	m_sound += m_clock->getMicroseconds() - m_start;
}

void gkStats::stopBufSwapLodClock(void)
{
	m_bufswaplod += m_clock->getMicroseconds() - m_start;
}

void gkStats::stopAnimationsClock(void)
{
	m_animations += m_clock->getMicroseconds() - m_start;
}

UT_IMPLEMENT_SINGLETON(gkStats);
