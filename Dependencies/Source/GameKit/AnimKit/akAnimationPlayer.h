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

#ifndef AKANIMATIONPLAYER_H
#define AKANIMATIONPLAYER_H

#include "akCommon.h"
#include "akMathUtils.h"


/// A player is linked to an animation clip and contains temporal
/// information specific to a single object that can play the animation clip.
/// This permit having several players paying the same clip et the same time
/// but at a different positon in this clip and apply the results to different objects.
class akAnimationPlayer
{
public:
	enum Mode {
		///Reset loop when done.
		AK_ACT_LOOP    = (1 << 0),
		///Play till the end and stop.
		AK_ACT_END     = (1 << 1)
	};

protected:
	akAnimationClip*     m_clip;
	akScalar             m_length;
	akScalar             m_evalTime;
	akScalar             m_speedfactor;
	akScalar             m_weight;
	int                  m_mode;
	bool                 m_enabled;


public:
	akAnimationPlayer();
	akAnimationPlayer(akAnimationClip* clip);
	~akAnimationPlayer() {}
	
	void setAnimationClip(akAnimationClip* v);
	void setTimePosition(akScalar v);
	void setUniformTimePosition(akScalar v);
	
	akScalar getUniformTimePosition(void) const;

	void stepTime(akScalar tick);
	void reset(void);

	void evaluate(akSkeletonPose* pose) const;
	void evaluate(akTransformState* pose) const;
	
	UT_INLINE akScalar getTimePosition(void) const 
	{
		return m_evalTime;
	}
	
	UT_INLINE int              getMode(void) const         
	{
		return m_mode;
	}
	
	UT_INLINE akScalar         getSpeedFactor(void) const  
	{
		return m_speedfactor;
	}
	
	UT_INLINE akScalar         getLength(void) const       
	{
		return m_length;
	}
	
	UT_INLINE akScalar         getWeight(void) const       
	{
		return m_weight;
	}
	
	UT_INLINE bool             isEnabled(void) const       
	{
		return m_enabled;
	}
	
	UT_INLINE void             setMode(Mode v)                      
	{
		m_mode = v;
	}
	
	UT_INLINE void             setSpeedFactor(akScalar v)          
	{
		m_speedfactor = v; 
	}
	
	UT_INLINE void             setLength(akScalar v)                
	{
		m_length = v; 
	}
	
	UT_INLINE void             setEnabled(bool v)
	{
		m_enabled = v;
	}
	
	UT_INLINE void             setWeight(akScalar v)
	{
		m_weight = v;
	}
	
	UT_INLINE bool             isDone(void) const
	{
		return m_evalTime >= m_length; 
	}

};

#endif // AKANIMATIONPLAYER_H
