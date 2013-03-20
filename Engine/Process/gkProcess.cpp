/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2013 Thomas Trocha

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

#include "Process/gkProcess.h"

gkProcess::gkProcess()
	:	m_suspended(false),m_loopCount(1),m_initialLoopCount(1)
{}

gkProcess::~gkProcess() {
}

void gkProcess::_init()
{
	m_loopCount = m_initialLoopCount;
	init();
	_onInit();
}

void gkProcess::_update(gkScalar delta)
{
	if (!m_suspended)
		update(delta);
}

void gkProcess::setSuspend(bool suspend)
{
	if (suspend!=m_suspended)
	{
		m_suspended = suspend;
		if (suspend)
			_onSuspend();
		else
			_onResume();
	}

}

bool gkProcess::_isFinished() {

	// call virtual method
	bool procResult = isFinished();

	if (procResult)
	{
		// loopCount=0 means infinite
		if (m_loopCount==0)
		{
			init();
			return false;
		}

		m_loopCount--;
		// still loops to do?
		if (m_loopCount>0) {
			_onLoopEnd();
			init();
			return false;
		}
		_onFinished();
		return true;
	}
	return false;
}

void gkProcess::addListener(Listener* listener)
{
	if (!m_listener.find(listener))
		m_listener.push_back(listener);
}

void gkProcess::removeListener(Listener* listener)
{
	m_listener.erase(listener);
}

void gkProcess::sendNotification(const Listener::Event& e)
{
	ProcessListener::Iterator iter(m_listener);
	while (iter.hasMoreElements())
		iter.getNext()->notifyEvent(this,e);
}
