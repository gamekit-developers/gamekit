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


#include "gkAction.h"
#include "gkActionPlayer.h"



///Single action strip that supports blend in and blend out operations
///as part of the larger action sequence.
class gkActionStrip
{
public:
	gkActionStrip();
	~gkActionStrip() {}


	void setAction(gkAction* act);


	///Blend min/max in frames
	void setBlend(const gkVector2& io);

//	gkActionPlayer*   getActionPlayer(void) const  {return &m_actionPlayer;}
	const gkVector2&  getBlend(void)  const         {return m_blend;}
	gkScalar          getWeight(void) const         {return m_weight; }
	gkScalar          getCurrentWeight(void) const  {return m_weight*m_curBlend; }
	gkScalar          getCurrentBlend(void) const   {return m_curBlend; }
	gkScalar          getStart(void)  const         {return m_scaledRange.x;}
	gkScalar          getEnd(void)    const         {return m_scaledRange.y;}
	gkScalar          getLength(void) const         {return m_scaledRange.y - m_scaledRange.x;}


	///The start and end frame, the time will be scaled to fit
	void setDeltaRange(const gkVector2& v);

	void enable(bool v);

	void evaluate(gkScalar time, gkScalar weight, gkGameObject* object);
	void reset(void);



private:

	gkVector2 m_scaledRange;
	gkScalar m_curBlend;

	gkScalar m_weight;
	gkActionPlayer  m_actionPlayer;
	gkVector2  m_blend;
};



///Chain of actions to form a larger set animation sequences.
class gkActionSequence : public gkAction
{
public:

	typedef utArray<gkActionStrip*> Sequences;

public:

	gkActionSequence(gkResourceManager* creator, const gkResourceName& name, const gkResourceHandle& handle);
	virtual ~gkActionSequence();


	void addItem(const gkString& act, const gkVector2& timeRange, const gkVector2& blendIO = gkVector2(0.f, 0.f));
	void addItem(gkAction* act, const gkVector2& timeRange, const gkVector2& blendIO = gkVector2(0.f, 0.f));


	virtual void evaluate(const gkScalar& time, const gkScalar& delta, const gkScalar& weight, gkGameObject* object) const
	{
		UTsize i, s;
		gkActionStrip* const* p;
		
		i = 0;
		s = m_seq.size();
		p = m_seq.ptr();
		
		
		while (i < s)
		{
			gkActionStrip* seq = p[i++];
			const gkScalar st  = seq->getStart();
			const gkScalar end = st + seq->getEnd();
			
			
			if (time < st || time > end)
			{
				// out of current range
				continue;
			}
			
			
			if (time <= m_start)
			{
				// out of initial starting range
				continue;
			}
			
			
			//		m_active = seq->getAction();
			seq->evaluate(time-st, weight, object);
		}
	}

//	void clearWeights(void);

protected:

	Sequences m_seq;
};




#endif//_gkActionSequence_h_
