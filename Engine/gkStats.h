/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Xavier T.

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
#ifndef _gkStats_h_
#define _gkStats_h_

#include "utSingleton.h"
#include "OgreTimer.h"

class gkStats : public utSingleton<gkStats>
{

private:
	Ogre::Timer* m_clock;

	unsigned long m_start;
	unsigned long m_render;
	unsigned long m_logicBricks;
	unsigned long m_logicNodes;
	unsigned long m_physics;
	unsigned long m_dbvt;
	unsigned long m_sound;
	unsigned long m_bufswaplod;
	unsigned long m_animations;

	unsigned long m_lastRender;
	unsigned long m_lastLogicBricks;
	unsigned long m_lastLogicNodes;
	unsigned long m_lastPhysics;
	unsigned long m_lastDbvt;
	unsigned long m_lastSound;
	unsigned long m_lastBufswaplod;
	unsigned long m_lastAnimations;
	unsigned long m_lastTotal;
public:
	gkStats();

	void resetClock(void);

	void startClock(void);

	void nextFrame(void);

	void stopRenderClock(void);
	void stopLogicBricksClock(void);
	void stopLogicNodesClock(void);
	void stopPhysicsClock(void);
	void stopDbvtClock(void);
	void stopSoundClock(void);
	void stopBufSwapLodClock(void);
	void stopAnimationsClock(void);

	unsigned long getLastRenderMicroSeconds(void)      {return m_lastRender; }
	unsigned long getLastLogicBricksMicroSeconds(void) {return m_lastLogicBricks; }
	unsigned long getLastLogicNodesMicroSeconds(void)  {return m_lastLogicNodes;}
	unsigned long getLastPhysicsMicroSeconds(void)     {return m_lastPhysics;}
	unsigned long getLastDbvtMicroSeconds(void)        {return m_lastDbvt;}
	unsigned long getLastSoundMicroSeconds(void)       {return m_lastSound;}
	unsigned long getLastBufSwapLodMicroSeconds(void)  {return m_lastBufswaplod;}
	unsigned long getLastAnimationsMicroSeconds(void)  {return m_lastAnimations;}
	unsigned long getLastTotalMicroSeconds(void)       {return m_lastTotal;}

	UT_DECLARE_SINGLETON(gkStats);
};

#endif //_gkStats_h_
