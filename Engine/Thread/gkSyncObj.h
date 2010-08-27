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
#ifndef _gkSyncObj_h_
#define _gkSyncObj_h_

#include "gkNonCopyable.h"

#ifdef OGREKIT_USE_COCOA
#include <pthread.h>
#include <semaphore.h>
class gkSyncObjPrivate;
#elif WIN32
#include <windows.h>
#elif __APPLE__
#include <pthread.h>
#include <semaphore.h>
#include <Carbon/Carbon.h>
#else
#include <pthread.h>
#include <semaphore.h>
#endif

class gkSyncObj : gkNonCopyable
{
public:

	gkSyncObj();

	~gkSyncObj();

	bool wait();

	bool signal();

private:

#ifdef OGREKIT_USE_COCOA
	gkSyncObjPrivate* m_syncObj;
#elif WIN32
	HANDLE m_syncObj;
#elif __APPLE__
	MPSemaphoreID m_syncObj;
#else
	sem_t m_syncObj;
#endif

};

#endif//_gkSyncObj_h_
