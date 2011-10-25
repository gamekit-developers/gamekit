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

#include "gkMessageSensor.h"
#include "gkLogicManager.h"
#include "gkLogicDispatcher.h"


gkMessageSensor::gkMessageSensor(gkGameObject* object, gkLogicLink* link, const gkString& name)
	:       gkLogicSensor(object, link, name)
{
	m_listener = new gkMessageManager::GenericMessageListener("",object->getName(),"");
	m_listener->setAcceptEmptyTo(true);
	gkMessageManager::getSingleton().addListener(m_listener);

	m_dispatchType = DIS_CONSTANT;
	connect();
}



gkMessageSensor::~gkMessageSensor()
{
	gkMessageManager::getSingleton().removeListener(m_listener);
	delete m_listener;
	m_messages.clear();
}


gkLogicBrick* gkMessageSensor::clone(gkLogicLink* link, gkGameObject* dest)
{
	gkMessageSensor* sens = new gkMessageSensor(*this);
	sens->cloneImpl(link, dest);
	sens->m_listener = new gkMessageManager::GenericMessageListener("", "", m_listener->m_subjectFilter);
	return sens;
}


bool gkMessageSensor::query(void)
{
	bool ret = false;

	m_messages.clear();

	if (m_listener->m_messages.size() > 0 )
	{
		ret = true;

		// We copy the massages so that Logic scripts can retrieve messages recieved between each logic step
		utArrayIterator<utArray<gkMessageManager::Message> > iter(m_listener->m_messages);
		while (iter.hasMoreElements())
		{
			gkMessageManager::Message m = iter.peekNext();
			if (m.m_to==""
					|| m.m_to == m_object->getName())
			{
				m_messages.push_back(m);
			}
			iter.getNext();
		}
	}

	m_listener->emptyMessages();

	return ret;
}
