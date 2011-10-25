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

#ifndef _gkMessageManager_h_
#define _gkMessageManager_h_

#include "gkCommon.h"
#include "utSingleton.h"

class gkMessageManager : public utSingleton<gkMessageManager>
{
public:

	struct Message
	{
		gkString m_from;
		gkString m_to;
		gkString m_subject;
		gkString m_body;

		Message& operator = (const Message& m);
	};

	struct    MessageListener
	{
		MessageListener() {}
		virtual ~MessageListener() {}

		virtual void handleMessage(gkMessageManager::Message* message) = 0;
	};

	struct GenericMessageListener : public MessageListener
	{
		gkString m_fromFilter, m_toFilter, m_subjectFilter;
		bool m_acceptEmptyTo;
		utArray<Message> m_messages;

		GenericMessageListener(gkString fromfilter = "", gkString tofilter = "", gkString subjectfilter = "")
			:    m_fromFilter(fromfilter), m_toFilter(tofilter), m_subjectFilter(subjectfilter), m_acceptEmptyTo(false) {}

		void setAcceptEmptyTo(bool accept){this->m_acceptEmptyTo=accept;}
		bool isAcceptingEmptyTo(){return this->m_acceptEmptyTo;}

		~GenericMessageListener() {m_messages.clear();}

		void handleMessage(gkMessageManager::Message* message);
		void emptyMessages() {m_messages.clear();}
	};

private:
	utArray<MessageListener*> m_listeners;

public:
	gkMessageManager();
	virtual ~gkMessageManager() {}

	void addListener(MessageListener* listener);
	void removeListener(MessageListener* listener);
	void sendMessage(gkString from, gkString to, gkString subject, gkString body);

	UT_DECLARE_SINGLETON(gkMessageManager);
};

#endif // _gkMessageManager_h_
