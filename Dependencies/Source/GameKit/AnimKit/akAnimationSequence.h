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
#ifndef _akAnimationSequence_h_
#define _akAnimationSequence_h_


#include "akAnimation.h"
#include "akAnimationPlayer.h"



///Single action strip that supports blend in and blend out operations
///as part of the larger action sequence.
class akAnimationStrip
{
public:
	akAnimationStrip();
	~akAnimationStrip() {}


	void setAnimation(akAnimation* act);


	///Blend min/max in frames
	void setBlend(const akScalar& blendin, const akScalar& blendout );

	akScalar          getBlendIn(void) const        {return m_blendin;}
	akScalar          getBlendOut(void) const       {return m_blendout;}
	akScalar          getWeight(void) const         {return m_weight; }
	akScalar          getCurrentWeight(void) const  {return m_weight*m_curBlend; }
	akScalar          getCurrentBlend(void) const   {return m_curBlend; }
	akScalar          getStart(void)  const         {return m_scaledRangeStart;}
	akScalar          getEnd(void)    const         {return m_scaledRangeEnd;}
	akScalar          getLength(void) const         {return m_scaledRangeEnd - m_scaledRangeStart;}


	///The start and end frame, the time will be scaled to fit
	void setDeltaRange(const akScalar& start, const akScalar& end );
	void evaluate(akScalar time, akScalar weight, void* object);



private:

	akScalar m_scaledRangeStart, m_scaledRangeEnd;
	akScalar m_curBlend;

	akScalar m_weight;
	akScalar  m_blendin, m_blendout;
	akAnimation* m_anim;
};



///Chain of actions to form a larger set animation sequences.
class akAnimationSequence : public akAnimation
{
public:

	typedef utArray<akAnimationStrip*> Sequences;

public:

	akAnimationSequence();
	virtual ~akAnimationSequence();

	void addItem(akAnimation* act, const akScalar& start, const akScalar& end, const akScalar& blendin = 0, const akScalar& blendout = 0, bool fitLength = true);


	virtual void evaluate(const akScalar& time, const akScalar& weight, void* object) const;

//	void clearWeights(void);

protected:

	Sequences m_seq;
};




#endif//_akAnimationSequence_h_
