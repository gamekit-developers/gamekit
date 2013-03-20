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

#ifndef _gkProcess_h_
#define _gkProcess_h_
#include "gkMathUtils.h"
#include "Process/gkProcess.h"

class gkProcess {
	friend class gkProcessManager;
	friend class gsProcess;

public:

	class Listener
	{
	public:

		enum Event
		{
			INIT,
			FINSHED,
			LOOP_END,
			SUSPENDED,
			RESUMED
		};

	public:

		virtual ~Listener() {}

		virtual void notifyEvent(gkProcess* proc, const Event& id) = 0;
	};

	typedef utList<Listener*> ProcessListener;

	gkProcess();
	virtual ~gkProcess();

	virtual bool isFinished() { return true; }
	virtual void init() {}
	virtual void update(gkScalar delta) {}
	GK_INLINE int getLoopCount(void) { return m_initialLoopCount; }
	GK_INLINE void setLoopCount(int loopCount) { m_initialLoopCount = loopCount; }
	GK_INLINE bool isSuspended(void) { return m_suspended;}
	void setSuspend(bool suspend);

	void addListener(Listener* listener);
	void removeListener(Listener* listener);
	void sendNotification(const Listener::Event& e);

protected:
	int m_loopCount,m_initialLoopCount;
	bool m_suspended;
	ProcessListener m_listener;

	void _init(void);
	void _update(gkScalar delta);
	bool _isFinished(void);

#define CALL_EVENT(METHOD,EVENT)\
	void _##METHOD() {\
		sendNotification(EVENT);\
	}\

	CALL_EVENT(onInit,Listener::INIT)
	CALL_EVENT(onFinished,Listener::FINSHED)
	CALL_EVENT(onLoopEnd,Listener::LOOP_END)
	CALL_EVENT(onSuspend,Listener::SUSPENDED)
	CALL_EVENT(onResume,Listener::RESUMED)
};


#endif /* GKPROCESS_H_ */
