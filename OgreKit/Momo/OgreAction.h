/*
-------------------------------------------------------------------------------
	This file is part of OgreKit.
	http://gamekit.googlecode.com/

	Copyright (c) 2009 Charlie C.
-------------------------------------------------------------------------------
 This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
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
#ifndef _OgreAction_h_
#define _OgreAction_h_

#include "OgrePrerequisites.h"
#include "OgreBezierSpline.h"
#include "OgreArray.h"
#include "OgreHashedString.h"
#include "OgreUtilsAllocator.h"



namespace Ogre 
{
class ActionChannel;


/// Holds ActionChannel objects, one for each Bone.
class Action : public AllocObject
{
public:
	typedef Array<ActionChannel*> Channels;


protected:

	const HashedString	m_name;
	Channels			m_channels;
	float				m_start, m_end;
	float				m_evalTime, m_weight, m_blendFrames;

public:
	Action(const HashedString& name);
	~Action();

	OGRE_INLINE float getLength(void)
	{ return m_end - m_start; }

	// Sets the start frame of this action.
	OGRE_INLINE void setStart(float v)
	{ m_start = v;}
	
	// Sets the end frame of this action.
	OGRE_INLINE void setEnd(float v)
	{ m_end = v;}
	
	// Gets the start frame of this action.
	OGRE_INLINE float getStart(void)
	{ return m_start;}
	
	// Sets the end frame of this action.
	OGRE_INLINE float getEnd(void)
	{ return m_end;}

	// Gets the action/animation name. 
	OGRE_INLINE const HashedString& getName(void)
	{return m_name;}

	/// Appends a new channel for this action.
	void addChannel(ActionChannel *chan);

	/// Raw const access to channels
	OGRE_INLINE Action::Channels::ConstPointer getChannels(void)
	{return m_channels.ptr();}

	/// Gets the current channel count.
	OGRE_INLINE int getNumChannels(void)
	{return (int)m_channels.size();}

	/// Updates matrices for the specified time 
	void evaluate(float time);

	OGRE_INLINE void setBlendFrames(float v)
	{m_blendFrames = OgreClamp(v, 1, m_end);}

	OGRE_INLINE float getBlendFrames(void)
	{return m_blendFrames;}

	OGRE_INLINE void setTimePosition(float v)	
	{m_evalTime = OgreClamp(v, m_start, m_end);}
	
	OGRE_INLINE float getTimePosition(void)
	{return m_evalTime;}

	OGRE_INLINE void setWeight(float w) 
	{m_weight = OgreClamp(w, 0, 1); }

	OGRE_INLINE float getWeight(void)
	{ return m_weight; }
};

}


#endif//_OgreAction_h_
