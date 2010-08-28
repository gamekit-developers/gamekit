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
#include "gkAction.h"
#include "gkActionChannel.h"





// ----------------------------------------------------------------------------
gkAction::gkAction(const gkString &name) 
	:	m_name(name), m_start(1), m_end(1), m_evalTime(1), m_weight(1.0),
		m_blendFrames(1.0)
{
}


// ----------------------------------------------------------------------------
gkAction::~gkAction()
{
	gkActionChannel **ptr = m_channels.ptr();
	int len = getNumChannels(), i;
	for (i = 0; i < len; ++i)
		delete ptr[i];
}


// ----------------------------------------------------------------------------
void gkAction::addChannel(gkActionChannel *chan)
{
	GK_ASSERT(chan);
	m_channels.push_back(chan);
}

// ----------------------------------------------------------------------------
gkActionChannel *gkAction::getChannel(gkBone *bone)
{
	for (UTsize i=0; i<m_channels.size(); i++)
	{
		gkActionChannel *chan = m_channels[i];
		if (chan->getBone()==bone)
		{
			return chan;
		}
	}
	return NULL;
}


// ----------------------------------------------------------------------------
void gkAction::evaluate(gkScalar time)
{
	// loop for now
	if (m_evalTime <= m_start)
		m_evalTime = m_start;
	if (m_evalTime >= m_end)
		m_evalTime = m_start;

	m_evalTime += time;

	gkScalar delta = (m_evalTime - m_start) / (m_end - m_start);

	gkActionChannel **ptr = m_channels.ptr();
	int len = getNumChannels(), i = 0;
	while (i < len)
		ptr[i++]->evaluate(m_evalTime, delta, m_weight);
}
