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
#include "gkSkeleton.h"


class gkActionChannel;


// Holds gkActionChannel objects, one for each Bone.
class gkAction
{
public:
	typedef utArray<gkActionChannel *> Channels;


protected:

	const gkString      m_name;
	Channels            m_channels;
	gkScalar            m_start, m_end;
	gkScalar            m_evalTime, m_weight, m_blendFrames;

public:
	gkAction(const gkString &name);
	~gkAction();

	GK_INLINE gkScalar getLength(void)
	{ return m_end - m_start; }

	// Sets the start frame of this action.
	GK_INLINE void setStart(gkScalar v)
	{ m_start = v;}

	// Sets the end frame of this action.
	GK_INLINE void setEnd(gkScalar v)
	{ m_end = v;}

	// Gets the start frame of this action.
	GK_INLINE gkScalar getStart(void)
	{ return m_start;}

	// Gets the end frame of this action.
	GK_INLINE gkScalar getEnd(void)
	{ return m_end;}

	// Gets the action/animation name.
	GK_INLINE const gkString &getName(void)
	{ return m_name;}

	// Appends a new channel for this action.
	void addChannel(gkActionChannel *chan);

	// Raw const access to channels
	GK_INLINE gkAction::Channels::ConstPointer getChannels(void)
	{return m_channels.ptr();}

	// Gets the current channel count.
	GK_INLINE int getNumChannels(void)
	{return(int)m_channels.size();}

	//Gets the channel corresponding to a bone
	gkActionChannel *getChannel(gkBone *bone);

	// Updates matrices for the specified time
	void evaluate(gkScalar time);

	GK_INLINE void setBlendFrames(gkScalar v)
	{m_blendFrames = gkClampf(v, 1, m_end);}

	GK_INLINE gkScalar getBlendFrames(void)
	{return m_blendFrames;}

	GK_INLINE void setTimePosition(gkScalar v)
	{m_evalTime = gkClampf(v, m_start, m_end);}

	GK_INLINE gkScalar getTimePosition(void)
	{return m_evalTime;}

	GK_INLINE void setWeight(gkScalar w)
	{m_weight = gkClampf(w, 0, 1); }

	GK_INLINE gkScalar getWeight(void)
	{ return m_weight; }
};


#endif//_gkAction_h_
