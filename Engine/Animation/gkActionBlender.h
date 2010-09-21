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
#ifndef _gkActionBlender_h_
#define _gkActionBlender_h_


#include "Animation/gkAction.h"
#include "gkActionSequence.h"



///Action blend item, for overall blending / switching between actions
class gkActionBlend
{
public:

	enum Way
	{
		AB_NONE,
		AB_IN,
		AB_OUT
	};

public:

	gkActionBlend();
	~gkActionBlend() {}


	int      getMode(void) const         {return m_mode;}
	int      getDirection(void) const    {return m_way; }
	int      getPriority(void) const     {return m_priority;}
	gkScalar getBlendFrames(void) const  {return 1.f / m_frames; }


	gkAction*         getAction(void)   {return m_base;}
	gkActionSequence* getSequence(void) {return m_sequ;}


	gkScalar getLength(void);

	void enable(bool v);
	GK_INLINE bool isEnabled(void)  {return m_enabled;}
	GK_INLINE bool isDone(void)     {return !m_enabled || m_time >= getLength();}

	void setMode(int mode)                          {m_mode = mode;}
	void setDirection(int way)                      {m_way = way;}
	void setPriority(int pri)                       {m_priority = pri;}
	void setAction(gkAction *act)                   {m_base = act;}
	void setActionSequence(gkActionSequence *act)   {m_sequ = act;}

	void setBlendFrames(gkScalar f);
	bool evaluate(gkScalar delta);

	void reset(void);


	bool operator == (const gkActionBlend& rhs) const { return m_base == rhs.m_base && m_sequ == rhs.m_sequ;}

private:

	int m_priority, m_way, m_mode;
	gkScalar m_blend, m_frames;
	gkScalar m_time;


	gkAction *m_base;
	gkActionSequence *m_sequ;
	bool m_enabled;
};




///Pushes prioritized actions onto a stack for changing and
///blending between a chain of gkAction or gkActionSequence objects
class gkActionBlender
{
public:
	typedef utArray<gkActionBlend> Stack;

public:

	gkActionBlender();
	~gkActionBlender();

	void push(gkAction *action, gkScalar frames, int mode = GK_ACT_END, int priority = 0);
	void push(gkActionSequence *action, gkScalar frames, int mode = GK_ACT_END, int priority = 0);

	void evaluate(gkScalar delta);


	void   setMaximumActions(UTsize v) {m_max = v;}
	UTsize getMaximumActions(void)     {return m_max;}



private:

	void pushStack(gkActionBlend &blend);

	Stack  m_stack;
	UTsize m_max;
};


#endif//_gkActionBlender_h_
