/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) Nestor Silveira.

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
#include "gkSyncObj.h"
#include "gkCommon.h"

#ifdef WIN32
#include <process.h>
#endif

#ifdef OGREKIT_USE_COCOA

#include <Foundation/NSLock.h>

class gkSyncObjPrivate
{
	enum {GK_HAS_DATA, GK_NO_DATA};

public:
	gkSyncObjPrivate()
	{
m_syncObj = [[NSConditionLock alloc] initWithCondition: GK_NO_DATA];
	}
	~gkSyncObjPrivate()
	{
		[m_syncObj release];
	}
	void wait()
	{
[m_syncObj lockWhenCondition: GK_HAS_DATA];
[m_syncObj unlockWithCondition: GK_NO_DATA];
	}
	void signal()
	{
		[m_syncObj lock];
[m_syncObj unlockWithCondition: GK_HAS_DATA];
	}

private:
	NSConditionLock* m_syncObj;

};

#endif

gkSyncObj::gkSyncObj()
{
#ifdef OGREKIT_USE_COCOA

	m_syncObj = new gkSyncObjPrivate;

#elif WIN32
	m_syncObj = CreateSemaphore(
	                0,    // Security attributes
	                0,    // Initial count zero (everybody will wait)
	                std::numeric_limits<int>::max(), // the mamimum number of resources
	                0    // No name for this semaphore
	            );
#elif __APPLE__

	OSStatus result = MPCreateSemaphore(std::numeric_limits<int>::max(), 0, &m_syncObj);

	GK_ASSERT(result == 0);

#else
	int result = sem_init(&m_syncObj, 0, 0);

	GK_ASSERT(result != -1);
#endif
}

gkSyncObj::~gkSyncObj()
{
#ifdef OGREKIT_USE_COCOA
	delete m_syncObj;
#elif WIN32
	CloseHandle(m_syncObj);
#elif __APPLE__
	OSStatus result = MPDeleteSemaphore(m_syncObj);

	GK_ASSERT(result == 0);
#else
	int result = sem_destroy(&m_syncObj);
	GK_ASSERT(result != -1);
#endif
}

bool gkSyncObj::wait()
{
	bool ok = true;

#ifdef OGREKIT_USE_COCOA

	m_syncObj->wait();

#elif WIN32
	switch (WaitForSingleObject(m_syncObj, INFINITE))
	{
	case WAIT_TIMEOUT:
		ok = false;
		break;

	default: // OK
		break;
	}
#elif __APPLE__
	OSStatus result = MPWaitOnSemaphore (m_syncObj, kDurationForever);
	GK_ASSERT(result == 0);
#else
	int result = sem_wait(&m_syncObj);
	GK_ASSERT(result != -1);
#endif

	return ok;
}

bool gkSyncObj::signal()
{
#ifdef OGREKIT_USE_COCOA

	m_syncObj->signal();

#elif WIN32
	if (!ReleaseSemaphore(m_syncObj, 1, 0))
		return false;
#elif __APPLE__
	OSStatus result = MPSignalSemaphore (m_syncObj);
	GK_ASSERT(result == 0);
#else
	int result = sem_post(&m_syncObj);
	GK_ASSERT(result != -1);
#endif
	return true;
}
