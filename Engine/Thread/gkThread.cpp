/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Nestor Silveira.

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
#include "gkCommon.h"
#include "gkThread.h"
#include "gkLogger.h"

#ifdef WIN32
#include <process.h>
#endif

#ifdef WIN32
unsigned __stdcall gkThread::task(void* p)
{
	gkThread* pThread = static_cast<gkThread*>(p);

	pThread->run();

	_endthreadex(0);

	return 0;
}
#else
void* gkThread::task(void* p)
{
	gkThread* pThread = static_cast<gkThread*>(p);

	pThread->run();
}
#endif

#if __ANDROID__
//http://igourd.blogspot.com/2009/05/work-around-on-pthreadcancel-for.html
void thread_exit_handler(int sig)
{
	//printf("this signal is %d \n", sig);
	pthread_exit(0);
}

void setup_thread_exit_handler()
{
	static bool _inited = false;
	if (_inited)
		return;

	_inited = true;

	struct sigaction actions;
	memset(&actions, 0, sizeof(actions));
	sigemptyset(&actions.sa_mask);
	actions.sa_flags = 0;
	actions.sa_handler = thread_exit_handler;
	int rc = sigaction(SIGUSR1,&actions,NULL);
}

#endif


gkThread::gkThread(gkCall* call)
	: m_call(call)
{
	GK_ASSERT(m_call);

#ifdef WIN32
	m_hChilThread = (HANDLE)_beginthreadex(
	                    0, // no security
	                    65535,
	                    task,
	                    this,
	                    0, // running
	                    &m_threadId
	                );
#else
	int failed = pthread_create(&m_threadId, NULL, &task, this);

	GK_ASSERT(!failed);
#endif

#if __ANDROID__
	setup_thread_exit_handler();
#endif
}

gkThread::~gkThread()
{
#ifdef WIN32
	CloseHandle(m_hChilThread);
#elif __ANDROID__
	int status = 0;
	if ((status = pthread_kill(m_threadId, SIGUSR1)) != 0)
	{
		gkPrintf("Error cancelling thread id:%d error:%d", m_threadId, status);
	}
#else
	pthread_cancel(m_threadId);
#endif
}

void gkThread::join()
{
	m_syncObj.wait();
}

void gkThread::run()
{
	m_call->run();

	m_syncObj.signal();
}
