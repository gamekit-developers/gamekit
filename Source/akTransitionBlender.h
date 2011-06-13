/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Charlie C.

    Contributor(s): Xavier T.
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

#ifndef AKTRANSITIONBLENDER_H
#define AKTRANSITIONBLENDER_H

#include "akCommon.h"
#include "akMathUtils.h"
#include "akAnimationPlayer.h"

#include "utTypes.h"

///Animation blend item, for overall blending / switching between actions
class akTransitionBlend
{
public:

	enum Way
	{
		AB_NONE,
		AB_IN,
		AB_OUT
	};

private:

	int m_priority, m_way, m_mode;
	akScalar m_blend, m_frames;
	akScalar m_time;


	akAnimationPlayer* m_base;
	bool m_enabled;
	
public:

	akTransitionBlend();
	~akTransitionBlend() {}


	int      getMode(void) const         {return m_mode;}
	int      getDirection(void) const    {return m_way; }
	int      getPriority(void) const     {return m_priority;}
	akScalar getBlendFrames(void) const  {return 1.f / m_frames; }


	akAnimationPlayer*         getAnimationPlayer(void)   {return m_base;}


	akScalar getLength(void) const;

	void enable(bool v);
	UT_INLINE bool isEnabled(void) const {return m_enabled;}
	UT_INLINE bool isDone(void) const    {return !m_enabled || m_time >= getLength();}

	void setMode(int mode)                          {m_mode = mode;}
	void setDirection(int way)                      {m_way = way;}
	void setPriority(int pri)                       {m_priority = pri;}
	void setAnimationPlayer(akAnimationPlayer* act) {m_base = act;}

	void setBlendFrames(akScalar f);
	bool evaluate(akScalar delta);

	void reset(void);


	bool operator == (const akTransitionBlend& rhs) const { return m_base == rhs.m_base;}
};




///Pushes prioritized animation player onto a stack for changing and
///blending between a chain of Animation
class akTransitionBlender
{
public:
	typedef utArray<akTransitionBlend> Stack;

public:

	akTransitionBlender();
	~akTransitionBlender();

	void push(akAnimationPlayer* action, const akScalar& frames, int mode = akAnimationPlayer::AK_ACT_END, int priority = 0);

	void evaluate(akScalar delta);


	UT_INLINE void   setMaximumAnimations(UTsize v)
	{
		m_max = v;
	}
	
	UT_INLINE UTsize getMaximumAnimation(void) const
	{
		return m_max;
	}



private:

	void pushStack(akTransitionBlend& blend);

	Stack  m_stack;
	UTsize m_max;
};

#endif // AKTRANSITIONBLENDER_H
