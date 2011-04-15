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

#include "akAnimationClip.h"
#include "akAnimationChannel.h"
#include "akSkeleton.h"
#include "akSkeletonPose.h"

akAnimationClip::akAnimationClip()
{
}


akAnimationClip::~akAnimationClip()
{
	akAnimationChannel** ptr = m_channels.ptr();
	int len = getNumChannels(), i;
	for (i = 0; i < len; ++i)
		delete ptr[i];
	
	m_channels.clear();
}


void akAnimationClip::evaluate(akSkeletonPose* pose, akScalar time, akScalar weight, akScalar delta) const
{
	akAnimationChannel* const* ptr = m_channels.ptr();
	int len = getNumChannels();
	
	for(int i=0; i<len; i++)
	{
		if(ptr[i]->getType() == akAnimationChannel::AC_BONE)
		{
			akTransformState* jpose = pose->getByName(ptr[i]->getName());
			if (jpose)
				ptr[i]->evaluate(*jpose, time, weight, delta);
		}
	}
}

void akAnimationClip::evaluate(akTransformState* pose, akScalar time, akScalar weight, akScalar delta) const
{
	akAnimationChannel* const* ptr = m_channels.ptr();
	int len = getNumChannels();
	
	for(int i=0; i<len; i++)
	{
		if(ptr[i]->getType() == akAnimationChannel::AC_TRANSFORM)
		{
			ptr[i]->evaluate(*pose, time, weight, delta);
		}
	}
}

void akAnimationClip::addChannel(akAnimationChannel* chan)
{
	UT_ASSERT(chan);
	m_channels.push_back(chan);
}


akAnimationChannel* akAnimationClip::getChannel(const utString& name)
{
	for (UTsize i = 0; i < m_channels.size(); i++)
	{
		akAnimationChannel* chan = m_channels[i];
		if (chan->getName() == name)
		{
			return chan;
		}
	}
	return 0;
}
