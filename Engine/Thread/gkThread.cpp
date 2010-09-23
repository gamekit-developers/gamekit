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
}

gkThread::~gkThread()
{
#ifdef WIN32
	CloseHandle(m_hChilThread);
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
