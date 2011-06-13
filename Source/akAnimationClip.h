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

#ifndef AKKEYEDANIMATION_H
#define AKKEYEDANIMATION_H


#include "utCommon.h"
#include "utTypes.h"
#include "utString.h"

#include "akCommon.h"
#include "akMathUtils.h"


/// Animation clip contains all the animation data for a specific animation.
/// It can be shared by different players.
/// It is composed of channels which are composed of curves for each animated data part of the animation clip.
class akAnimationClip
{
public:
	typedef utArray<akAnimationChannel*> Channels;
	
protected:
	akScalar m_length;
	Channels m_channels;

public:
	akAnimationClip();
	~akAnimationClip();

	void addChannel(akAnimationChannel* chan);
	bool removeChannel(const akAnimationChannel* chan);
	akAnimationChannel* getChannel(const utHashedString& name);
	
	void evaluate(akPose* pose, akScalar time, akScalar weight=1.0f, akScalar delta=0.5f) const;
	void evaluate(akSkeletonPose* pose, akScalar time, akScalar weight=1.0f, akScalar delta=0.5f) const;
	void evaluate(akTransformState* pose, akScalar time, akScalar weight, akScalar delta=0.5f) const;
	
	UT_INLINE Channels::ConstPointer getChannels(void) const
	{
		return m_channels.ptr();
	}
	
	UT_INLINE int getNumChannels(void) const
	{
		return(int)m_channels.size();
	}
	
	UT_INLINE akScalar getLength(void)
	{
		return m_length;
	}
	
	UT_INLINE void setLength(akScalar v)
	{
		m_length = v;
	}
	
};


#endif // AKKEYEDANIMATION_H
