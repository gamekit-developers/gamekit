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

#include "gkKeyedAction.h"
#include "gkAnimationChannel.h"

gkKeyedAction::gkKeyedAction(gkResourceManager* creator, const gkResourceName& name, const gkResourceHandle& handle)
	:	gkAction(creator, name, handle)
{
}


gkKeyedAction::~gkKeyedAction()
{
	gkAnimationChannel** ptr = m_channels.ptr();
	int len = getNumChannels(), i;
	for (i = 0; i < len; ++i)
		delete ptr[i];
}


void gkKeyedAction::evaluate(const gkScalar& time, const gkScalar& delta, const gkScalar& weight, gkGameObject* object) const
{
	gkAnimationChannel* const* ptr = m_channels.ptr();
	int len = getNumChannels(), i = 0;
	while (i < len)
		ptr[i++]->evaluate(time, delta, weight, object);
}


void gkKeyedAction::addChannel(gkAnimationChannel* chan)
{
	GK_ASSERT(chan);
	m_channels.push_back(chan);
}


gkAnimationChannel* gkKeyedAction::getChannel(const gkString& name)
{
	for (UTsize i = 0; i < m_channels.size(); i++)
	{
		gkAnimationChannel* chan = m_channels[i];
		if (chan->getName() == name)
		{
			return chan;
		}
	}
	return NULL;
}
