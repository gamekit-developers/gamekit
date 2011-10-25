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

#include "gkMessageManager.h"


gkMessageManager::Message& gkMessageManager::Message::operator = (const Message& m)
{
	m_from = m.m_from;
	m_to = m.m_to;
	m_subject = m.m_subject;
	m_body = m.m_body;
	return *this;
}


void gkMessageManager::GenericMessageListener::handleMessage(gkMessageManager::Message* message)
{
	if (!m_fromFilter.empty() && m_fromFilter.compare(message->m_from) != 0) return;
	if ((!m_toFilter.empty() && m_toFilter.compare(message->m_to) != 0)
			&& (m_acceptEmptyTo && !message->m_to.empty())) return;
	if (!m_subjectFilter.empty() && m_subjectFilter.compare(message->m_subject) != 0) return;

	Message m = *message;

	m_messages.push_back(m);
}


gkMessageManager::gkMessageManager()
{

}


void gkMessageManager::addListener(MessageListener* listener)
{
	if ( m_listeners.find(listener) == UT_NPOS)
		m_listeners.push_back(listener);
}


void gkMessageManager::removeListener(MessageListener* listener)
{
	m_listeners.erase(listener);
}


void gkMessageManager::sendMessage(gkString from, gkString to, gkString subject, gkString body)
{
	Message* m = new Message();
	m->m_from = from;
	m->m_to = to;
	m->m_subject = subject;
	m->m_body = body;

	utArrayIterator<utArray<MessageListener*> > iter(m_listeners);
	while (iter.hasMoreElements())
	{
		iter.peekNext()->handleMessage(m);
		iter.getNext();
	}

	delete m;
}

UT_IMPLEMENT_SINGLETON(gkMessageManager);
