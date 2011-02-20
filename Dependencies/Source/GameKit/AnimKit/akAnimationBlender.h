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
#ifndef _akAnimationBlender_h_
#define _akAnimationBlender_h_

#include "akCommon.h"
#include "akMathUtils.h"

#include "utTypes.h"

///Animation blend item, for overall blending / switching between actions
class akAnimationBlend
{
public:

	enum Way
	{
		AB_NONE,
		AB_IN,
		AB_OUT
	};

public:

	akAnimationBlend();
	~akAnimationBlend() {}


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


	bool operator == (const akAnimationBlend& rhs) const { return m_base == rhs.m_base;}

private:

	int m_priority, m_way, m_mode;
	akScalar m_blend, m_frames;
	akScalar m_time;


	akAnimationPlayer* m_base;
	bool m_enabled;
};




///Pushes prioritized animation player onto a stack for changing and
///blending between a chain of Animation
class akAnimationBlender
{
public:
	typedef utArray<akAnimationBlend> Stack;

public:

	akAnimationBlender();
	~akAnimationBlender();

	void push(akAnimationPlayer* action, const akScalar& frames, int mode = AK_ACT_END, int priority = 0);
	void remove(akAnimationPlayer* action);

	void evaluate(akScalar delta);


	void   setMaximumAnimations(UTsize v)    {m_max = v;}
	UTsize getMaximumAnimation(void) const  {return m_max;}



private:

	void pushStack(akAnimationBlend& blend);

	Stack  m_stack;
	UTsize m_max;
};


#endif//_akAnimationBlender_h_
