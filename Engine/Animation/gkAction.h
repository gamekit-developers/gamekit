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
#ifndef _gkAction_h_
#define _gkAction_h_

#include "gkCommon.h"
#include "gkMathUtils.h"
#include "gkBezierSpline.h"


class gkActionChannel;

enum gkActionEvalMode
{
	///Reset loop when done.
	GK_ACT_LOOP    = (1 << 0),
	///Play till the end and stop.
	GK_ACT_END     = (1 << 1),
	///Invert frames
	GK_ACT_INVERSE = (1 << 2)
};



class gkAction
{
public:
	typedef utArray<gkActionChannel*> Channels;


protected:

	const gkString       m_name;
	Channels             m_channels;
	gkScalar             m_start, m_end;
	gkScalar             m_evalTime, m_weight, m_blendFrames;
	bool                 m_enabled;
	int                  m_mode;
	gkGameObjectChannel* m_objectChannel;

public:
	gkAction(const gkString& name);
	~gkAction();

	GK_INLINE gkScalar         getLength(void) const       { return m_end - m_start; }
	GK_INLINE gkScalar         getStart(void) const        { return m_start; }
	GK_INLINE gkScalar         getEnd(void) const          { return m_end; }
	GK_INLINE const gkString&  getName(void) const         { return m_name; }
	GK_INLINE gkScalar         getBlendFrames(void) const  { return m_blendFrames; }
	GK_INLINE gkScalar         getTimePosition(void) const { return m_evalTime; }
	GK_INLINE gkScalar         getWeight(void) const       { return m_weight; }
	GK_INLINE int              getMode(void) const         { return m_mode; }


	GK_INLINE gkAction::Channels::ConstPointer getChannels(void) const {return m_channels.ptr();}
	GK_INLINE int getNumChannels(void) const {return(int)m_channels.size();}


	GK_INLINE void setStart(gkScalar v) { m_start = v;}
	GK_INLINE void setEnd(gkScalar v)   { m_end = v;}
	GK_INLINE void setMode(int v)       { m_mode = v; }


	void addChannel(gkActionChannel* chan);
	gkActionChannel* getChannel(gkBone* bone);
	
	GK_INLINE void setObjectChannel(gkGameObjectChannel* chan)  { m_objectChannel = chan; }
	GK_INLINE gkGameObjectChannel* getObjectChannel(void)       { return m_objectChannel; }


	void setBlendFrames(gkScalar v);
	void setTimePosition(gkScalar v);
	void setWeight(gkScalar w);


	GK_INLINE void enable(bool v)        {m_enabled = v;}
	GK_INLINE bool isEnabled(void) const {return m_enabled;}
	GK_INLINE bool isDone(void) const    {return !m_enabled || m_evalTime >= (m_end - m_start);}



	void evaluate(gkScalar time);
	void reset(void);
};


#endif//_gkAction_h_
