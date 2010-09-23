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
#ifndef _gkActionSequence_h_
#define _gkActionSequence_h_


#include "Animation/gkAction.h"



///Single action strip that supports blend in and blend out operations
///as part of the larger action sequence.
class gkActionStrip
{
public:
	gkActionStrip();
	~gkActionStrip() {}


	void setAction(gkAction* act)         {m_action = act;}


	///Blend min/max in frames
	void setBlend(const gkVector2& io);

	gkAction*          getAction(void)    {return m_action;}
	const gkVector2&  getBlend(void)      {return m_blend;}
	gkScalar          getWeight(void)     {return m_curBlend; }
	gkScalar          getStart(void)      {return m_scaledRange.x;}
	gkScalar          getEnd(void)        {return m_scaledRange.y;}
	gkScalar          getLength(void)     {return m_scaledRange.y - m_scaledRange.x;}


	///The start and end frame, the time will be scaled to fit
	void setDeltaRange(const gkVector2& v);

	void enable(bool v);

	void evaluate(const gkScalar& delta);
	void reset(void);

	///Scales weights as a whole, for use in blending between seperate gkActionSequence objects
	void setParentWeight(gkScalar w);



private:

	gkVector2 m_scaledRange;
	gkScalar  m_scaleDelta, m_parentWeight;

	gkScalar   m_curBlend, m_time, m_prevDelta;
	gkAction*  m_action;
	gkVector2  m_blend;
	bool       m_init;
};



///Chain of actions to form a larger set animation sequences.
class gkActionSequence
{
public:

	typedef utArray<gkActionStrip*> Sequences;

public:

	gkActionSequence();
	~gkActionSequence();


	void addItem(gkAction* act, const gkVector2& timeRange, const gkVector2& blendIO = gkVector2(0.f, 0.f));


	///The default / underlying sequence item to play when weight's are zero.
	void setDefault(gkAction* act) {m_default = act;}

	gkAction* getActiveStrip(void) {return m_active;}


	gkScalar getStart(void)    {return m_range.x;}
	gkScalar getEnd(void)      {return m_range.y;}
	gkScalar getLength(void)   {return m_range.y - m_range.x;}


	void      setTimePosition(const gkScalar& v);
	gkScalar  getTimePosition(void)              {return m_evalTime;}


	GK_INLINE void enable(bool v)   {m_enabled = v;}
	GK_INLINE bool isEnabled(void)  {return m_enabled;}
	GK_INLINE bool isDone(void)     {return !m_enabled || m_evalTime >= getLength();}


	void evaluate(const gkScalar& delta);

	void setWeight(gkScalar w);

	void clearWeights(void);
	void reset(void);

protected:

	Sequences m_seq;

	bool m_timeDirty;
	gkVector2 m_range;
	gkScalar  m_evalTime, m_weight, m_defTime;
	gkAction*  m_default;
	gkAction*  m_active;
	bool m_enabled;
};




#endif//_gkActionSequence_h_
